/*
 *
 */

#include "reader.h"
#include "./../Rtps.h"

using namespace omnetpp;

Define_Module(Reader);

void Reader::initialize()
{
    /// Initialize RTPS context
    Rtps* rtpsParent = dynamic_cast<Rtps*>(getParentModule());
    entityId = rtpsParent->getNextEntityId();
    appID = par("appID");

    sizeCache = par("historySize");

    writerProxy = new WriterProxy(this->sizeCache);

    responseDelay = 0;
}


void Reader::finish()
{

}

void Reader::handleMessage(cMessage *msg)
{
    // Check message type
    if((dynamic_cast<RtpsInetPacket*>(msg) != NULL) && msg->isSelfMessage())
    {
        RtpsInetPacket *rtpsMsg = check_and_cast<RtpsInetPacket*>(msg);

        if(rtpsMsg->getDataFragSet())
        {
            // if DataFrag

            // update cache
        }

        if(rtpsMsg->getHeartBeatFragSet())
        {
            // if HB or HBFrag
            // respond with NackFrag
        }
    }

    if (msg->isSelfMessage()) {
        // self message for delaying answering (reader response delay)
        RtpsInetPacket *rtpsMsg = check_and_cast<RtpsInetPacket*>(msg);
        send(rtpsMsg, "dispatcher_out");
    }
}

void Reader::sendMessage(RtpsInetPacket* rtpsMsg)
{
    // wait for specified time (corresponding to reader response delay) before actually sending the NackFrag back to the writer
    scheduleAt(simTime() + responseDelay, rtpsMsg);
}




void Reader::addNewSampleToProxy(RtpsInetPacket* rtpsInetPacket)
{

}

RtpsInetPacket* Reader::generateNackFrag(WriterProxy* wp)
{
    //Create message instance
    RtpsInetPacket* nackFrag = new RtpsInetPacket();

    // Create header
    this->setGuid(nackFrag);

    // Create NackFrag submessage
    nackFrag->setNackFragSet(true);
    nackFrag->setReaderId(entityId);
}

void Reader::checkCompletionOfLatestSample()
{

}






