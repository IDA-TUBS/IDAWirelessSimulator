/*
 *
 */

#include "writerProxy.h"


void WriterProxy::addChange(CacheChange &change)
{
    if(change.sequenceNumber == highestSequenceNumber)
    {
        // no new sample, no need to add to history
        return;
    }
    /// TODO check cache size prior to adding new change
    ChangeForWriter* cfr = new ChangeForWriter(change);

    history.push_back(cfr);
}

bool WriterProxy::updateFragmentStatus (fragmentStates status, unsigned int sequenceNumber, unsigned int fragmentNumber)
{
    // first find change corresponding to the given sequence number
    ChangeForWriter* tmp = nullptr;
    for (auto cfw: history)
    {
        if (cfw->sequenceNumber == sequenceNumber)
        {
            tmp = cfw;
            break;
        }
    }

    return tmp->setFragmentStatus(status, fragmentNumber);
}


bool WriterProxy::checkSampleCompleteness(unsigned int sequenceNumber)
{
    // TODO implement
}
