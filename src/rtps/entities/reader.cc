/*
 *
 */

#include "reader.h"
#include "./../Rtps.h"

using namespace omnetpp;

Define_Module(Reader);

Reader::~Reader() {
}

void Reader::initialize() {
    // assign entity IDs in .ini file
//    entityId = -1//TODO;
    appID = par("appID");

    writerProxy = new WriterProxy();

    responseDelay = 0;
}


void Reader::finish() {

}

void Reader::handleMessage(cMessage *msg) {

}

void Reader::sendMessage(RtpsInetPacket* rtps_msg)

}




void Reader::addNewSampleToProxy(RtpsInetPacket* rtpsInetPacket) {

}

RtpsInetPacket* Reader::generateNackFrag(WriterProxy* wp) {

}

void Reader::checkCompletionOfLatestSample() {

}






