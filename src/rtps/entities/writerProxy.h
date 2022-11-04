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
    /// max size of history
    unsigned int historySize;

  public:
    /*
     * default constructor
     */
    WriterProxy(unsigned int historySize):
        historySize(historySize)
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
        for(auto &cfw: history)
        {
            delete[] cfw;
        }
        history.clear();
    };

    void addChange(CacheChange &change);

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
     * @param sequenceNumber seq number of sample that shall be checked
     * @return true if complete, else returns false
     */
    bool checkSampleCompleteness(unsigned int sequenceNumber);

};

#endif // RTPS_WRITERPROXY_H_
