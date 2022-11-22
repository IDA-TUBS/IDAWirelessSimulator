/*
 *
 */

#include "writerProxy.h"


bool WriterProxy::addChange(CacheChange &change)
{
    if(change.sequenceNumber == highestSequenceNumber) // || history.size() == historySize)
    {
        // no new sample, no need to add to history
        return false;
    }

    ChangeForWriter* cfr = new ChangeForWriter(change);

    history.push_back(cfr);
    highestSequenceNumber = change.sequenceNumber;

    return true;
}

void WriterProxy::removeChange(unsigned int sequenceNumber)
{
    for (auto it = history.begin(); it != history.end();)
    {
        if ((*it)->sequenceNumber <= sequenceNumber)
        {
            history.erase(it);
            return;
        }
        else
        {
            ++it;
        }
    }
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

    if(tmp)
    {
        return tmp->setFragmentStatus(status, fragmentNumber);
    }
    return false;
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

    bool complete = false;
    if(change)
    {
        complete = change->checkForCompleteness();
    }
    if(complete)
    {
        // remove sample from history? or do at a different point in time
        // done elsewhere
    }
    return complete;
}
