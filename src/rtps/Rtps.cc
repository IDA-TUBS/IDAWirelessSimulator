/*
 *
 */


#include <omnetpp.h>
#include "Rtps.h"

using namespace omnetpp;

Define_Module(Rtps);

Rtps::~Rtps()
{
   // cancelAndDelete(selfMsg);
}

void Rtps::initialize()
{
    // Entity id is needed to dispatch message destinations
    entityIdCounter = 0;
}

void Rtps::handleMessage(cMessage *msg)
{
}
