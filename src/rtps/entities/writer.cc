/*
 *
 */

#include "writer.h"

using namespace omnetpp;

Define_Module(Writer);

void Writer::initialize()
{
    // Create the internal event messages
    sendEvent = new cMessage("sendEvent");
    // heartBeat message time
    hbTimer = new cMessage("hbTimer");

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
    hbPeriod = par("hbPeriod");

    // reader proxy initialization
    for(int i = 0; i < numReaders; i++) {
        // the app's reader IDs are in the range of [appID * maxNumberReader + 1, (appID + 1) * maxNumberReader - 1]
        unsigned int readerId = this->appID * rtpsParent->getMaxNumberOfReaders() + i + 1;
        auto rp = new ReaderProxy(readerId, this->sizeCache);
        matchedReaders.push_back(rp);
    }
}

void Writer::finish()
{

}



void Writer::handleMessage(cMessage *msg)
{
    // Check message type
    if (dynamic_cast<Sample*>(msg)!=NULL){
		// Received new sample from application
		Sample *sample = check_and_cast<Sample*>(msg);

		// first check existing samples for deadline expiry
		checkSampleLiveliness();

		addSampleToCache(sample);

		// TODO if new sample is the only sample in the historyCache: priming send queue with all fragments of the new sample

		sendMessage();

		// schedule periodic heartbeats
		if(hbTimer->isScheduled()){
            cancelEvent(hbTimer);
        }
        scheduleAt(simTime() + hbPeriod, hbTimer);

		delete msg;
    }
    else if(dynamic_cast<RtpsInetPacket*>(msg)!=NULL)
    {
        // Received new NackFrag
        RtpsInetPacket *rtpsMsg = check_and_cast<RtpsInetPacket*>(msg);
        handleNackFrag(rtpsMsg);
        delete msg;
    }
    else if(msg == sendEvent)
    {
		sendMessage();
	}
    else if(msg == hbTimer)
    {
        sendHeartbeatMsg();
	}


}


bool Writer::addSampleToCache(Sample* sample)
{
    // create cacheChange once
    auto change = new CacheChange(sample->getSequenceNumber(), sample->getSize(), this->fragmentSize, simTime());

    if(historyCache.size() == sizeCache)
    {
        return false;
    }
    historyCache.push_back(change);

    // then generate ChangeForReaders based on CacheChange and add to reader proxies (done by ReaderProxy itself)
    for (auto rp: matchedReaders)
    {
        rp->addChange(*change);
    }

    return true;
}


void Writer::checkSampleLiveliness()
{
    if(historyCache.size() == 0){
        return;
    }

    std::vector<unsigned int> deprecatedSNs;
    std::vector<CacheChange*> toDelete;
    // check liveliness of samples in history cache, if deadline expired remove sample from cache and ReaderProxies
    while(1){
        auto* change = historyCache.front();

        if(!change->isValid(this->deadline))
        {
            deprecatedSNs.push_back(change->sequenceNumber);
            historyCache.pop_front();
            toDelete.push_back(change); // delete all expired changes in the end
            if(historyCache.size() == 0)
            {
                // deleted only existing entry in cache
                break;
            }
        }
        if(change == historyCache.back())
        {
            break;
        }
    }

    for (unsigned int sequenceNumber: deprecatedSNs)
    {
        for (auto rp: matchedReaders)
        {
            rp->removeChange(sequenceNumber);
        }
    }

    if(!sendQueue.empty())
    {

        // also purge fragments of expired samples from sendQueue
        for(auto it = sendQueue.cbegin(); it != sendQueue.end(); it++)
        {
            // remove fragments if their sequence number matches any of those in deprecatedSNs
            unsigned int sequenceNumber = (*it)->baseChange->sequenceNumber;
            for(unsigned int deprecatedSn: deprecatedSNs)
            {
                if(sequenceNumber == deprecatedSn)
                {
                    sendQueue.erase(it--);
                    break;
                }
            }
        }
    }

    // finally delete expired changes
    toDelete.clear();
}


ReaderProxy* Writer::selectReader()
{
    // for retransmissions this does not matter anyway, as default RTPS just retransmits any
    // negatively acknowledged fragment immediately, hence just select the first one
    ReaderProxy* rp = matchedReaders[0];

    return rp;
}


SampleFragment* Writer::selectNextFragment(ReaderProxy* rp)
{
    SampleFragment *tmp = nullptr;
    SampleFragment **fragments = rp->getCurrentChange()->getFragmentArray();
    for (int i = 0; i < rp->getCurrentChange()->numberFragments; i++)
    {
        SampleFragment* sf = fragments[i];
        if (sf->sent || sf->acked) {
            continue;
        }
        // take the first unsent and unacknowledged fragment
        tmp = sf;
        break;
    }
    return tmp;
}


bool Writer::sendMessage()
{
    // Check if the writer is currently allowed to send
    if(sendEvent->isScheduled()){
        return false;
    }

    // check liveliness of sample in history cache
    checkSampleLiveliness();

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
        }

        // construct RtpsInetPacket from fragment and send out to dispatcher
        if(destinationAddresses.size() > 1)
        {
            throw cRuntimeError("Handling of multiple addresses not implemented yet!");
        }
        std::string addr = destinationAddresses[0];

        auto msg = createRtpsMsgFromFragment(sf, this->entityId, this->fragmentSize, addr, this->appID);
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


bool Writer::sendHeartbeatMsg()
{
    // first get the relevant sample
    // could choose any reader, as we assume multicast and not multiple unicast streams here
    ChangeForReader* change = matchedReaders[0]->getCurrentChange();

    RtpsInetPacket* rtpsMsg = new RtpsInetPacket();
    rtpsMsg->setInfoDestinationSet(false);

    if(destinationAddresses.size() > 1)
    {
        throw cRuntimeError("Handling of multiple addresses not implemented yet!");
    }
    std::string addr = destinationAddresses[0];
    // set destination address (needed in InetAdapter)
    rtpsMsg->setDestinationAddress(addr.c_str());

    // get highest sent fragment number from the change
    rtpsMsg->setLastFragmentNum(change->highestFNSend);

    // set all other message attributes accordingly
    rtpsMsg->setHeartBeatFragSet(true);
    rtpsMsg->setSampleSize(change->sampleSize);
    rtpsMsg->setFragmentSize(this->fragmentSize);
    rtpsMsg->setGeneralFragmentSize(this->fragmentSize);

    rtpsMsg->setWriterSN(change->sequenceNumber);
    rtpsMsg->setWriterId(this->entityId);

    // calc and set message size
    calculateRtpsMsgSize(rtpsMsg);

    // transmit HB message
    send(rtpsMsg , gate("dispatcherOut"));

    // schedule next heartbeat
    scheduleAt(simTime() + hbPeriod, hbTimer);
    return true;
}


void Writer::handleNackFrag(RtpsInetPacket* nackFrag) {
    // First find the history cache corresponding to the reader, sending the NackFrag msg
    unsigned int readerID = nackFrag->getReaderId();
    // the app's reader IDs are in the range of [appID * maxNumberReader + 1, (appID + 1) * maxNumberReader - 1]
    // reader entity ID mapped to entityId - thisappID * (maxNumberReader + 1) - 1
    auto rp = matchedReaders[readerID - this->appID * (rtpsParent->getMaxNumberOfReaders() + 1) - 1];

    rp->processNack(nackFrag);
    unsigned int sequenceNumber = nackFrag->getWriterSN();
    bool complete = rp->checkSampleCompleteness(sequenceNumber);


    // default RTPS behavior: just retransmit all fragments marked as missing asap
    // add missing fragments to sendQueue (if not already present)
    auto unsentFragments = rp->getUnsentFragments(sequenceNumber);

    // use actual 'data' sample fragment from history cache instead of sf from reader proxy
    for (auto sf: unsentFragments)
    {
        auto sfToSend = (sf->baseChange->getFragmentArray())[sf->fragmentStartingNum];
        sendQueue.push_back(sfToSend);
    }

    // if no new send event is scheduled, schedule a new one immediately (default rtps behaviour)
    if(!sendEvent->isScheduled()){
        scheduleAt(simTime(), sendEvent);
    }
}

bool Writer::stopScheduledTimer()
{
    return true;
}

