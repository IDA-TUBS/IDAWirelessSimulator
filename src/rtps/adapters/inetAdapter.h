#ifndef RTPS_ADAPTERS_INETADAPTER_H_
#define RTPS_ADAPTERS_INETADAPTER_H_

#include <string.h>
#include <omnetpp.h>

#include "inet/applications/base/ApplicationPacket_m.h"
#include "inet/common/INETDefs.h"
#include "inet/applications/base/ApplicationBase.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "./../messages/RtpsInetPacket_m.h"

using namespace omnetpp;
using namespace inet;


class InetAdapter :  public ApplicationBase, public UdpSocket::ICallback
{
  private:
    // ---- Internal parameters ----
    // ---- UdpBasicApp ----
    enum SelfMsgKinds { START = 1, SEND, STOP };

    // parameters
    std::vector<L3Address> destAddresses;
    std::vector<std::string> destAddressStr;
    int numberDestinations;
    int localPort = -1, destPort = -1;

    simtime_t startTime;
    simtime_t stopTime;
    bool dontFragment = false;
    const char *packetName = nullptr;

    std::vector<std::string> localAddr;

    /// state
    UdpSocket socket;
    cMessage *selfMsg = nullptr;

    int numSent = 0;

  public:
    /*
     * empty default constructor
     */
    InetAdapter() {}
    /*
     * empty default destructor
     */
    virtual ~InetAdapter();

  protected:
    /*
     * return the number of init stages
     *
     * @return number of init stages
     */
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }

    /*
     * initialize inet adapter, right now, only stage INITSTAGE_LOCAL supported
     *
     * @param stage unique stage identifier
     */
    virtual void initialize(int stage) override;

    /*
     * process any (self and incoming) message and call corresponding callbacks
     *
     * @param msg message to be handled
     */
    virtual void handleMessageWhenUp(cMessage *msg) override;

    /*
     * to be called at the end if not needed any more
     */
    virtual void finish() override;

    /*
     * refresh packet counter if qt GUI used for executing simulation
     */
    virtual void refreshDisplay() const override;


    /*
     * select a destination address for a given packet
     *
     * @return destintion address
     */
    virtual L3Address chooseDestAddr();

    /*
     * does not do anything right now
     */
    virtual void sendPacket();

    /*
     * pass RTPS Inet packet down to UDP layer
     *
     * @param packet that shall be send
     */
    virtual void sendRtpsPacket(RtpsInetPacket *packet);

    /*
     * visualizer for received packets
     *
     * @param msg
     */
    virtual void processPacket(Packet *msg);

    /*
     * configure socket options such as multicast addresses, broadcast, tos, ...
     */
    virtual void setSocketOptions();


    /*
     * called on start of inet adapter, further initialize destination addresses
     */
    virtual void processStart();

    /*
     * does not do anything right now
     */
    virtual void processSend();

    /*
     * called to close the socket
     */
    virtual void processStop();


    /*
     * set up event for start of inet adapter
     *
     * @param operation unused parameter
     */
    virtual void handleStartOperation(LifecycleOperation *operation) override;

    /*
     * called to stop inet adapter
     *
     * @param operation unused parameter
     */
    virtual void handleStopOperation(LifecycleOperation *operation) override;

    /*
     * called in case of some crash within the inet adapter
     *
     * @param operation unused parameter
     */
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

    /*
     * message callback // TODO actually used at all?
     *
     * @param socket unused parameter
     * @param packet the data that shall be processed
     */
    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;

    /*
     * return error indication
     *
     * @param socket unused parameter
     * @param indication
     */
    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;

    /*
     * additional method for closing socket
     *
     * @param socket unused parameter
     */
    virtual void socketClosed(UdpSocket *socket) override;
};

#endif //RTPS_ADAPTERS_INETADAPTER_H_
