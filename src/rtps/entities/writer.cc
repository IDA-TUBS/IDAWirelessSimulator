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
    enableSeparateHBs = par("enableSeparateHBs");
    hbPeriod = par("hbPeriod");
    enableNackSuppression = par("enableNackSuppression");
    measureEfficiency = par("measureEfficiency");
    nackSuppressionDuration = par("nackSuppressionDuration");
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

    // analysis related code
    RTPSAnalysis::registerAppID(this->appID);
}

void Writer::finish()
{
    if(this->measureEfficiency){
        RTPSAnalysis::maximumNumberOfUnnecessaryRetransmissionsPerSampleVector.record(maximumNumberOfUnnecessaryRetransmissionsPerSample);
    }
    RTPSAnalysis::calculateCombinedViolationRate();
    EV << "Total application deadline violation rate: " << this->combinedViolationRate << endl;
    recordScalar("appViolationRate", this->combinedViolationRate);

}



void Writer::handleMessage(cMessage *msg)
{
    // Check message type
    if (dynamic_cast<Sample*>(msg)!=NULL){

        // "discovery" setup
        if(currentSampleNumber == -1)
        {
            handleDiscovery();
        }

		// Received new sample from application
		Sample *sample = check_and_cast<Sample*>(msg);

		simtime_t x = simTime();
        double t = x.dbl();
        unsigned int sn = sample->getSequenceNumber();

		// first check existing samples for deadline expiry
		checkSampleLiveliness();
		addSampleToCache(sample);

		sendMessage();

		if(enableSeparateHBs)
		{
		    // schedule periodic heartbeats
            if(hbTimer->isScheduled()){
                cancelEvent(hbTimer);
            }
            scheduleAt(simTime() + hbPeriod, hbTimer);
		}
		delete msg;


    }
    else if(dynamic_cast<RtpsInetPacket*>(msg)!=NULL)
    {
        // Received new NackFrag
        RtpsInetPacket *rtpsMsg = check_and_cast<RtpsInetPacket*>(msg);

        // first check whether appID is corresponding to the writer's appID
        if(rtpsMsg->getAppId() != this->appID)
        {
            return;
        }
//        return; // TODO remove
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


void Writer::handleDiscovery()
{
    // no additional setup needed here so far
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

    // analysis related code
    RTPSAnalysis::addSample(this->appID, change);
    return true;
}


void Writer::checkSampleLiveliness()
{
    if(historyCache.size() == 0)
    {
        while(sendQueue.size() > 0){
            SampleFragment* to_delete_element = sendQueue.front();
            sendQueue.erase(sendQueue.begin());
            delete to_delete_element;
        }
//        sendQueue.clear();
        return;
    }

    std::vector<unsigned int> deprecatedSNs;
    std::vector<CacheChange*> toDelete;
    // check liveliness of samples in history cache, if deadline expired remove sample from cache and ReaderProxies
    auto* change = historyCache.front();
    while(1)
    {
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
        // assumption here: samples put into history cache in the right order AND
        // all samples have the same deadline, hence any sample in the history
        // following the first valid sample is also still valid!
        break;
    }

    for (unsigned int sequenceNumber: deprecatedSNs)
    {
        for (auto rp: matchedReaders)
        {
            if(!(rp->changeExists(sequenceNumber)))
            {
                continue;
            }
            if(!(rp->checkSampleCompleteness(sequenceNumber)))
            {
                RTPSAnalysis::incrementIncompleteCounter();
            }
            else
            {
                RTPSAnalysis::incrementCompleteCounter();
            }
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
    //toDelete.clear();
    while(toDelete.size() > 0){
        CacheChange* to_delete_element = toDelete.front();
        toDelete.erase(toDelete.begin());
        delete to_delete_element;
    }

}




void Writer::removeCompleteSamples()
{
    if(historyCache.size() == 0)
    {
        return;
    }

    // iterate over all changes, remove those that are complete at ALL (!!) readers
    while(1)
    {
        auto* change = historyCache.front();

        if(!change)
        {
            break;
        }

        bool completed = true;
        for(auto rp: matchedReaders)
        {
            if(!(rp->checkSampleCompleteness(change->sequenceNumber)))
            {
                completed = false;
                break;
            }
        }

        if(completed)
        {
            // remove if change successfully acknowledged by all readers
            for(auto rp: matchedReaders)
            {
                rp->removeChange(change->sequenceNumber);
                RTPSAnalysis::incrementCompleteCounter();
            }
            historyCache.pop_front();
            // remove all queued fragments of that sample from the send queue
            for(auto it = sendQueue.cbegin(); it != sendQueue.end(); it++)
            {
                // remove fragments if their sequence number matches that of the just removed change
                unsigned int sequenceNumber = (*it)->baseChange->sequenceNumber;

                if(sequenceNumber == change->sequenceNumber)
                {
                    sendQueue.erase(it--);
                    break;
                }

            }
            delete change;
        }
        else
        {
            // assumption here: samples put into history cache in the right order AND
            // a consecutive sample will only be transmitted after a previous sample is
            // either complete or its deadline expired. Hence, there is no need to check
            // further samples for completeness if there is any incomplete sample
            break;
        }
//        if(historyCache.size() == 0 || change == historyCache.back())
//        {
//            break;
//        }
    }
}



ReaderProxy* Writer::selectReader()
{
    // for retransmissions this does not matter anyway, as default RTPS just retransmits any
    // negatively acknowledged fragment immediately, hence just select the first one here
    // only used for queuing fragments at the beginning
    ReaderProxy* rp = matchedReaders[0];

    return rp;
}


SampleFragment* Writer::selectNextFragment(ReaderProxy* rp)
{
    // find the unacknowledged fragment and return that fragment for transmission
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
        if(measureEfficiency){
            RTPSAnalysis:handleEfficiencyOnWriter(this->appID, sf->baseChange->sequenceNumber, sf->fragmentStartingNum, sf->sendCounter);
        }
        send(msg , gate("dispatcherOut"));
        sf->sendTime = simTime(); // TODO Wurde nirgends anders aufgerufen. Vllt. besser im Adapter?
        fragmentCounter++;


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

    return true;
}


bool Writer::sendHeartbeatMsg()
{
    // first get the relevant sample
    // could choose any reader as we assume either uni- or multicast and not multiple unicast streams here
    ChangeForReader* change = matchedReaders[0]->getCurrentChange();

    if(change)
    {
        // TODO move the following code to a separate method in class Endpoint
        // only send heartbeat if there is a sample in the history cache
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
    }

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

    // only handle NackFrag if sample still in history, if already complete or expired just ignore NackFrag
    if(rp->processNack(nackFrag))
    {
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
        if(!sendEvent->isScheduled())
        {
            scheduleAt(simTime(), sendEvent);
        }
    }
}

bool Writer::stopScheduledTimer()
{
    return true;
}

