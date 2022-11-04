/*
 *
 */

#include "writer.h"
#include "./../Rtps.h"


using namespace omnetpp;


Define_Module(Writer);


void Writer::initialize()
{
    // Create the internal event messages
    sendEvent = new cMessage("sendEvent");
    // heartBeat message time
    hbTimer = new cMessage("hbTimer");

    /// Initialize RTPS context
    Rtps* rtpsParent = dynamic_cast<Rtps*>(getParentModule());
    entityId = rtpsParent->getNextEntityId();
    appID = par("appID");

    // writer parametrization
    deadline = par("deadline");
    fragmentSize = par("fragmentSize");
    numReaders = par("numReaders");
    sizeCache = par("historySize");

    shaping = par("shaping");
    hbPeriod = par("hbPeriod");

    // reader proxy initialization
    for(int i = 0; i < numReaders; i++) {
        // TODO set up reader IDs here as well!
        unsigned int readerId = i;
        auto rp = new ReaderProxy(readerId, this->sizeCache);
        matchedReaders.push_back(rp);
    }
}

void Writer::finish()
{

}



void Writer::handleMessage(cMessage *msg)
{
    if (dynamic_cast<Sample*>(msg)!=NULL){
		// Received new sample from application
		Sample *sample = check_and_cast<Sample*>(msg);

		addSampleToCache(sample);

		sendMessage();
    }
    else if(dynamic_cast<RtpsInetPacket*>(msg)!=NULL)
    {
        // Received new NackFrag
        RtpsInetPacket *rtpsMsg = check_and_cast<RtpsInetPacket*>(msg);
        handleNackFrag(rtpsMsg);
    }
    else if(msg == sendEvent)
    {
		sendMessage();
	}
    else if(msg == hbTimer)
    {
        sendHeartbeatMsg();
	}

	delete msg;
}


void Writer::addSampleToCache(Sample* sample)
{
    // create cacheChange once
    auto change = new CacheChange(sample->getSequenceNumber(), sample->getSize(), this->fragmentSize, simTime());

    // then generate ChangeForReaders based on CacheChange and add to reader proxies (done by ReaderProxy itself)
    for (auto rp: matchedReaders)
    {
        rp->addChange(*change);
    }
}




bool Writer::sendMessage() // TODO implement completely
{
    // Check if the writer is currently allowed to send
    if(sendEvent->isScheduled()){
        return false;
    }

    // check liveliness of current sample, if deadline expired remove sample from cache and ReaderProxies

    // then select a reader

    // finally select a fragment from the previously chosen reader for transmission
    // update fragment status (at all readers if multicast is used)

    // actual send fragment using RtpsInetPacket

    // Schedule the next event with ideal shaping
    simtime_t timeToSend = simTime() + shaping;
    scheduleAt(timeToSend, sendEvent);
}

bool Writer::sendHeartbeatMsg()
{
    // first get the relevant sample
    // could choose any reader, as we assume multicast and not multiple unicast streams here
    ChangeForReader* change = matchedReaders[0]->getCurrentChange();

    RtpsInetPacket* rtpsMsg = new RtpsInetPacket();
    rtpsMsg->setInfoDestinationSet(false);

    // get highest sent fragment number from the change
    rtpsMsg->setLastFragmentNum(change->highestFNSend);

    // set all other message attributes accordingly
    rtpsMsg->setHeartBeatFragSet(true);
//    rtpsMsg->setUcId(-1); // TODO for simulation only (otherwise sometimes data missing, would likely to be communicated via discovery protocol IRL)
    rtpsMsg->setSampleSize(change->sampleSize);
    rtpsMsg->setFragmentSize(this->fragmentSize);
    rtpsMsg->setGeneralFragmentSize(this->fragmentSize);

    // calc and set message size
    calculateRtpsMsgSize(rtpsMsg);

    // transmit HB message
    cModule *target = getAnalysisModule(getParentModule());
    if(target != nullptr){
        send(rtpsMsg , gate("dispatcher_out"));
    }
    // schedule next heartbeat
    scheduleAt(simTime() + hbPeriod, hbTimer);
}



void Writer::handleNackFrag(RtpsInetPacket* nackFrag) { // TODO implement completely
    /// First find the history cache corresponding to the reader, sending the NackFrag msg
    // TODO how to implement entity IDs simulation wide and allow for easy assignment here?
    unsigned int readerID = nackFrag->getReaderId();
    auto rp = matchedReaders[readerID];

    rp->processNack(nackFrag);
    unsigned int sequenceNumber = nackFrag->getWriterSN();
    bool complete = rp->checkSampleCompleteness(sequenceNumber);
    // TODO how to proceed from here?

    // TODO: default RTPS behavior: just retransmit all fragments marked as missing asap
    // add missing fragments to sendQueue
}








SampleFragment* Writer::selectNextFragment(ReaderProxy *rp) { // TODO implement completely

}

RtpsInetPacket* Writer::createRtpsMsgFromFragment(SampleFragment* sample_fragment) { // TODO implement completely

}
