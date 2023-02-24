/*
 * rtpsmessagedispatcher.cc
 *
 *  Created on: Jan 21, 2020
 *      Author: Jonas Peeck
 */

#include <string.h>
#include <omnetpp.h>

#include "rtpsMessageDispatcher.h"
#include "./../messages/RtpsInetPacket_m.h"


using namespace omnetpp;
using namespace inet;

Define_Module(RtpsMessageDispatcher);

RtpsMessageDispatcher::~RtpsMessageDispatcher()
{
}


void RtpsMessageDispatcher::initialize()
{
}

void RtpsMessageDispatcher::handleMessage(cMessage *msg)
{
    cGate* inGate = msg->getArrivalGate();
    cGate *outGate = nullptr;

    if(inGate == nullptr){
        EV_ERROR <<"ERROR in ["<< this->getClassName()<<"] No valid gate \n";
        throw cRuntimeError("ERROR No valid gate");
        return;
    }

    // Messages from the adapter
    if(inGate->getName() == gate("adapterIn")->getName()){

        // TODO FIXME Currently only one writer OR reader per rtps unit. Implement dispatching based on registry and entity ids here.
        if(gateSize("writerIn")>0){
            outGate = gate("writerOut",0);
        }

        if(gateSize("readerIn")>0){
            outGate = gate("readerOut",0);
        }
    }

    // Messages from writer or reader
    if(gateSize("writerIn")>0){
        if(inGate->getName() == gate("writerIn",0)->getName()){
            outGate = gate("adapterOut");
        }
    }
    if(gateSize("readerIn")>0){
        if(inGate->getName() == gate("readerIn",0)->getName()){
            outGate = gate("adapterOut");
        }
    }

    send(msg, outGate);
}
