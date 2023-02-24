//
// Generated file, do not edit! Created by opp_msgtool 6.0 from src/rtps/messages/RtpsInetPacket.msg.
//

#ifndef __INET_RTPSINETPACKET_M_H
#define __INET_RTPSINETPACKET_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// opp_msgtool version check
#define MSGC_VERSION 0x0600
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgtool: 'make clean' should help.
#endif


namespace inet {

class RtpsInetPacket;

}  // namespace inet

#include "inet/common/packet/Packet_m.h" // import inet.common.packet.Packet


namespace inet {

/**
 * Class generated from <tt>src/rtps/messages/RtpsInetPacket.msg:24</tt> by opp_msgtool.
 * <pre>
 * packet RtpsInetPacket extends Packet
 * {
 *     // ---- Protocol related content ----
 *     // General
 *     int payloadSize;
 *     int protocolOverhead;
 *     int rtpsMessageSize;
 * 
 * 
 *     // ---------- Header ----------
 *     int sourceNodeId;
 *     int sourceParticipantId;
 *     string destinationAddress;
 *     int appId;
 * 
 *     // AckNack
 *     bool AckNackSet;
 *     int readerId;
 *     int writerId;
 *     int readerSNStateBase;
 *     int readerSNStateNbrBits;
 *     bool readerSNStateBitmap[256];
 * 
 *     // Data
 *     bool DataSet;
 *     // readerId (s.o.)
 *     // writerId (s.o.)
 *     int writerSN;
 *     int sampleSize;
 * 
 *     // DataFrag
 *     bool DataFragSet;
 *     // readerId (s.o.)
 *     // writerId (s.o.)
 *     // writerSN (s.o.)
 *     int fragmentStartingNum;    // Starting Fragment Number of the fragments in serialized data
 *     int fragmentsInSubmessage;  //Anzahl der fragmente in der DataFrag-Submessage
 *     int fragmentSize;
 *     // sampleSize (s.o.)
 * 
 * 
 *     // HeartBeat
 *     bool HeartBeatSet;
 *     // readerId (s.o.)
 *     // writerId (s.o.)
 *     int firstSN;
 *     int lastSN;
 * 
 * 
 *     // HeartBeatFrag
 *     bool HeartBeatFragSet;
 *     // readerId (s.o.)
 *     // writerId (s.o.)
 *     int lastFragmentNum;
 * 
 *     // InfoDestination
 *     bool InfoDestinationSet;
 *     int destinationNodeId;
 *     int destinationParticipantId;
 * 
 *     // InfoTimestamp
 *     bool InfoTimestampSet;
 *     simtime_t timestamp;
 * 
 *     // NackFrag
 *     bool NackFragSet;
 *     // readerId (s.o.)
 *     // writerId (s.o.)
 *     // writerSN (s.o.)    
 *     int fragmentNumberStateBase;
 *     int fragmentNumberStateNbrBits;
 *     bool fragmentNumberBitmap[256];
 * 
 *     int GeneralFragmentSize;
 * 
 *     // ---- Analysis related content ----
 *     simtime_t publisherSendTime;
 * 
 *     // Raw transmission time over the network (reader_receive_time - writer_send_time)
 *     simtime_t sendTime;
 *     simtime_t receiveTime;
 * 
 *         // Efficency
 *     int fragCount;
 *     int sentFragments;
 * 
 * }
 * </pre>
 */
class RtpsInetPacket : public ::inet::Packet
{
  protected:
    int payloadSize = 0;
    int protocolOverhead = 0;
    int rtpsMessageSize = 0;
    int sourceNodeId = 0;
    int sourceParticipantId = 0;
    ::omnetpp::opp_string destinationAddress;
    int appId = 0;
    bool AckNackSet = false;
    int readerId = 0;
    int writerId = 0;
    int readerSNStateBase = 0;
    int readerSNStateNbrBits = 0;
    bool readerSNStateBitmap[256];
    bool DataSet = false;
    int writerSN = 0;
    int sampleSize = 0;
    bool DataFragSet = false;
    int fragmentStartingNum = 0;
    int fragmentsInSubmessage = 0;
    int fragmentSize = 0;
    bool HeartBeatSet = false;
    int firstSN = 0;
    int lastSN = 0;
    bool HeartBeatFragSet = false;
    int lastFragmentNum = 0;
    bool InfoDestinationSet = false;
    int destinationNodeId = 0;
    int destinationParticipantId = 0;
    bool InfoTimestampSet = false;
    ::omnetpp::simtime_t timestamp = SIMTIME_ZERO;
    bool NackFragSet = false;
    int fragmentNumberStateBase = 0;
    int fragmentNumberStateNbrBits = 0;
    bool fragmentNumberBitmap[256];
    int GeneralFragmentSize = 0;
    ::omnetpp::simtime_t publisherSendTime = SIMTIME_ZERO;
    ::omnetpp::simtime_t sendTime = SIMTIME_ZERO;
    ::omnetpp::simtime_t receiveTime = SIMTIME_ZERO;
    int fragCount = 0;
    int sentFragments = 0;

  private:
    void copy(const RtpsInetPacket& other);

  protected:
    bool operator==(const RtpsInetPacket&) = delete;

  public:
    RtpsInetPacket(const char *name=nullptr, short kind=0);
    RtpsInetPacket(const RtpsInetPacket& other);
    virtual ~RtpsInetPacket();
    RtpsInetPacket& operator=(const RtpsInetPacket& other);
    virtual RtpsInetPacket *dup() const override {return new RtpsInetPacket(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    virtual int getPayloadSize() const;
    virtual void setPayloadSize(int payloadSize);

    virtual int getProtocolOverhead() const;
    virtual void setProtocolOverhead(int protocolOverhead);

    virtual int getRtpsMessageSize() const;
    virtual void setRtpsMessageSize(int rtpsMessageSize);

    virtual int getSourceNodeId() const;
    virtual void setSourceNodeId(int sourceNodeId);

    virtual int getSourceParticipantId() const;
    virtual void setSourceParticipantId(int sourceParticipantId);

    virtual const char * getDestinationAddress() const;
    virtual void setDestinationAddress(const char * destinationAddress);

    virtual int getAppId() const;
    virtual void setAppId(int appId);

    virtual bool getAckNackSet() const;
    virtual void setAckNackSet(bool AckNackSet);

    virtual int getReaderId() const;
    virtual void setReaderId(int readerId);

    virtual int getWriterId() const;
    virtual void setWriterId(int writerId);

    virtual int getReaderSNStateBase() const;
    virtual void setReaderSNStateBase(int readerSNStateBase);

    virtual int getReaderSNStateNbrBits() const;
    virtual void setReaderSNStateNbrBits(int readerSNStateNbrBits);

    virtual size_t getReaderSNStateBitmapArraySize() const;
    virtual bool getReaderSNStateBitmap(size_t k) const;
    virtual void setReaderSNStateBitmap(size_t k, bool readerSNStateBitmap);

    virtual bool getDataSet() const;
    virtual void setDataSet(bool DataSet);

    virtual int getWriterSN() const;
    virtual void setWriterSN(int writerSN);

    virtual int getSampleSize() const;
    virtual void setSampleSize(int sampleSize);

    virtual bool getDataFragSet() const;
    virtual void setDataFragSet(bool DataFragSet);

    virtual int getFragmentStartingNum() const;
    virtual void setFragmentStartingNum(int fragmentStartingNum);

    virtual int getFragmentsInSubmessage() const;
    virtual void setFragmentsInSubmessage(int fragmentsInSubmessage);

    virtual int getFragmentSize() const;
    virtual void setFragmentSize(int fragmentSize);

    virtual bool getHeartBeatSet() const;
    virtual void setHeartBeatSet(bool HeartBeatSet);

    virtual int getFirstSN() const;
    virtual void setFirstSN(int firstSN);

    virtual int getLastSN() const;
    virtual void setLastSN(int lastSN);

    virtual bool getHeartBeatFragSet() const;
    virtual void setHeartBeatFragSet(bool HeartBeatFragSet);

    virtual int getLastFragmentNum() const;
    virtual void setLastFragmentNum(int lastFragmentNum);

    virtual bool getInfoDestinationSet() const;
    virtual void setInfoDestinationSet(bool InfoDestinationSet);

    virtual int getDestinationNodeId() const;
    virtual void setDestinationNodeId(int destinationNodeId);

    virtual int getDestinationParticipantId() const;
    virtual void setDestinationParticipantId(int destinationParticipantId);

    virtual bool getInfoTimestampSet() const;
    virtual void setInfoTimestampSet(bool InfoTimestampSet);

    virtual ::omnetpp::simtime_t getTimestamp() const;
    virtual void setTimestamp(::omnetpp::simtime_t timestamp);

    virtual bool getNackFragSet() const;
    virtual void setNackFragSet(bool NackFragSet);

    virtual int getFragmentNumberStateBase() const;
    virtual void setFragmentNumberStateBase(int fragmentNumberStateBase);

    virtual int getFragmentNumberStateNbrBits() const;
    virtual void setFragmentNumberStateNbrBits(int fragmentNumberStateNbrBits);

    virtual size_t getFragmentNumberBitmapArraySize() const;
    virtual bool getFragmentNumberBitmap(size_t k) const;
    virtual void setFragmentNumberBitmap(size_t k, bool fragmentNumberBitmap);

    virtual int getGeneralFragmentSize() const;
    virtual void setGeneralFragmentSize(int GeneralFragmentSize);

    virtual ::omnetpp::simtime_t getPublisherSendTime() const;
    virtual void setPublisherSendTime(::omnetpp::simtime_t publisherSendTime);

    virtual ::omnetpp::simtime_t getSendTime() const;
    virtual void setSendTime(::omnetpp::simtime_t sendTime);

    virtual ::omnetpp::simtime_t getReceiveTime() const;
    virtual void setReceiveTime(::omnetpp::simtime_t receiveTime);

    virtual int getFragCount() const;
    virtual void setFragCount(int fragCount);

    virtual int getSentFragments() const;
    virtual void setSentFragments(int sentFragments);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const RtpsInetPacket& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, RtpsInetPacket& obj) {obj.parsimUnpack(b);}


}  // namespace inet


namespace omnetpp {

template<> inline inet::RtpsInetPacket *fromAnyPtr(any_ptr ptr) { return check_and_cast<inet::RtpsInetPacket*>(ptr.get<cObject>()); }

}  // namespace omnetpp

#endif // ifndef __INET_RTPSINETPACKET_M_H
