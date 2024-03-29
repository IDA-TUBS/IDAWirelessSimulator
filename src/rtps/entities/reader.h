/*
 *
 */

#ifndef RTPS_ENTITIES_READER_H_
#define RTPS_ENTITIES_READER_H_

#include <omnetpp.h>
#include "./../messages/RtpsInetPacket_m.h"
#include "./../Rtps.h"
#include "./../analysis/analysis.h"

#include "endpoint.h"
#include "changeForWriter.h"
#include "writerProxy.h"
#include "sampleFragment.h"

using namespace omnetpp;
using namespace inet;

class Reader : public cSimpleModule, protected Endpoint, protected RTPSAnalysis
{
  public:
    /*
     * empty default constructor
     */
    Reader() {}

    /*
     * default destructor
     */
    ~Reader()
    {
        delete writerProxy;
    };

  protected:
    /// Sample latency deadline
    simtime_t deadline;

    /// user configurable reader response delay
    simtime_t responseDelay;

    /// HistoryCache
    WriterProxy *writerProxy;

    /// application ID
    unsigned int appID;

    /// the readers priority
    unsigned int priority;

    /// max number of cache changes to be stored simultaneously
    unsigned int sizeCache;

    /// if true, the efficiency is evaluated and stored
    bool measureEfficiency;

    /// vector storing destination address(es)
    std::vector<std::string> destinationAddresses;

    // ==============
    // ==== misc ====
    // ==============
    /// pointer to rtps parent instances, used for parameter retrieval
    Rtps* rtpsParent;

  protected:





    /*
     * Overwritten method, initializes modules after its instantiation
     */
    virtual void initialize() override;

    /*
     * Overwritten method, trigger either on arrival of a new sample (from app) or by self-messages
     *
     * @param message each incoming (external or self) message is interpreted as a stimulus
     */
    virtual void handleMessage(cMessage *message) override;

    /*
     * Callback for handling self messages
     *
     * @param message each incoming self message is interpreted as a stimulus
     */
    void handleInternalMessage(cMessage* message);

    /*
     * Callback for handling incoming external (RTPS) messages
     *
     * @param message each incoming external message is interpreted as a stimulus
     */
    void handleExternalMessage(cMessage* message);

    /*
     * process incoming data (DataFrag)
     *
     * @param rtpsMessage message containing the new fragment
     */
    void processDataFragment(RtpsInetPacket* rtpsMessage);

    /*
     * handle HeartbeatFrags accordingly by responding with NackFrag
     *
     * @param rtpsMessage message containing the HeartbeatFrag
     */
    void processHeartBeatFragment(RtpsInetPacket* rtpsMessage);

    /*
     * Method for evaluating whether a sample is still valid or whether its deadline elapsed.
     * Handles removing of sample in case of elapsed deadline
     */
    void checkSampleLiveliness();

    /*
     * Method that is triggered from transmitting NackFrags. At the end, sends a packaged sample fragment down towards the UDP/IP stack
     *
     * @return true on success, else false
     */
    void sendMessage(RtpsInetPacket* rtpsMessage);

    /*
    * generate a NackFrag message with bitmap based on a heartbeat message
    *
    * @param hb heartbeatmessage
    * @return NackFrag message
    */
    RtpsInetPacket* generateNackFrag(RtpsInetPacket* heartBeatMessage);


    /**
     *
     */
    virtual void finish() override;
};

#endif /* RTPS_ENTITIES_READER_H_ */
