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

// TODO code commentary

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
    InetAdapter() {}
    virtual ~InetAdapter();

  protected:
    /*
     * TODO Code commentary
     *
     * @param
     * @return
     */
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }

    /*
     * TODO Code commentary
     *
     * @param
     */
    virtual void initialize(int stage) override;

    /*
     * TODO Code commentary
     *
     * @param
     */
    virtual void handleMessageWhenUp(cMessage *msg) override;

    /*
     * TODO Code commentary
     */
    virtual void finish() override;

    /*
     * TODO Code commentary
     */
    virtual void refreshDisplay() const override;


    /*
     * TODO Code commentary
     *
     * @return
     */
    virtual L3Address chooseDestAddr();

    /*
     * TODO Code commentary
     *
     * @param
     */
    virtual void sendPacket();

    /*
     * TODO Code commentary
     *
     * @param
     */
    virtual void sendRtpsPacket(RtpsInetPacket *packet);

    /*
     * TODO Code commentary
     *
     * @param
     */
    virtual void processPacket(Packet *msg);

    /*
     * TODO Code commentary
     */
    virtual void setSocketOptions();


    /*
     * TODO Code commentary
     */
    virtual void processStart();

    /*
     * TODO Code commentary
     */
    virtual void processSend();

    /*
     * TODO Code commentary
     */
    virtual void processStop();


    /*
     * TODO Code commentary
     *
     * @param
     */
    virtual void handleStartOperation(LifecycleOperation *operation) override;

    /*
     * TODO Code commentary
     *
     * @param
     */
    virtual void handleStopOperation(LifecycleOperation *operation) override;

    /*
     * TODO Code commentary
     *
     * @param
     */
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

    /*
     * TODO Code commentary
     *
     * @param
     * @param
     */
    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;

    /*
     * TODO Code commentary
     *
     * @param
     * @param
     */
    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;

    /*
     * TODO Code commentary
     *
     * @param
     */
    virtual void socketClosed(UdpSocket *socket) override;
};

#endif //RTPS_ADAPTERS_INETADAPTER_H_
