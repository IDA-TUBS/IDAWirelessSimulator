/*
 *
 */

#include "readerWiMEP.h"

using namespace omnetpp;

Define_Module(ReaderWiMEP);

void ReaderWiMEP::initialize()
{
    /// Initialize RTPS context
    appID = par("appID");
    priority = par("priority");

    /// get unique entity ID
    rtpsParent = dynamic_cast<Rtps*>(getParentModule());
    entityId = rtpsParent->getNextEntityId(appID, false);

    // register priority at global RTPS class
    rtpsParent->setReaderPriority(entityId, priority);

    // process destination addresses
    const char *destAddrs = par("destAddresses");
    destinationAddresses = cStringTokenizer(destAddrs).asVector();

    deadline = par("deadline");

    sizeCache = par("historySize");

    writerProxy = new WriterProxy(this->sizeCache);

    responseDelay = par("readerResponseDelay");
}


void ReaderWiMEP::finish()
{

}
