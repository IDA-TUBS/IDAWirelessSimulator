/*
 *
 */

#ifndef RTPS_CHANGEFORREADER_H_
#define RTPS_CHANGEFORREADER_H_

#include <omnetpp.h>
#include <math.h>

#include "cacheChange.h"
#include "sampleFragment.h"

using namespace omnetpp;


class SampleFragment;

class ChangeForReader: public CacheChange
{
  private:
    /// reader entity ID
    unsigned int readerID;

  public:
    /// flag for signaling that all fragments have been acknowledged
    bool complete;

    /// storing fragment number that has been sent last
    int lastSentFN;
    /// highest yet sent fragment number
    int highestFNSend;

    /// reference (pointer) to intial CacheChange
    CacheChange *baseChange;

    /*
     * empty default constructor
     */
    ChangeForReader(){
        complete = false;
        lastSentFN = -1;
        highestFNSend = 0;
    }

    /*
     * overloaded constructor
     * // TODO add reference to cache change?
     *
     * @param id of reader
     * @param seqNum sequence number of the current sample
     * @param sampleSize size of the sample in bytes
     * @param fragmentSize size of a fragment in bytes
     * @param timestamp time when sample was generated/arrived at middleware
     */
    ChangeForReader(CacheChange* change, unsigned int id, unsigned int seqNum, unsigned int sampleSize, unsigned int fragmentSize, simtime_t timestamp):
        CacheChange(seqNum, sampleSize, fragmentSize, timestamp),
        readerID(id),
        complete(false),
        lastSentFN(-1),
        highestFNSend(-1),
        baseChange(change)
    {
        sampleFragmentArray = new SampleFragment*[this->numberFragments];

        // instantiate all fragments comprising the sample
        for(unsigned int i = 0; i < this->numberFragments; i++){
            sampleFragmentArray[i] = new SampleFragment(baseChange,
                                                        i,
                                                        (fragmentSize < sampleSize - (i*fragmentSize)) ? fragmentSize : sampleSize - (i*fragmentSize),
                                                        arrivalTime);
        }
    };

    /*
     * overloaded constructor, generate ChangeForReader from CacheChange
     *
     * @param id of reader
     * @param change reference to cache change
     */
    ChangeForReader(unsigned int id, CacheChange &change):
        CacheChange(change.sequenceNumber, change.sampleSize, change.fragmentSize, change.arrivalTime),
        readerID(id),
        complete(false),
        lastSentFN(-1),
        highestFNSend(-1),
        baseChange(baseChange)
    {
        sampleFragmentArray = new SampleFragment*[this->numberFragments];

        auto sampleArrayRef = change.getFragmentArray();

        // copy contents of reference array (CacheChange) to this instance's array
        for(unsigned int i = 0; i < this->numberFragments; i++){
            sampleFragmentArray[i] = new SampleFragment(*sampleArrayRef[i]);
        }
    };

    /*
     * copy constructor
     */
    ChangeForReader(ChangeForReader &change):
        CacheChange(change.sequenceNumber, change.sampleSize, change.fragmentSize, change.arrivalTime),
        readerID(change.readerID),
        complete(change.complete),
        lastSentFN(change.lastSentFN),
        highestFNSend(change.highestFNSend),
        baseChange(change.baseChange)
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
    ~ChangeForReader()
    {
        delete[] sampleFragmentArray;
    };

    /*
     * determine number of fragments that have not been acknowledged yet
     *
     * @return number of fragments
     */
    unsigned int notAckCount();

    /*
     * determine number of fragments that have been acknowledged so far
     *
     * @return number of fragments
     */
    unsigned int ackCount();

    /*
     * determine number of fragments that have been sent so far, also includes those already acknowledged by the reader
     *
     * @return number of fragments
     */
    unsigned int sentCount();

    /*
     * method for updating the fragment status
     *
     * @param statues new fragment status
     * @param fragmentNumber fragment whose fragment shall be updated
     * @return true if operation successful, else returns false
     */
    virtual bool setFragmentStatus(fragmentStates status, unsigned int fragmentNumber);

};



#endif // RTPS_CHANGEFORREADER_H_
