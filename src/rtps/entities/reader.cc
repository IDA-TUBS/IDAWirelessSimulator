/*
 *
 */

#include "reader.h"

using namespace omnetpp;
Define_Module(Reader);


void Reader::initialize()
{
    // TODO assigning of entity IDs
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






