/*
 *
 */

#include "changeForWriter.h"
#include "sampleFragment.h"


unsigned int ChangeForWriter::receivedCount()
{
    // count received fragments
    unsigned int received = 0;
    for(unsigned int i = 0; i < numberFragments; i++){
        auto frag = sampleFragmentArray[i];
        if(frag->received) {
            received++;
        }
    }
    return received;
};

bool ChangeForWriter::setFragmentStatus(fragmentStates status, unsigned int fragmentNumber)
{
    auto frag = sampleFragmentArray[fragmentNumber];

    switch(status)
    {
        case RECEIVED:
            frag->received = true;
            break;
        default:
            break;
    }

    return true;
}
