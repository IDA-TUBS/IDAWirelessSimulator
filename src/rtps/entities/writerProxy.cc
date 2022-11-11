/*
 *
 */

#include "writerProxy.h"


bool WriterProxy::addChange(CacheChange &change)
{
    if(change.sequenceNumber == highestSequenceNumber || history.size() == historySize)
    {
        // no new sample, no need to add to history
        return false;
    }
    ChangeForWriter* cfr = new ChangeForWriter(change);

    history.push_back(cfr);

    return true;
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
    // access change with the given sequence number
    ChangeForWriter* change = nullptr;
    for (auto cfw: history)
    {
        if (cfw->sequenceNumber == sequenceNumber)
        {
            change = cfw;
            break;
        }
    }

    bool complete = change->checkForCompleteness();
    if(complete)
    {
        // TODO remove sample from history? or do at a different point in time
    }
    return complete;
}
