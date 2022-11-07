/*
 *
 */

#include "readerProxy.h"


void ReaderProxy::addChange(CacheChange &change)
{
    // TODO what to do if max size exceeded?

    ChangeForReader* cfr = new ChangeForReader(this->readerID, change);

    history.push_back(cfr);
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
}

bool ReaderProxy::checkSampleCompleteness(unsigned int sequenceNumber)
{
    // TODO implement
}
