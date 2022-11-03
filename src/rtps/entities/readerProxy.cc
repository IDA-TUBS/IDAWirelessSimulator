/*
 *
 */

#include "readerProxy.h"


void ReaderProxy::addChange(CacheChange &change)
{
    ChangeForReader* cfr = new ChangeForReader(this->readerID, change);

    history.push(cfr);
}

bool ReaderProxy::updateFragmentStatus (fragmentStates status, unsigned int sequenceNumber, unsigned int fragmentNumber)
{
    // first find change corresponding to the given sequence number
    ChangeForReader* tmp = nullptr;
    for (auto cfr: history)
    {
        if (cfr->getSequenceNumber() == sequenceNumber)
        {
            tmp = cfr;
            break;
        }
    }

    return cfr.setFragmentStatus(status, fragmentNumber);
}

bool ReaderProxy::processNack(RtpsInetMessage* nackFrag)
{
    //first parse data
    fragmentStates status;
    unsigned int sequenceNumber = nackFrag->getWriterSN();


    // Iterate through the addressed bitmap region and update the reader's history cache
    for(int i = nackFrag->getFragmentNumberStateBase(); i < nackFrag->getFragmentNumberStateBase() + nackFrag->getFragmentNumberStateNbrBits();i++)
    {
       unsigned int fragmentNumber = i;
       bool acked = nackFrag->getFragmentNumberBitmap(i-nackFrag->getFragmentNumberStateBase());

        if(current_fragment->acked){ // TODO
            continue;
        }

        // then update fragment status accordingly
        if(acked == true){
            this->updateFragmentStatus(ACKED, sequenceNumber, i);
            continue;
        }

        if(current_fragment->send){ // TODO
            this->updateFragmentStatus(NACKED, sequenceNumber, i);
        }
    }
}

bool ReaderProxy::checkSampleCompleteness(unsigned int sequenceNumber)
{
    // TODO implement
}
