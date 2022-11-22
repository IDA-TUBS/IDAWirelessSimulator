/*
 *
 */

#ifndef RTPS_ENTITIES_SAMPLEFRAGMENT_H_
#define RTPS_ENTITIES_SAMPLEFRAGMENT_H_

#include <omnetpp.h>

using namespace omnetpp;

class CacheChange;

class SampleFragment
{
  public:
    /// fragment number
    unsigned int fragmentStartingNum;
    /// counting how often the given fragments has been transmitted
    unsigned int sendCounter;
    /// actual fragment size (can be smaller than fragmentSize)
    unsigned int dataSize;

    /// reference (pointer) to ChangeForReader owning this object
    CacheChange* baseChange;

    /// timestamp the fragment has been sent (the latest timestamp if already sent multiple times)
    simtime_t sendTime;

    /// flag fragment as sent - relevant for writer
    bool sent;
    /// flag fragment as acknowledged - relevant for writer
    bool acked;
    /// flag fragment as received - relevant for reader
    bool received;


  public:
    /*
     * empty default constructor
     */
    SampleFragment() {};

    /*
     * overloaded constructor
     *
     * @param baseChange reference (pointer) to Change, the fragment is associated to
     * @param fragStartNum fragment number
     * @param dataSize size of the fragment in bytes
     * @param sendTime time when sample first arrived at middleware
     */
    SampleFragment(CacheChange *baseChange, unsigned int fragStartNum, unsigned int dataSize, simtime_t sendTime):
        fragmentStartingNum(fragStartNum),
        dataSize(dataSize),
        sendTime(sendTime),
        // Relevant for the Writer
        sent(false),
        acked(false),
        // Relevant for the Reader
        received(false),
        baseChange(baseChange)
    {};

    /*
     * copy constructor
     *
     * @param sf reference to object to be copied
     */
    SampleFragment(SampleFragment &sf):
        fragmentStartingNum(sf.fragmentStartingNum),
        dataSize(sf.dataSize),
        sendTime(sf.sendTime),
        sent(sf.sent),
        acked(sf.acked),
        received(sf.received),
        baseChange(sf.baseChange)
    {};

    /*
     * empty default destructor
     */
    ~SampleFragment() {};

    /*
     * set status of flag 'sent'
     *
     * @param b set 'sent' to this boolean value
     */
    void setSent(bool b)
    {
        if(!(this->acked))
        {
            this->sent = b;

            this->sendTime = simTime();
            sendCounter++;
        }
    };

    /*
     * set status of flag 'acked'
     *
     * @param b set 'acked' to this boolean value
     */
    void setAcked(bool b)
    {
        this->acked = b;
    };

    /*
     * set status of flag 'received'
     *
     * @param b set 'received' to this boolean value
     */
    void setReceived(bool b)
    {
        this->received = b;
    };
};



#endif // RTPS_ENTITIES_SAMPLEFRAGMENT_H_
