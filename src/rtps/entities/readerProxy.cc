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
            history.erase(it);
            return;
        }
        else
        {
            ++it;
        }
    }
}


bool ReaderProxy::updateFragmentStatus (fragmentStates status, unsigned int sequenceNumber, unsigned int fragmentNumber)
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

    return tmp->setFragmentStatus(status, fragmentNumber);
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
