/*
 *
 */


#include "endpoint.h"

void Endpoint::setGuid(RtpsInetPacket* rtpsMsg)
{
    rtpsMsg->setInfoDestinationSet(true);
}


cModule* Endpoint::getAnalysisModule(cModule* parent){

    cModule* analysis = nullptr;
    cModule* currentParent = parent;
    while(currentParent != nullptr){
        analysis = currentParent->getSubmodule("rtpsAnalysis");
        if(analysis != nullptr){
            break;
        }
        currentParent = currentParent->getParentModule();
    }

    return analysis;
}


RtpsInetPacket* Endpoint::createRtpsMsgFromFragment(SampleFragment* sampleFragment, unsigned int entityID, unsigned int fragmentSize)
{
    RtpsInetPacket* rtpsMsg = new RtpsInetPacket();
    rtpsMsg->setPayloadSize(0);
    // Header
    rtpsMsg->setPayloadSize(rtpsMsg->getPayloadSize() + 20);
    // InfoDestination
    rtpsMsg->setInfoDestinationSet(false);
    // InfoTimestamp
    if(sampleFragment->fragmentStartingNum == 0){
        rtpsMsg->setInfoTimestampSet(true);
    }
    // Fragment
    rtpsMsg->setWriterId(entityId);
    rtpsMsg->setDataFragSet(true);
    rtpsMsg->setFragmentStartingNum(sampleFragment->fragmentStartingNum);
    rtpsMsg->setFragmentsInSubmessage(1);
    rtpsMsg->setFragmentSize(sampleFragment->dataSize);
    rtpsMsg->setPayloadSize(rtpsMsg->getPayloadSize() + sampleFragment->dataSize);
    rtpsMsg->setWriterSN(sampleFragment->baseChange->sequenceNumber);
    rtpsMsg->setSampleSize(sampleFragment->baseChange->sampleSize);
    rtpsMsg->setPublisherSendTime(sampleFragment->baseChange->arrivalTime);
    rtpsMsg->setFragCount(++(sampleFragment->sendCounter));

    // Other
    rtpsMsg->setGeneralFragmentSize(fragmentSize);

    //misc
//    rtpsMsg->setUcId(-1);

    calculateRtpsMsgSize(rtpsMsg);
    return rtpsMsg;
}

RtpsInetPacket* Endpoint::addHbFragToRtpsMsg(RtpsInetPacket *msg)
{
//    rtpsMsg->setHeartBeatFragSet(true);
//    if(sampleFragment->fragmentStartingNum > sampleFragment->baseChange->highest_entry_send){
//        sampleFragment->base_sample->highest_entry_send = sampleFragment->fragmentStartingNum;
//    }
//    rtpsMsg->setLastFragmentNum(sampleFragment->base_sample->highest_entry_send);

    calculateRtpsMsgSize(msg);
}




void Endpoint::calculateRtpsMsgSize(RtpsInetPacket* rtpsMsg){

    int currentSize = 0;
    /* Add the header size */
    currentSize+=20;

    /* DataFrag */
    if(rtpsMsg->getDataFragSet()){
        currentSize += 36;
        currentSize += rtpsMsg->getFragmentSize();
    }

    /* HeartBeatFrag */
    if(rtpsMsg->getHeartBeatFragSet()){
        currentSize += 28;
    }

//    if(rtpsMsg->getMapEnabledReaders()) {
//        currentSize += 4;
//    }

    /* InfoDestination */
    if(rtpsMsg->getInfoDestinationSet()){
        currentSize += 16;
    }

    /* InfoTimestamp */
    if(rtpsMsg->getInfoTimestampSet()){
        currentSize += 12;
    }

    /* NackFrag */
    if(rtpsMsg->getNackFragSet()){
        int bitmapSizeBytes = (rtpsMsg->getFragmentNumberStateNbrBits()+7)/8;
        int int32AllignedBytes = ((bitmapSizeBytes + 3)/4)*4;
        currentSize += 32 + int32AllignedBytes;
    }

    rtpsMsg->setRtpsMessageSize(currentSize);
}
