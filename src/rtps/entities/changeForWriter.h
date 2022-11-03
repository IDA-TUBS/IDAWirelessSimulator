/*
 *
 */

#ifndef RTPS_CHANGEFORWRITER_H_
#define RTPS_CHANGEFORWRITER_H_

#include <omnetpp.h>
#include <math.h>

#include "cacheChange.h"
#include "sampleFragment.h"

using namespace omnetpp;


class SampleFragment;

class ChangeForWriter: CacheChange
{
  private:
    /// flag for signaling that all fragments have been acknowledged
    bool complete;

    /// storing fragment number that has been sent last
    int lastSentFN;
    /// highest yet sent fragment number
    int highestFNSend;

  public:
    /*
     * empty default constructor
     */
    ChangeForWriter()
    {
        complete = false;
        lastSentFN = -1;
        highestFNSend = 0;
    }

    /*
     * overloaded constructor
     *
     * @param id of reader
     * @param seqNum sequence number of the current sample
     * @param sampleSize size of the sample in bytes
     * @param fragmentSize size of a fragment in bytes
     * @param timestamp time when sample was generated/arrived at middleware
     */
    ChangeForWriter(unsigned int seqNum, unsigned int sampleSize, unsigned int fragmentSize, simtime_t timestamp):
        CacheChange(seqNum, sampleSize, fragmentSize, timestamp),
        complete(false),
        lastSentFN(-1),
        highestFNSend(-1)
    {
        sampleFragmentArray = new SampleFragment*[this->numberFragments];

        // instantiate all fragments comprising the sample
        for(unsigned int i = 0; i < this->numberFragments; i++){
            sampleFragmentArray[i] = new SampleFragment(this,
                                                        i,
                                                        (fragmentSize < sampleSize - (i*fragmentSize)) ? fragmentSize : sampleSize - (i*fragmentSize),
                                                        arrivalTime);
        }
    };

    /*
     * copy constructor
     */
    ChangeForWriter(ChangeForWriter &change):
        CacheChange(change.sequenceNumber, change.sampleSize, change.fragmentSize, change.arrivalTime),
        complete(change.complete),
        lastSentFN(change.lastSentFN),
        highestFNSend(change.highestFNSend)
    {
        sampleFragmentArray = new SampleFragment*[this->numberFragments];
        auto sampleArrayRef = change.getFragmentArray();

        // copy contents of reference array to this instance's array
        for(unsigned int i = 0; i < this->numberFragments; i++){
            sampleFragmentArray[i] = new SampleFragment(*sampleArrayRef[i]);
        }
    }

    /*
     * empty default destructor
     */
    ~ChangeForWriter()
    {
        delete sampleFragmentArray; // TODO necessary to delete all SampleFragments first?
    };

    /*
     * determine number of fragments that have been received so far
     *
     * @return number of fragments
     */
    unsigned int receivedCount();

    /*
     * method for updating the fragment status
     *
     * @param statues new fragment status
     * @param fragmentNumber fragment whose fragment shall be updated
     * @return true if operation successful, else returns false
     */
    virtual bool setFragmentStatus(fragmentStates status, unsigned int fragmentNumber);
};



#endif // RTPS_CHANGEFORWRITER_H_
