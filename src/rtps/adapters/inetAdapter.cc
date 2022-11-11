#include <omnetpp.h>

#include "inetAdapter.h"

#include "inet/applications/base/ApplicationPacket_m.h"
#include "inet/applications/udpapp/UdpBasicBurst.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/common/FragmentationTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"

#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/applications/base/ApplicationPacket_m.h"

#include "inet/common/packet/Packet_m.h"
#include "./../messages/RtpsInetPacket_m.h"

#include <string.h>

using namespace omnetpp;
using namespace inet;

Define_Module(InetAdapter);

InetAdapter::~InetAdapter()
{
    cancelAndDelete(selfMsg);
}

void InetAdapter::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        localPort = par("localPort");
        destPort = par("destPort");
        startTime = par("startTime");
        stopTime = par("stopTime");
        packetName = par("packetName");
        dontFragment = par("dontFragment");
        numberDestinations = par("numberDestinations");
        if (stopTime >= SIMTIME_ZERO && stopTime < startTime)
        {
            throw cRuntimeError("Invalid startTime/stopTime parameters");
        }
        selfMsg = new cMessage("sendTimer");
    }
}

void InetAdapter::handleMessageWhenUp(cMessage *msg)
{
    if (dynamic_cast<RtpsInetPacket*>(msg))
    {
        cGate *inGate = msg->getArrivalGate();
        if(inGate->getName() == gate("updSocketIn")->getName())
        {
            send(msg, gate("dispatcherOut"));
        }
        else if(inGate->getName() == gate("dispatcherIn")->getName())
        {
            sendRtpsPacket(check_and_cast<RtpsInetPacket*>(msg));
        }
        return;
    }

    if (msg->isSelfMessage())
    {
        ASSERT(msg == selfMsg);
        switch (selfMsg->getKind()) {
            case START:
                processStart();
                break;
            case SEND:
                processSend();
                break;
            case STOP:
                processStop();
                break;
            default:
                throw cRuntimeError("Invalid kind %d in self message", (int)selfMsg->getKind());
        }
    }
    else
    {
        socket.processMessage(msg); //legacy?? // TODO remove?
    }
}

void InetAdapter::sendRtpsPacket(RtpsInetPacket *packet)
{
    packet->addTag<FragmentationReq>()->setDontFragment(true);
    const auto& payload = makeShared<ApplicationPacket>();
    payload->setChunkLength(B(packet->getRtpsMessageSize()));
    payload->setSequenceNumber(numSent);
    payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
    packet->insertAtBack(payload);

    // take address from RtpsInetPacket
    const char *addr = packet->getDestinationAddress();

    L3Address destAddr;
    L3AddressResolver().tryResolve(addr, destAddr);

    emit(packetSentSignal, packet);
    socket.sendTo(packet, destAddr, destPort);
    numSent++;
}

void InetAdapter::finish()
{
    ApplicationBase::finish();
}

void InetAdapter::setSocketOptions()
{
    int timeToLive = par("timeToLive");
    if (timeToLive != -1)
    {
        socket.setTimeToLive(timeToLive);
    }

    int dscp = par("dscp");
    if (dscp != -1)
    {
        socket.setDscp(dscp);
    }

    int tos = par("tos");
    if (tos != -1)
    {
        socket.setTos(tos);
    }

    const char *multicastInterface = par("multicastInterface");
    if (multicastInterface[0])
    {
        IInterfaceTable *ift = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);
        NetworkInterface *ie = ift->findInterfaceByName(multicastInterface);
        if (!ie)
        {
            throw cRuntimeError("Wrong multicastInterface setting: no interface named \"%s\"", multicastInterface);
        }
        socket.setMulticastOutputInterface(ie->getInterfaceId());
    }

    bool receiveBroadcast = par("receiveBroadcast");
    if (receiveBroadcast)
    {
        socket.setBroadcast(true);
    }

    bool joinLocalMulticastGroups = par("joinLocalMulticastGroups");
    if (joinLocalMulticastGroups)
    {
        MulticastGroupList mgl = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this)->collectMulticastGroups();
        socket.joinLocalMulticastGroups(mgl);
    }
    socket.setCallback(this);
}

L3Address InetAdapter::chooseDestAddr()
{
    int k = intrand(destAddresses.size());
    if(destAddresses[k].isUnspecified() || destAddresses[k].isLinkLocal())
    {
        L3AddressResolver().tryResolve(destAddresses[k].str().c_str(), destAddresses[k]);
    }
    return destAddresses[k];
}

void InetAdapter::sendPacket()
{
    return;
}

void InetAdapter::processStart()
{
    socket.setOutputGate(gate("udpSocketOut"));
    const char *localAddress = par("localAddress");

    // determine unique addresses based on name of parent module
    if(strlen(localAddress) == 0){
        // used for assigning unique and correct local addresses to nodes part of an array of hosts in the network!
        const char *tmp = ((this->getParentModule())->getParentModule())->getFullName();
        *(&localAddress) = *(&tmp);
    }
    socket.bind(*localAddress ? L3AddressResolver().resolve(localAddress) : L3Address(), localPort);
    setSocketOptions();

    const char *destAddrs = par("destAddresses");
    cStringTokenizer tokenizer(destAddrs);
    const char *token;

    int i = 0;
    while ((token = tokenizer.nextToken()) != nullptr) {
        if(numberDestinations != -1){
            if (i >= numberDestinations) {
                continue;
            } else {
                i++;
            }
        }
        destAddressStr.push_back(token);
        L3Address result;
        L3AddressResolver().tryResolve(token, result);
        if (result.isUnspecified())
            EV_ERROR << "cannot resolve destination address: " << token << endl;
        destAddresses.push_back(result);
    }

    if (!destAddresses.empty()) {
        selfMsg->setKind(SEND);
        processSend();
    }
    else {
        EV<<"destAddress is empty\n";
        if (stopTime >= SIMTIME_ZERO) {
            selfMsg->setKind(STOP);
            scheduleAt(stopTime, selfMsg);
        }
    }
}

void InetAdapter::processSend()
{
    return;
}

void InetAdapter::processStop()
{
    socket.close();
}

void InetAdapter::socketDataArrived(UdpSocket *socket, Packet *packet)
{
    processPacket(packet);
}

void InetAdapter::socketErrorArrived(UdpSocket *socket, Indication *indication)
{
    delete indication;
}

void InetAdapter::socketClosed(UdpSocket *socket)
{
    if (operationalState == State::STOPPING_OPERATION)
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void InetAdapter::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();
}

void InetAdapter::processPacket(Packet *pk)
{
    emit(packetReceivedSignal, pk);
    delete pk;
}

void InetAdapter::handleStartOperation(LifecycleOperation *operation)
{
    simtime_t start = std::max(startTime, simTime());
    if ((stopTime < SIMTIME_ZERO) || (start < stopTime) || (start == stopTime && startTime == stopTime)) {
        selfMsg->setKind(START);
        scheduleAt(start, selfMsg);
    }
}

void InetAdapter::handleStopOperation(LifecycleOperation *operation)
{
    cancelEvent(selfMsg);
    socket.close();
    delayActiveOperationFinish(par("stopOperationTimeout"));
}

void InetAdapter::handleCrashOperation(LifecycleOperation *operation)
{
    cancelEvent(selfMsg);
    socket.destroy();         //TODO  in real operating systems, program crash detected by OS and OS closes sockets of crashed programs.
}










