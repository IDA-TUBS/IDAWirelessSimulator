/*
 *
 */

#include "writer.h"


using namespace omnetpp;

Define_Module(Writer);

Writer::~Writer()
{
}

void Writer::initialize()
{
    /// Create the internal event messages
    sendEvent = new cMessage("sendEvent");
    /// heartBeat message time
    hbTimer = new cMessage("hbTimer");

    /// Initialize RTPS context
    Rtps* rtps_parent = dynamic_cast<Rtps*>(getParentModule());
    entityID = rtps_parent->getNextEntityId();
    appID = par("appID");

    /// writer parametrization
    deadline = par("deadline");
    fragmentSize = par("fragmentSize");
    numReaders = par("numReaders");

    /// reader proxy initialization
    for(int i = 0; i < numReader; i++) {
        auto rp = new ReaderProxy();
        matchedReader.push_back(rp);
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


void Writer::addSampleToCache()
{
    // create cache change once, then copy to reader proxies
}




bool Writer::sendMessage()
{

}

bool Writer::sendHeartbeatMsg()
{

}



void Writer::handleNackFrag(RtpsInetPacket* nackFrag) {
    /// First find the history cache corresponding to the reader, sending the NackFrag msg
    unsigned int readerID = nackFrag->getReaderId(); // TODO does not work if there are multiple writers sending to different nodes!!!
    auto rp = matchedReaders[readerID];

    rp->processNackFrag(nackFrag);
    unsigned int sequenceNumber = nackFrag->getWriterSN();
    bool complete = rp->checkSampleCompleteness(sequenceNumber);
    // TODO how to proceed from here?
}








SampleFragment* Writer::select_next_fragment(ReaderProxy *rp) {

}

RtpsInetPacket* Writer::create_rtps_msg_from_fragment(SampleFragment* sample_fragment) {

}
