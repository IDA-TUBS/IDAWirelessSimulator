/*
 *
 */

#ifndef RTPS_ENTITIES_WRITERMULTICAST_H__
#define RTPS_ENTITIES_WRITERMULTICAST_H__

#include <omnetpp.h>
#include "./../../messages/Timeout_m.h"

#include "../writer.h"

using namespace omnetpp;
using namespace inet;

typedef enum
{
    NONE,
    FIXED,
    ADAPTIVE_LOW_PDR,
    ADAPTIVE_HIGH_PDR
} PrioritizationMode;

class WriterW2RP : public Writer
{
  public:
    /*
     * empty default constructor
     */
    WriterW2RP()
    {
        Writer();
    };

    /*
     * default destructor
     */
    ~WriterW2RP()
    {
        matchedReaders.clear();
        historyCache.clear();
        sendQueue.clear();
    };

  protected:
    // ==============================
    // ==== rtps parametrization ====
    // ==============================

    // same as Writer
    /// enabling and disabling of prioritization mechanism
    bool prioritized;
    /// priotization mode
    PrioritizationMode prioMode;
    /// timeout duration
    simtime_t timeout;

    // ===========================
    // ==== protocol entities ====
    // ===========================

    // same as Writer

    // =======================
    // ==== self messages ====
    // =======================
    // same as Writer
    /// event used for timeouts
    cMessage *timeoutEvent;

    // ==============
    // ==== misc ====
    // ==============

    // same as Writer
    /// Counter for active timeouts
    unsigned int activeTimeouts;

  protected:

    /*
     * Method that schedules the timeout based on the earliest sent but unacknowledged fragment
     *
     * @return true on success, else false
     */
    void scheduleTimeout();


    /*
     * Method that is triggered according to some schedule. At the end, sends a packaged sample fragment down towards the UDP/IP stack
     *
     * @return true on success, else false
     */
    bool sendMessage() override;

    /*
     * Overwritten method, trigger either on arrival of a new sample (from app) or by self-messages
     *
     * @param msg each incoming (external or self) message is interpreted as a stimulus
     */
    virtual void handleMessage(cMessage *msg) override;

    /*
     * Handle some writer configuration that would normally be done during discovery
     */
    virtual void handleDiscovery() override;

    /*
     * Method for reacting to NackFrags received from readers as defined by WiMEP
     *
     * @param nackFrag message containing the ack/nack bitmap
     */
    void handleNackFrag(RtpsInetPacket* nackFrag) override;

    /*
     * Method for selecting a reader for the next transmission
     *
     * @return reader proxy
     */
    virtual ReaderProxy* selectReader() override;

    /*
     * Method for selecting which fragment (missing at a specific reader) to transmit next
     *
     * @param rp pointer to reader proxy used for fragment selection
     * @return the sample fragment to be transmitted next
     */
    virtual SampleFragment* selectNextFragment(ReaderProxy *rp) override;

    /*
     * Method for enabling or disabling scheduling of new events in case of an empty send queue
     * here: do not stop timer (scheduling of new fragments if there is a timeout active)+
     *
     * @return true if timer shall be stopped, false if new event shall be scheduled anyway
     */
    virtual bool stopScheduledTimer() override;

    /*
     * Overwritten method, initializes modules after its instantiation
     */
    virtual void initialize() override;

    /*
     * Overwritten method, called at the end of the simulation
     */
    virtual void finish() override;

    /*
     * Method for priming the send queue with each fragment that needs to be transmitted.
     * Used in WiMEP's transmissions phase and ensure that no retransmissions will be
     * performed prior to each fragment being transmitted once. Always called if a new
     * sample has to be transmitted
     *
     * @param sequenceNumber of the corresponding sample that shall be transmitted next
     */
    void fillSendQueueWithSample(unsigned int sequenceNumber);
};

#endif //RTPS_ENTITIES_WRITERMULTICAST_H__
