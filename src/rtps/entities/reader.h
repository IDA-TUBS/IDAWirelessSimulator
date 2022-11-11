/*
 *
 */

#ifndef RTPS_ENTITIES_READER_H_1
#define RTPS_ENTITIES_READER_H_1

#include <omnetpp.h>
#include "./../messages/RtpsInetPacket_m.h"
#include "./../Rtps.h"

#include "endpoint.h"
#include "changeForWriter.h"
#include "writerProxy.h"
#include "sampleFragment.h"

using namespace omnetpp;
using namespace inet;

class Reader : public cSimpleModule, Endpoint
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
        delete[] writerProxy;
    };

  private:
    /// user configurable reader response delay
    simtime_t responseDelay;

    /// HistoryCache
    WriterProxy *writerProxy;

    /// application ID
    unsigned int appID;

    /// max number of cache changes to be stored simultaneously
    unsigned int sizeCache;

    /// vector storing destination address(es)
    std::vector<std::string> destinationAddresses;

    // ==============
    // ==== misc ====
    // ==============
    /// pointer to rtps parent instances, used for parameter retrieval
    Rtps* rtpsParent;

  protected:
    /*
     * Method that is triggered from transmitting NackFrags. At the end, sends a packaged sample fragment down towards the UDP/IP stack
     *
     * @return true on success, else false
     */
    void sendMessage(RtpsInetPacket* msg);

    /*
     * generate a NackFrag message with bitmap based on a heartbeat message
     *
     * @param hb heartbearmessage
     * @return NackFrag message
     */
    RtpsInetPacket* generateNackFrag(RtpsInetPacket* hb);


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


#endif /* RTPS_ENTITIES_READER_H_1 */
