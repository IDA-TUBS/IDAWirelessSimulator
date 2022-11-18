/*
 *
 */

#ifndef RTPS_ENTITIES_WRITERMULTICAST_H__
#define RTPS_ENTITIES_WRITERMULTICAST_H__

#include <omnetpp.h>

#include "writer.h"

using namespace omnetpp;
using namespace inet;

class WriterWiMEP : public Writer
{
  public:
    /*
     * empty default constructor
     */
    WriterWiMEP()
    {
        Writer();
    };

    /*
     * default destructor
     */
    ~WriterWiMEP()
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

    // ===========================
    // ==== protocol entities ====
    // ===========================

    // same as Writer

    // =======================
    // ==== self messages ====
    // =======================

    // same as Writer

    // ==============
    // ==== misc ====
    // ==============

    // same as Writer


  protected:
    /*
     * Method that is triggered according to some schedule. At the end, sends a packaged sample fragment down towards the UDP/IP stack
     *
     * @return true on success, else false
     */
    bool sendMessage() override;

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
