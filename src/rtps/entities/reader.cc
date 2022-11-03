/*
 *
 */

#include "reader.h"

Define_Module(Reader);


void Reader::initialize()
{
    // TODO assigning of entity IDs
    appID = par("appID");

    writerProxy = new WriterProxy();

    responseDelay = 0;
}


void Reader::finish()
{

}

void Reader::handleMessage(cMessage *msg)
{

}

void Reader::sendMessage(RtpsInetPacket* rtps_msg)
{

}




void Reader::addNewSampleToProxy(RtpsInetPacket* rtpsInetPacket)
{

}

RtpsInetPacket* Reader::generateNackFrag(WriterProxy* wp)
{

}

void Reader::checkCompletionOfLatestSample()
{

}






