/*
 *
 */

#ifndef RTPS_ENTITIES_WRITER_H__
#define RTPS_ENTITIES_WRITER_H__

#include <omnetpp.h>
#include "./../messages/RtpsInetPacket_m.h"
#include "./../messages/Sample_m.h"
#include "./../Rtps.h"

#include "endpoint.h"
#include "changeForReader.h"
#include "readerProxy.h"
#include "sampleFragment.h"

#include <string.h>

using namespace omnetpp;
using namespace inet;

class Writer : public cSimpleModule, protected Endpoint
{
  public:
    /*
     * empty default constructor
     */
    Writer() {};

    /*
     * default destructor
     */
    ~Writer()
    {
        for(auto &rp: matchedReaders)
        {
            delete[] rp;
        }
        matchedReaders.clear();

        for(auto &cc: historyCache)
        {
            delete[] cc;
        }
        historyCache.clear();

        for(auto &sf: sendQueue)
        {
            delete[] sf;
        }
        sendQueue.clear();
    };

  protected:
    // ==============================
    // ==== rtps parametrization ====
    // ==============================

    /// Fragment size
    unsigned int fragmentSize;
    /// Sample latency deadline
    simtime_t deadline;
    /// max number of cache changes to be stored simultaneously
    unsigned int sizeCache;

    /// number of subscribers/readers
    unsigned int numReaders;

    /// shaping parameter - value given in us
    simtime_t shaping;
    /// heartbeat period
    simtime_t hbPeriod;
    /// nack suppression duration // TODO implement properly
    simtime_t nackSuppressionDuration;

    /// application ID
    unsigned int appID;
    /// vector storing destination address(es)
    std::vector<std::string> destinationAddresses;

    // ===========================
    // ==== protocol entities ====
    // ===========================

    /// storing the "actual" data that is transmitted via RTPS
    std::list<CacheChange*> historyCache;
    /// the reader proxies keep track of sending/acknowledgment states for each reader
    std::vector<ReaderProxy*> matchedReaders;
    /// list of fragments to send next
    std::list<SampleFragment*> sendQueue;


    // =======================
    // ==== self messages ====
    // =======================
    /// self message used for repetitive scheduling of fragment transmission
    cMessage *sendEvent;
    /// self message used for periodic scheduling of heartbeat messages
    cMessage *hbTimer;
    /// self message used for triggering timeouts
    cMessage *nextTimeoutEvent;

    // ==============
    // ==== misc ====
    // ==============
    /// pointer to rtps parent instances, used for parameter retrieval
    Rtps* rtpsParent;


  protected:
    /*
     * Method that is triggered according to some schedule. At the end, sends a packaged sample fragment down towards the UDP/IP stack
     *
     * @return true on success, else false
     */
    virtual bool sendMessage();

    /*
     * Method that is used for "default" RTPS implementation to mimic only sending a heartbeart message - without any data attached
     *
     * @return true on success, else false
     */
    bool sendHeartbeatMsg();

    /*
     * Method for reacting to NackFrags received from readers
     *
     * @param nackFrag message containing the ack/nack bitmap
     */
    void handleNackFrag(RtpsInetPacket* nackFrag);

    /*
     * Method for creating new cache change on arrival of new sample
     *
     * @param sample containing all information for building CacheChange
     */
    bool addSampleToCache(Sample* sample);

    /*
     * Method for evaluating whether a sample is still valid or whether its deadline elapsed.
     * Handles removing of sample in case of elapsed deadline
     */
    void checkSampleLiveliness();

    /*
     * Method for evaluating whether a reader has received all fragments of a sample
     *
     * @param rp pointer to ReaderProxy corresponding to the reader whose status of the current sample shall be checked
     * @return true if complete, else false
     */
    bool checkSampleCompletenessAtReader(ReaderProxy* rp);

    /*
     * Method for selecting a reader for the next transmission
     *
     * @return reader proxy
     */
    virtual ReaderProxy* selectReader();

    /*
     * Method for selecting which fragment (missing at a specific reader) to transmit next
     *
     * @param rp pointer to reader proxy used for fragment selection
     * @return the sample fragment to be transmitted next
     */
    virtual SampleFragment* selectNextFragment(ReaderProxy *rp);


    /*
     * Overwritten method, initializes modules after its instantiation
     */
    virtual void initialize() override;

    /*
     * Overwritten method, trigger either on arrival of a new sample (from app) or by self-messages
     *
     * @param msg each incoming (external or self) message is interpreted as a stimulus
     */

    virtual void handleMessage(cMessage *msg) override;
    /*
     * Overwritten method, called at the end of the simulation
     */

    virtual void finish() override;
};

#endif //RTPS_ENTITIES_WRITER_H__
