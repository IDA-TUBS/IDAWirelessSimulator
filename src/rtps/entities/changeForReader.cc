/*
 *
 */

#include "changeForReader.h"
#include "sampleFragment.h"


unsigned int ChangeForReader::notAckCount()
{
    // count negatively acknowledged fragments and fragments that timed out
    // does not count fragments that have been sent but not acknowledged yet!
    unsigned int notAcked = 0;
    for(unsigned int i = 0; i < numberFragments; i++)
    {
        auto frag = sampleFragmentArray[i];
        if(!(frag->acked) && !(frag->sent))
        { //|| (frag->timeout && !(frag->acked))
            notAcked++;
        }
    }
    return notAcked;
}

unsigned int ChangeForReader::ackCount()
{
    // count acknowledged fragments
    unsigned int acked = 0;
    for(unsigned int i = 0; i < numberFragments; i++)
    {
        auto frag = sampleFragmentArray[i];
        if(frag->acked)
        {
            acked++;
        }
    }
    return acked;
}

unsigned int ChangeForReader::sentCount()
{
    // count sent fragments, includes already acked fragments as well!
    unsigned int sent = 0;
    for(unsigned int i = 0; i < numberFragments; i++)
    {
        auto frag = sampleFragmentArray[i];
        if(frag->sent || frag->acked)
        {
            sent++;
        }
    }
    return sent;
}

bool ChangeForReader::setFragmentStatus(fragmentStates status, unsigned int fragmentNumber)
{
    auto frag = sampleFragmentArray[fragmentNumber];

    switch(status)
    {
        case UNSENT:
            frag->sent = false;
            frag->acked = false;
            break;
        case SENT:
            lastSentFN = fragmentNumber;
            frag->sent = true;
            frag->acked = false;
            if(fragmentNumber > highestFNSend)
            {
                highestFNSend = fragmentNumber;
            }
            break;
        case ACKED:
            frag->sent = true;
            frag->acked = true;
            break;
        case NACKED:
            frag->sent = false;
            frag->acked = false;
            break;
        default:
            break;
    }

    return true;
}

std::vector<SampleFragment*> ChangeForReader::getUnsentFragments()
{
    std::vector<SampleFragment*> unsentFragments;

    for(unsigned int i = 0; i < numberFragments; i++)
    {
        auto frag = sampleFragmentArray[i];
        if(!(frag->sent) && !(frag->acked))
        {
            unsentFragments.push_back(frag);
        }
    }
    return unsentFragments;
}
