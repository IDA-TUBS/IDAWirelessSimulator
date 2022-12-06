/*
 *
 */

#ifndef RTPS_WRITERPROXY_H_
#define RTPS_WRITERPROXY_H_

#include <omnetpp.h>
#include <math.h>
#include <list>

#include "changeForWriter.h"

#include "./../messages/RtpsInetPacket_m.h"

using namespace omnetpp;
using namespace inet;

class SampleFragment;

class WriterProxy
{
  private:
    /// history story all samples
    std::list<ChangeForWriter*> history;
    /// latest sample number;
    int highestSequenceNumber;

    /// max size of history
    unsigned int historySize;

  public:
    /*
     * default constructor
     */
    WriterProxy(unsigned int historySize):
        historySize(historySize),
        highestSequenceNumber(-1)
    {};

    /*
     * overloaded constructor, add first change to history
     */
    WriterProxy(unsigned int historySize, CacheChange &change):
        historySize(historySize)
    {
        this->addChange(change);
    };

    /*
     * default destructor
     */
    ~WriterProxy()
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
     * method for evaluating completeness of samples
     *
     * @param sequenceNumber seq number of sample that shall be checked
     * @return true if complete, else returns false
     */
    bool checkSampleCompleteness(unsigned int sequenceNumber);

    /*
     * method returning the current (oldest) cache change
     *
     * @return ChangeForWriter object
     */
    ChangeForWriter* getCurrentChange()
    {
        return history.front();
    };

    /*
     * method returning the latest cache change
     *
     * @return ChangeForWriter object
     */
    ChangeForWriter* getLatestChange()
    {
        return history.back();
    };

    /*
     * Method for returning history size
     */
    unsigned int getSize()
    {
        return history.size();
    };

    /*
     * method cache change with corresponding sequence number
     *
     * @param sequenceNumber seq number of sample that shall be retrieved
     * @return ChangeForWriter object
     */
    ChangeForWriter* getChange(unsigned int sequenceNumber)
    {
        // first find change corresponding to the given sequence number
        ChangeForWriter* tmp = nullptr;
        for (auto cfw: history)
        {
            if (cfw->sequenceNumber == sequenceNumber)
            {
                tmp = cfw;
                break;
            }
        }

        return tmp;
    };
};

#endif // RTPS_WRITERPROXY_H_
