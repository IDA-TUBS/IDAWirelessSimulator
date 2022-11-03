/*
 *
 */

#include "writerProxy.h"


void WriterProxy::addChange(CacheChange &change)
{
    ChangeForWriter* cfr = new ChangeForWriter(this->readerID, change);

    history.push(cfr);
}

bool WriterProxy::updateFragmentStatus (fragmentStates status, unsigned int sequenceNumber, unsigned int fragmentNumber)
{
    // first find change corresponding to the given sequence number
    ChangeForWriter* tmp = nullptr;
    for (auto cfw: history)
    {
        if (cfw->getSequenceNumber() == sequenceNumber)
        {
            tmp = cfr;
            break;
        }
    }

    return cfw.setFragmentStatus(status, fragmentNumber);
}


bool WriterProxy::checkSampleCompleteness(unsigned int sequenceNumber)
{
    // TODO implement
}
