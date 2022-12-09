/*
 *
 */

#include "writerW2RP.h"

#include <random>
#include <algorithm>
#include <chrono>

using namespace omnetpp;

Define_Module(WriterW2RP);

void WriterW2RP::initialize()
{
    // Create the internal event messages
    sendEvent = new cMessage("sendEvent");
    // timeout event
    timeoutEvent = new cMessage("timeoutEvent");

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
    enableNackSuppression = par("enableNackSuppression");
    nackSuppressionDuration = par("nackSuppressionDuration");

    // always disable sending of separate HB messages
    enableSeparateHBs = false;

    // W2RP extension
    prioMode = NONE;
    timeout = par("timeout");

    // reader proxy initialization
    for(int i = 0; i < numReaders; i++) {
        // the app's reader IDs are in the range of [appID * maxNumberReader + 1, (appID + 1) * maxNumberReader - 1]
        unsigned int readerId = this->appID * rtpsParent->getMaxNumberOfReaders() + i + 1;

        ReaderProxy* rp = nullptr;
        if(enableNackSuppression)
        {
            rp = new ReaderProxy(readerId, this->sizeCache, nackSuppressionDuration);
        }
        else
        {
            rp = new ReaderProxy(readerId, this->sizeCache);
        }
        matchedReaders.push_back(rp);
    }

    // set up currentSampleNumber for new writer instance
    currentSampleNumber = -1;

    activeTimeouts = 0;

    // analysis related code
    RTPSAnalysis::registerAppID(this->appID);
}

void WriterW2RP::finish()
{
    RTPSAnalysis::calculateCombinedViolationRate();
    EV << "Total application deadline violation rate: " << this->combinedViolationRate << endl;
}


void WriterW2RP::handleMessage(cMessage *msg)
{
    if(msg == timeoutEvent)
        {
            sendMessage();

        } else
    {
        Writer::handleMessage(msg);
    }
}


void WriterW2RP::handleDiscovery()
{
}


void WriterW2RP::handleNackFrag(RtpsInetPacket* nackFrag) {
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
//        if(cf
    }
}



ReaderProxy* WriterW2RP::selectReader()
{
    ReaderProxy *nextReader = nullptr;
    // check whether prioritization mechanism shall be used

        // non-prioritized reader selection - in random order
        std::vector<int> v;
        for(int i = 0; i < numReaders; i++)
        {
            v.push_back(i);
        }
        unsigned seed = std::chrono::system_clock::now()
                                .time_since_epoch()
                                .count();
    auto rng = std::default_random_engine { seed };
    std::shuffle(std::begin(v), std::end(v), rng);

    for (auto i: v)
    {
        auto rp = matchedReaders[i];
        if(!(rp->checkSampleCompleteness(currentSampleNumber)))
         {
           nextReader = rp;
        }     }


    return nextReader;
}


SampleFragment* WriterW2RP::selectNextFragment(ReaderProxy* rp)
{
//    EV << "Select next fragment at time: " << simTime() << "\n";
    // TODO Implement prioritization method

    // find the unacknowledged fragment and return that fragment for transmission
    SampleFragment *tmp = nullptr;
    SampleFragment *tmp_proxy = nullptr;
    SampleFragment **fragments = rp->getCurrentChange()->getFragmentArray();

    for (int i = 0; i < rp->getCurrentChange()->numberFragments; i++)
    {
        SampleFragment* sf = fragments[i];
        SampleFragment* baseFragment = (sf->baseChange->getFragmentArray())[sf->fragmentStartingNum];

//        EV << "NAKCED FN: " << sf->sent << "\n";
//        EV << "NAKCED FN: " << baseFragment->sent << "\n\n";
        //        EV <<"FN: " << baseFragment->fragmentStartingNum << "; sendCounter: " << baseFragment->sendCounter << "; Send Timestamp: " << baseFragment->sendTime << "\n";
        //        EV <<"FN: " << sf->fragmentStartingNum << "; sendCounter: " << sf->sendCounter << "; Send Timestamp: " << sf->sendTime << "\n";


//        EV << "Status frag " << sf->fragmentStartingNum << "; sent: " << sf->sent << "; acked: " << sf->acked << "; sent timestamp: " << baseFragment->sendTime << "\n";

        // Ignore ACKed fragments
        if (sf->acked) {
            continue;
        }


//        bool send = sf->sent | baseFragment->sent;
//        bool acked = sf->acked | baseFragment->acked;
//        simtime_t = max(sf->sendTime,baseFragment->sendTime);

        // Priority 1: Unsent fragments
        if(baseFragment->sendCounter == 0){
            tmp = baseFragment;
            tmp_proxy = sf;
            break;
        }

        // Priority 2: NACKed fragments
        if(baseFragment->sendCounter > 0 && !sf->sent){

            // Start with the first one
            if(tmp == nullptr){
                tmp = baseFragment;
                tmp_proxy = sf;
//                EV << "(1) FN: " << baseFragment->fragmentStartingNum << ": Prio 2\n";
                continue;
            }

            // Choose the earliest fragment of this class
            if(tmp->sendTime > baseFragment->sendTime){
//                EV << "(2) FN: " << baseFragment->fragmentStartingNum << ": Prio 2\n";
                tmp = baseFragment;
                tmp_proxy = sf;
            }
        }
    }

    if(tmp_proxy != nullptr){
        return tmp_proxy;
    }

    EV << "Check timeout condition\n";

    // Timeout condition
    for (int i = 0; i < rp->getCurrentChange()->numberFragments; i++)
    {
        SampleFragment* sf = fragments[i];
        SampleFragment* baseFragment = (sf->baseChange->getFragmentArray())[sf->fragmentStartingNum];

        // Ignore ACKed fragments
        if (sf->acked) {
            continue;
        }

        if(simTime() - baseFragment->sendTime < this->timeout && sf->sent != 0){
            continue;
        }

        if(tmp == nullptr){
            tmp = baseFragment;
            tmp_proxy = sf;
            continue;
        }

        if(tmp->sendTime > baseFragment->sendTime){
            tmp = baseFragment;
            tmp_proxy = sf;
        }

    }

    return tmp_proxy;

}


bool WriterW2RP::sendMessage() // TODO ist diese methode identisch mit der aus dem writer? Dann bitte ersetzen durch den entsprechenden aufruf + timeout aufruf wenn nötig (wenn kein fragment zurück  gegeben wurde...)
{
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
    currentSampleNumber = historyCache.front()->sequenceNumber;

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
            } else {
                scheduleTimeout();
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
        }

        // construct RtpsInetPacket from fragment and send out to dispatcher
        if(destinationAddresses.size() > 1)
        {
            throw cRuntimeError("Handling of multiple addresses not implemented yet!");
        }
        std::string addr = destinationAddresses[0];

        auto msg = createRtpsMsgFromFragment(sf, this->entityId, this->fragmentSize, addr, this->appID, fragmentCounter);
        addHBFrag(msg, matchedReaders[0]->getCurrentChange()->highestFNSend);
        send(msg , gate("dispatcherOut"));
        sf->sendTime = simTime(); // TODO Wurde nirgends anders aufgerufen. Vllt. besser im Adapter?
        fragmentCounter++;

//        EV << "FN: " << sf->fragmentStartingNum << "; time: " << simTime() << "\n";

        // analysis related code:
        RTPSAnalysis::addFragment(this->appID, sf);
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











    if(!sendEvent->isScheduled()){
        scheduleTimeout();
    }

    return true;

}


void WriterW2RP::fillSendQueueWithSample(unsigned int sequenceNumber)
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

bool WriterW2RP::stopScheduledTimer()
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




void WriterW2RP::scheduleTimeout()
{

    ReaderProxy *rp = nullptr;

    SampleFragment* tmp = nullptr;
    SampleFragment* tmp_proxy = nullptr;

    if(rp = selectReader())
    {

        SampleFragment **fragments = rp->getCurrentChange()->getFragmentArray();

        for (int i = 0; i < rp->getCurrentChange()->numberFragments; i++)
        {

            SampleFragment* sf = fragments[i];
            SampleFragment* baseFragment = (sf->baseChange->getFragmentArray())[sf->fragmentStartingNum];

            // Ignore ACKed
            if (sf->acked || !sf->sent) {
                continue;
            }

            // Check, that the timeout for the fragment did not already expire
            if(baseFragment->sendTime + this->timeout <= simTime()){
                continue;
            }

            // Get the smalles timeout
            if(tmp == nullptr){
                tmp = baseFragment;
                tmp_proxy = sf;
                continue;
            }

            // Choose the next relevant timeout
            if(tmp->sendTime > baseFragment->sendTime)
            {
                tmp = baseFragment;
                tmp_proxy = sf;
            }

        }

        if(tmp == nullptr){
            return;
        }

//        EV << "Status frag " << tmp->fragmentStartingNum << "; sent: " << tmp_proxy->sent << "; acked: " << tmp_proxy->acked << "; sent timestamp: " << tmp->sendTime << "\n";

        if(timeoutEvent->isScheduled()){
            cancelEvent(timeoutEvent);
        }

        scheduleAt(tmp->sendTime + this->timeout, timeoutEvent);

    }
}

