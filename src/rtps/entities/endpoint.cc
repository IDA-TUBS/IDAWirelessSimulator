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
