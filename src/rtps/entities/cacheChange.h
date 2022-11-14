/*
 *
 */

#ifndef RTPS_CACHECHANGE_H_
#define RTPS_CACHECHANGE_H_

#include <omnetpp.h>
#include <math.h>

#include "sampleFragment.h"

using namespace omnetpp;

enum fragmentStates{
    UNSENT,
    SENT,
    ACKED,
    NACKED,
    RECEIVED
};

class SampleFragment;

class CacheChange
{
  public:
    /// sequence number of current sample
    unsigned int sequenceNumber;
    /// size of the sample in bytes
    unsigned int sampleSize;
    /// size of each fragment
    unsigned int fragmentSize;
    /// total number of fragments per sample
    unsigned int numberFragments;

    /// timestamp: when did the sample arrive at the middleware
    simtime_t arrivalTime;

    /// array storing the 'data'
    SampleFragment** sampleFragmentArray;

    /*
     * empty default constructor
     */
    CacheChange() {};

    /*
     * overloaded constructor
     *
     * @param seqNum sequence number of the current sample
     * @param sampleSize size of the sample in bytes
     * @param fragmentSize size of a fragment in bytes
     * @param timestamp time when sample was generated/arrived at middleware
     */
    CacheChange(unsigned int seqNum, unsigned int sampleSize, unsigned int fragmentSize, simtime_t timestamp):
        sequenceNumber(seqNum),
        sampleSize(sampleSize),
        fragmentSize(fragmentSize),
        numberFragments(int(ceil(sampleSize / fragmentSize))),
        arrivalTime(timestamp)
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
    CacheChange(CacheChange &change):
        sequenceNumber(change.sequenceNumber),
        sampleSize(change.sampleSize),
        fragmentSize(change.fragmentSize),
        numberFragments(change.numberFragments),
        arrivalTime(change.arrivalTime)
    {
        auto sampleArrayRef = change.getFragmentArray();

        // copy contents of reference array (CacheChange) to this instance's array
        for(unsigned int i = 0; i < this->numberFragments; i++){
            sampleFragmentArray[i] = new SampleFragment(*sampleArrayRef[i]);
        }
    }

    /*
     * empty default destructor
     */
    ~CacheChange()
    {
        delete[] sampleFragmentArray;
    };

    /*
     * method for altering a fragment's status (unsent, sent, acked, ...)
     *
     * @param status fragment status
     * @param fragmentNumber fn of fragment to be updated
     * @return true if successful, else false
     */
    bool setFragmentStatus (fragmentStates status, unsigned int fragmentNumber)
    {
        // actual implementations found in ChangeForReader/Writer
        return false;
    };


    /*
     * return the fragment array
     *
     * @return array of sample fragments
     */
    SampleFragment **getFragmentArray()
    {
        return this->sampleFragmentArray;
    }


    /*
     * determine whether age has been exceeded
     *
     * @param deadline sample deadline
     * @return true if sample still valid, else returns false
     */
    bool isValid(simtime_t deadline)
    {
        return ((simTime() - this->arrivalTime) <= deadline);
    }

    /*
     * check whether sample has been received or acknowledged in its entirety
     *
     * @return true if complete, else returns false
     */
    bool checkForCompleteness()
    {
       bool complete = true;
       for(int i = 0; i < this->numberFragments; i++){
           SampleFragment* fragment = this->sampleFragmentArray[i];
           if(!fragment->acked && !fragment->received){
               return false;
           }
           EV << "fragment "  << i << "  received\n";
       }
       return complete;
    };

};



#endif // RTPS_CACHECHANGE_H_
