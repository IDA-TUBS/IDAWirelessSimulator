/*
 *
 */

#include "readerProxy.h"


bool ReaderProxy::addChange(CacheChange &change)
{
    if(history.size() == historySize)
    {
        // cannot add a new change the history
        return false;
    }
    ChangeForReader* cfr = new ChangeForReader(this->readerID, change);

    history.push_back(cfr);
    return true;
}

void ReaderProxy::removeChange(unsigned int sequenceNumber)
{
    for (auto it = history.begin(); it != history.end();)
    {
        if ((*it)->sequenceNumber <= sequenceNumber)
        {
            ChangeForReader* change = (*it);
            history.erase(it);
            delete change;
            return;
        }
        else
        {
            ++it;
        }
    }
}

bool ReaderProxy::changeExists(unsigned int sequenceNumber)
{
    for (auto it = history.begin(); it != history.end();)
    {
        if ((*it)->sequenceNumber <= sequenceNumber)
        {
            return true;
        }
        else
        {
            ++it;
        }
    }

    return false;
}


bool ReaderProxy::updateFragmentStatus (fragmentStates status, unsigned int sequenceNumber, unsigned int fragmentNumber, simtime_t sentTimestamp)
{
    // first find change corresponding to the given sequence number
    ChangeForReader* tmp = nullptr;
    for (auto cfr: history)
    {
        if (cfr->sequenceNumber == sequenceNumber)
        {
            tmp = cfr;
            break;
        }
    }

    return tmp->setFragmentStatus(status, fragmentNumber, sentTimestamp);
}

bool ReaderProxy::processNack(RtpsInetPacket* nackFrag)
{
    // first get relevant sequence number
    unsigned int sequenceNumber = nackFrag->getWriterSN();

    if(history.size() == 0)
    {
        return false;
    }

    // access change with the given sequence number
    ChangeForReader* change = nullptr;
    for(auto cfr: history)
    {
        if (cfr->sequenceNumber == sequenceNumber)
        {
            change = cfr;
            break;
        }
    }
    if(!change)
    {
        return false;
    }

    // Note: we use NackFrags not AckNacks, hence there is no windowing limitation!
    // Iterate through the addressed bitmap region and update the reader's history cache
    for(int i = nackFrag->getFragmentNumberStateBase(); i < nackFrag->getFragmentNumberStateBase() + nackFrag->getFragmentNumberStateNbrBits();i++)
    {
       unsigned int fragmentNumber = i;
       bool acked = nackFrag->getFragmentNumberBitmap(i-nackFrag->getFragmentNumberStateBase());

       SampleFragment* currentFragment = change->getFragmentArray()[fragmentNumber];

        if(currentFragment->acked){
            continue;
        }

        // then update fragment status accordingly
        if(acked == true){
            this->updateFragmentStatus(ACKED, sequenceNumber, i);
            continue;
        }

        if(this->nackSuppressionEnabled && (currentFragment->sendTime + this->nackSuppressionDuration > simTime()))
        {
            // do not process Nacks if NackGuard hasn't expired yet
            continue;
        }

        if(currentFragment->sent){
            this->updateFragmentStatus(NACKED, sequenceNumber, i);
        }
    }

    return true;
}


std::vector<SampleFragment*> ReaderProxy::getUnsentFragments(unsigned int sequenceNumber)
{
    // access change with the given sequence number
    ChangeForReader* change = nullptr;
    for (auto cfr: history)
    {
        if (cfr->sequenceNumber == sequenceNumber)
        {
            change = cfr;
            break;
        }
    }

    std::vector<SampleFragment*> unsentFragments = change->getUnsentFragments();

    return unsentFragments;
}


bool ReaderProxy::checkSampleCompleteness(unsigned int sequenceNumber)
{
    // access change with the given sequence number
    ChangeForReader* change = nullptr;
    for (auto cfr: history)
    {
        if (cfr->sequenceNumber == sequenceNumber)
        {
            change = cfr;
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
        // remove sample from history? maybe just remove if expired or all readers completed reception of a given sample
        // done elsewhere
    }
    return complete;
}


bool ReaderProxy::checkForTimeout(unsigned int sequenceNumber)
{
    // access change with the given sequence number
    ChangeForReader* change = nullptr;
    for (auto cfr: history)
    {
        if (cfr->sequenceNumber == sequenceNumber)
        {
            change = cfr;
            break;
        }
    }

    if(change)
    {
        // timeout necessary if no unsent fragments remain but change has not been acknowledged in its entirety yet
        if(!(change->complete) && (change->unsentCount() == 0) && (change->ackCount() != change->numberFragments))
        {
            return true;
        }
    }
    return false;
}

void ReaderProxy::resetTimeoutedFragments(unsigned int sequenceNumber)
{
    // access change with the given sequence number
    ChangeForReader* change = nullptr;
    for (auto cfr: history)
    {
        if (cfr->sequenceNumber == sequenceNumber)
        {
            change = cfr;
            break;
        }
    }

    if(change && !(change->complete))
    {
        change->resetSentFragments();
    }
}
