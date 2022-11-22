/*
 *
 */

#include "writerWiMEP.h"

using namespace omnetpp;

Define_Module(WriterWiMEP);

void WriterWiMEP::initialize()
{
    // Create the internal event messages
    sendEvent = new cMessage("sendEvent");

    /// Initialize RTPS context
    appID = par("appID");
    rtpsParent = dynamic_cast<Rtps*>(getParentModule());
    entityId = rtpsParent->getNextEntityId(appID, true);

    // process destination addresses
    const char *destAddrs = par("destAddresses");
    destinationAddresses = cStringTokenizer(destAddrs).asVector();

    // writer parametrization
    deadline = par("deadline");
    fragmentSize = par("fragmentSize");
    numReaders = par("numberReaders");
    sizeCache = par("historySize");

    shaping = par("shaping");
    timeout = par("timeout");

    // always disable sending of separate HB messages
    enableSeparateHBs = false;

    // multicast extension
    prioritized = par("prioritized");

    // reader proxy initialization
    for(int i = 0; i < numReaders; i++) {
        // the app's reader IDs are in the range of [appID * maxNumberReader + 1, (appID + 1) * maxNumberReader - 1]
        unsigned int readerId = this->appID * rtpsParent->getMaxNumberOfReaders() + i + 1;
        auto rp = new ReaderProxy(readerId, this->sizeCache);
        matchedReaders.push_back(rp);
    }

    // set up currentSampleNumber for new writer instance
    currentSampleNumber = -1;

    activeTimeouts = 0;

    // analysis related code
    RTPSAnalysis::registerAppID(this->appID);
}

void WriterWiMEP::finish()
{

}


void WriterWiMEP::handleMessage(cMessage *msg)
{
    if (dynamic_cast<Timeout*>(msg)!=NULL) {
        Timeout *timeoutMsg = check_and_cast<Timeout*>(msg);
        handleTimeout(timeoutMsg);
        delete msg;
    }
    else
    {
        Writer::handleMessage(msg);
    }
}


void WriterWiMEP::handleDiscovery()
{
    // setup reader priorities
    for(auto rp: matchedReaders)
    {
        rp->setPriority(rtpsParent->getReaderPriority(rp->getReaderId()));
    }
}

void WriterWiMEP::handleTimeout(Timeout *timeoutMsg)
{
    unsigned int readerID = timeoutMsg->getId();
    int sequenceNumber = timeoutMsg->getSequenceNumber();
    auto rp = matchedReaders[readerID - this->appID * (rtpsParent->getMaxNumberOfReaders() + 1) - 1];

    rp->timeoutActive = false;
    rp->resetTimeoutedFragments(sequenceNumber);

    activeTimeouts--;
}

void WriterWiMEP::handleNackFrag(RtpsInetPacket* nackFrag) {
    // First find the history cache corresponding to the reader, sending the NackFrag msg
    unsigned int readerID = nackFrag->getReaderId();
    // the app's reader IDs are in the range of [appID * maxNumberReader + 1, (appID + 1) * maxNumberReader - 1]
    // reader entity ID mapped to entityId - thisappID * (maxNumberReader + 1) - 1
    auto rp = matchedReaders[readerID - this->appID * (rtpsParent->getMaxNumberOfReaders() + 1) - 1];

    // only handle NackFrag if sample still in history, if already complete or expired just ignore NackFrag
    if(rp->processNack(nackFrag))
    {
        unsigned int sequenceNumber = nackFrag->getWriterSN();
        bool complete = rp->checkSampleCompleteness(sequenceNumber);
    }
}






ReaderProxy* WriterWiMEP::selectReader()
{
    ReaderProxy *nextReader = nullptr;
    unsigned int highestPriority = std::numeric_limits<unsigned int>::max();
    // check whether prioritization mechanism shall be used
    if(prioritized)
    {
        // find the highest priority reader that still hasn't received the current sample completely
        for(auto rp: matchedReaders)
        {
            // note: if everything is working correctly currentSampleNumber will be also
            // be the sequence number of the first element in the reader proxy's history
            if(rp->priority < highestPriority && !(rp->checkSampleCompleteness(currentSampleNumber)))
            {
                nextReader = rp;
                highestPriority = rp->priority;
            }
        }
    }
    else
    {
        // TODO
    }
    return nextReader;
}


SampleFragment* WriterWiMEP::selectNextFragment(ReaderProxy* rp)
{
    // used for retransmissions only
    // find the unacknowledged fragment and return that fragment for retransmission
    return Writer::selectNextFragment(rp);
}


bool WriterWiMEP::sendMessage()
{
    // function the same as for generic RTPS Writer, but adds HeartbeatFrag to each transmitted fragment

    // Check if the writer is currently allowed to send
    if(sendEvent->isScheduled()){
        return false;
    }

    // check liveliness of sample in history cache, removes outdated samples
    checkSampleLiveliness();

    // check whether a sample has been successfully transmitted to all readers
    removeCompleteSamples();

    // if no sample left to transmit: no need to transmit anything or schedule a new transmission
    if(historyCache.size() == 0)
    {
        return false;
    }

    if(currentSampleNumber != historyCache.front()->sequenceNumber)
    {
        // new sample to transmit
        currentSampleNumber = historyCache.front()->sequenceNumber;
        // priming send queue with all fragments of the new sample
        fillSendQueueWithSample(currentSampleNumber);
    }

    // differentiate two scenarios:
    // 1. send queue is empty, select a new fragment for tx
    if(sendQueue.empty())
    {
        // add new fragment to queue
        SampleFragment* sf = nullptr;
        ReaderProxy *rp = nullptr;
        // then select a reader
        if(rp = selectReader())
        {
            // finally select a fragment from the previously chosen reader for transmission

            if(sf = selectNextFragment(rp))
            {
                // add sample fragment to send queue
                // use actual 'data' sample fragment from history cache instead of sf from reader proxy
                auto sfToSend = (sf->baseChange->getFragmentArray())[sf->fragmentStartingNum];
                sendQueue.push_back(sfToSend);
            }
        }
    }

    if(!sendQueue.empty())
    {
        SampleFragment* sf = sendQueue.front();
        sendQueue.pop_front();
        // update fragment status (at all reader proxies if multicast is used)
        for(auto rp: matchedReaders)
        {
            rp->updateFragmentStatus(SENT, sf->baseChange->sequenceNumber, sf->fragmentStartingNum);

            // check for timeout situation: reader has no fragments in state 'UNSENT' left
            if(rp->checkForTimeout(sf->baseChange->sequenceNumber) && !(rp->timeoutActive))
            {
                rp->timeoutActive = true;
                // trigger timeout
                auto nextTimeout = new Timeout("timeoutEvent");
                nextTimeout->setId(rp->getReaderId());
                nextTimeout->setSequenceNumber(sf->baseChange->sequenceNumber);

                activeTimeouts++;

                scheduleAt(simTime() + timeout, nextTimeout);
            }
        }

        // construct RtpsInetPacket from fragment and send out to dispatcher
        if(destinationAddresses.size() > 1)
        {
            throw cRuntimeError("Handling of multiple addresses not implemented yet!");
        }
        std::string addr = destinationAddresses[0];

        auto msg = createRtpsMsgFromFragment(sf, this->entityId, this->fragmentSize, addr, this->appID);
        // TODO append HBFrag to the message
        addHBFrag(msg, matchedReaders[0]->getCurrentChange()->highestFNSend);

        send(msg , gate("dispatcherOut"));
    }
    else
    {
        // do not schedule a new event if not desired
        if(stopScheduledTimer())
        {
            return true;
        }
    }

    // Schedule the next event
    simtime_t timeToSend = simTime() + shaping;
    scheduleAt(timeToSend, sendEvent);

    return true;
}


void WriterWiMEP::fillSendQueueWithSample(unsigned int sequenceNumber)
{
    // for multicast all readers need the same data. Just take the first reader here,
    // as it does not make a difference in the transmission phase which on is used
    // for fragment selection - each readers needs each fragment once

    ReaderProxy* rp = matchedReaders[0];

    auto unsentFragments = rp->getUnsentFragments(sequenceNumber);
    // use actual 'data' sample fragment from history cache instead of sf from reader proxy
    for (auto sf: unsentFragments)
    {
        auto sfToSend = (sf->baseChange->getFragmentArray())[sf->fragmentStartingNum];
        sendQueue.push_back(sfToSend);
    }
}

bool WriterWiMEP::stopScheduledTimer()
{
    if(activeTimeouts > 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}
