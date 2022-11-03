/*
 *
 */

#ifndef RTPS_READERPROXY_H_
#define RTPS_READERPROXY_H_

#include <omnetpp.h>
#include <math.h>

#include "cacheChange.h"

#include "./../messages/RtpsInetPacket_m.h"

using namespace omnetpp;


class SampleFragment;

class ReaderProxy
{
  private:
    /// entity of corresponding reader
    unsigned int readerID;

    /// the reader's priority
    // unsigned int priority;

    /// history story all samples
    std::queue<ChangeForReader*> history;
    /// max size of history
    unsigned int historySize;

  public:
    /*
     * default constructor
     */
    ReaderProxy(unsigned int id):
        readerID(id)
    {};

    /*
     * overloaded constructor, add first change to history
     */
    ReaderProxy(unsigned int id, CacheChange &change):
        readerID(id)
    {
        this->addChange(change);
    };

    /*
     * empty default destructor
     */
    ~ReaderProxy();

    /*
     * method for adding a new Cache Change to the proxy's history cache
     *
     * @param change reference to cache change that will be replicated in the reader proxy
     */
    void addChange(CacheChange &change);

    /*
     * method for removing a Cache Change from the proxie's history cache
     *
     * @param sequenceNumber sequence number of the change that has be be removed
     */
    void removeChange(unsigned int sequenceNumber);

    /*
     * method for altering a fragment's status (unsent, sent, acked, ...)
     *
     * @param status fragment status
     * @param sequenceNumber seq number of the sample which fragment status shall be altered
     * @param fragmentNumber fn of fragment to be updated
     * @return true if successful, else false
     */
    bool updateFragmentStatus (fragmentStates status, unsigned int sequenceNumber, unsigned int fragmentNumber);

    /*
     * method for updating the fragment status based on NackFrag information
     *
     * @param nackFrag message containing the nackFrag
     * @return true if successful, else returns false
     */
    bool processNack(RtpsInetMessage* nackFrag);

    /*
     * method for updating the fragment status based on NackFrag information
     *
     * @param sequenceNumber seq number of sample that shall be checked
     * @return true if complete, else returns false
     */
    bool checkSampleCompleteness(unsigned int sequenceNumber);

    /*
     * method returning the current (oldest) cache change
     */
    ChangeForReader* getCurrentChange()
    {
        return history.front();
    }

};

#endif // RTPS_READERPROXY_H_
