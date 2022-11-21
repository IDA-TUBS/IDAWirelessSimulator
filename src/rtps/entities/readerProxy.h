/*
 *
 */

#ifndef RTPS_READERPROXY_H_
#define RTPS_READERPROXY_H_

#include <omnetpp.h>
#include <math.h>
#include <list>

#include "changeForReader.h"

#include "./../messages/RtpsInetPacket_m.h"

using namespace omnetpp;
using namespace inet;


class SampleFragment;

class ReaderProxy
{
  private:
    /// entity of corresponding reader
    unsigned int readerID;

    /// history story all samples
    std::list<ChangeForReader*> history;
    /// max size of history
    unsigned int historySize;

  public:
    /// the reader's priority
    unsigned int priority;

    /*
     * default constructor
     */
    ReaderProxy(unsigned int id, unsigned int historySize):
        readerID(id),
        historySize(historySize)
    {};

    /*
     * overloaded constructor, add first change to history
     */
    ReaderProxy(unsigned int id, unsigned int historySize, CacheChange &change):
        readerID(id),
        historySize(historySize)
    {
        this->addChange(change);
    };

    /*
     * default destructor
     */
    ~ReaderProxy()
    {
        history.clear();
    };

    /*
     * method for adding a new Cache Change to the proxy's history cache
     *
     * @param change reference to cache change that will be replicated in the reader proxy
     * @return true if change was added, false if cache was full and change was not added to history
     */
    bool addChange(CacheChange &change);

    /*
     * get readerID (entity ID) of corresponding reader, added for WiMEP protocol
     *
     * @return entity id
     */
    unsigned int getReaderId()
    {
        return readerID;
    }

    /*
     * set priority of corresponding reader, added for WiMEP protocol
     *
     * @param prio the priority assigned to the corresponding reader
     */
    void setPriority(unsigned int prio)
    {
        this->priority = prio;
    }

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
    bool processNack(RtpsInetPacket* nackFrag);

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

    /*
     * gather all fragments of a given change that are currently in state unsent (and not acknowledged!)
     *
     * @param sequenceNumber seq number of sample that shall be checked
     * @return list of fragments
     */
    std::vector<SampleFragment*> getUnsentFragments(unsigned int sequenceNumber);

};

#endif // RTPS_READERPROXY_H_
