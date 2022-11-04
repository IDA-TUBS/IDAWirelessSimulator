//
// Generated file, do not edit! Created by opp_msgtool 6.0 from src/rtps/messages/RtpsInetPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "RtpsInetPacket_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace inet {

Register_Class(RtpsInetPacket)

RtpsInetPacket::RtpsInetPacket(const char *name, short kind) : ::inet::Packet(name, kind)
{
    std::fill(this->readerSNStateBitmap, this->readerSNStateBitmap + 256, false);
    std::fill(this->fragmentNumberBitmap, this->fragmentNumberBitmap + 256, false);
}

RtpsInetPacket::RtpsInetPacket(const RtpsInetPacket& other) : ::inet::Packet(other)
{
    copy(other);
}

RtpsInetPacket::~RtpsInetPacket()
{
}

RtpsInetPacket& RtpsInetPacket::operator=(const RtpsInetPacket& other)
{
    if (this == &other) return *this;
    ::inet::Packet::operator=(other);
    copy(other);
    return *this;
}

void RtpsInetPacket::copy(const RtpsInetPacket& other)
{
    this->payloadSize = other.payloadSize;
    this->protocolOverhead = other.protocolOverhead;
    this->rtpsMessageSize = other.rtpsMessageSize;
    this->sourceNodeId = other.sourceNodeId;
    this->sourceParticipantId = other.sourceParticipantId;
    this->AckNackSet = other.AckNackSet;
    this->readerId = other.readerId;
    this->writerId = other.writerId;
    this->readerSNStateBase = other.readerSNStateBase;
    this->readerSNStateNbrBits = other.readerSNStateNbrBits;
    for (size_t i = 0; i < 256; i++) {
        this->readerSNStateBitmap[i] = other.readerSNStateBitmap[i];
    }
    this->DataSet = other.DataSet;
    this->writerSN = other.writerSN;
    this->sampleSize = other.sampleSize;
    this->DataFragSet = other.DataFragSet;
    this->fragmentStartingNum = other.fragmentStartingNum;
    this->fragmentsInSubmessage = other.fragmentsInSubmessage;
    this->fragmentSize = other.fragmentSize;
    this->HeartBeatSet = other.HeartBeatSet;
    this->firstSN = other.firstSN;
    this->lastSN = other.lastSN;
    this->HeartBeatFragSet = other.HeartBeatFragSet;
    this->lastFragmentNum = other.lastFragmentNum;
    this->InfoDestinationSet = other.InfoDestinationSet;
    this->destinationNodeId = other.destinationNodeId;
    this->destinationParticipantId = other.destinationParticipantId;
    this->InfoTimestampSet = other.InfoTimestampSet;
    this->timestamp = other.timestamp;
    this->NackFragSet = other.NackFragSet;
    this->fragmentNumberStateBase = other.fragmentNumberStateBase;
    this->fragmentNumberStateNbrBits = other.fragmentNumberStateNbrBits;
    for (size_t i = 0; i < 256; i++) {
        this->fragmentNumberBitmap[i] = other.fragmentNumberBitmap[i];
    }
    this->GeneralFragmentSize = other.GeneralFragmentSize;
    this->publisherSendTime = other.publisherSendTime;
    this->writerSendTime = other.writerSendTime;
    this->readerReceiveTime = other.readerReceiveTime;
    this->fragCount = other.fragCount;
    this->sentFragments = other.sentFragments;
}

void RtpsInetPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::Packet::parsimPack(b);
    doParsimPacking(b,this->payloadSize);
    doParsimPacking(b,this->protocolOverhead);
    doParsimPacking(b,this->rtpsMessageSize);
    doParsimPacking(b,this->sourceNodeId);
    doParsimPacking(b,this->sourceParticipantId);
    doParsimPacking(b,this->AckNackSet);
    doParsimPacking(b,this->readerId);
    doParsimPacking(b,this->writerId);
    doParsimPacking(b,this->readerSNStateBase);
    doParsimPacking(b,this->readerSNStateNbrBits);
    doParsimArrayPacking(b,this->readerSNStateBitmap,256);
    doParsimPacking(b,this->DataSet);
    doParsimPacking(b,this->writerSN);
    doParsimPacking(b,this->sampleSize);
    doParsimPacking(b,this->DataFragSet);
    doParsimPacking(b,this->fragmentStartingNum);
    doParsimPacking(b,this->fragmentsInSubmessage);
    doParsimPacking(b,this->fragmentSize);
    doParsimPacking(b,this->HeartBeatSet);
    doParsimPacking(b,this->firstSN);
    doParsimPacking(b,this->lastSN);
    doParsimPacking(b,this->HeartBeatFragSet);
    doParsimPacking(b,this->lastFragmentNum);
    doParsimPacking(b,this->InfoDestinationSet);
    doParsimPacking(b,this->destinationNodeId);
    doParsimPacking(b,this->destinationParticipantId);
    doParsimPacking(b,this->InfoTimestampSet);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->NackFragSet);
    doParsimPacking(b,this->fragmentNumberStateBase);
    doParsimPacking(b,this->fragmentNumberStateNbrBits);
    doParsimArrayPacking(b,this->fragmentNumberBitmap,256);
    doParsimPacking(b,this->GeneralFragmentSize);
    doParsimPacking(b,this->publisherSendTime);
    doParsimPacking(b,this->writerSendTime);
    doParsimPacking(b,this->readerReceiveTime);
    doParsimPacking(b,this->fragCount);
    doParsimPacking(b,this->sentFragments);
}

void RtpsInetPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::Packet::parsimUnpack(b);
    doParsimUnpacking(b,this->payloadSize);
    doParsimUnpacking(b,this->protocolOverhead);
    doParsimUnpacking(b,this->rtpsMessageSize);
    doParsimUnpacking(b,this->sourceNodeId);
    doParsimUnpacking(b,this->sourceParticipantId);
    doParsimUnpacking(b,this->AckNackSet);
    doParsimUnpacking(b,this->readerId);
    doParsimUnpacking(b,this->writerId);
    doParsimUnpacking(b,this->readerSNStateBase);
    doParsimUnpacking(b,this->readerSNStateNbrBits);
    doParsimArrayUnpacking(b,this->readerSNStateBitmap,256);
    doParsimUnpacking(b,this->DataSet);
    doParsimUnpacking(b,this->writerSN);
    doParsimUnpacking(b,this->sampleSize);
    doParsimUnpacking(b,this->DataFragSet);
    doParsimUnpacking(b,this->fragmentStartingNum);
    doParsimUnpacking(b,this->fragmentsInSubmessage);
    doParsimUnpacking(b,this->fragmentSize);
    doParsimUnpacking(b,this->HeartBeatSet);
    doParsimUnpacking(b,this->firstSN);
    doParsimUnpacking(b,this->lastSN);
    doParsimUnpacking(b,this->HeartBeatFragSet);
    doParsimUnpacking(b,this->lastFragmentNum);
    doParsimUnpacking(b,this->InfoDestinationSet);
    doParsimUnpacking(b,this->destinationNodeId);
    doParsimUnpacking(b,this->destinationParticipantId);
    doParsimUnpacking(b,this->InfoTimestampSet);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->NackFragSet);
    doParsimUnpacking(b,this->fragmentNumberStateBase);
    doParsimUnpacking(b,this->fragmentNumberStateNbrBits);
    doParsimArrayUnpacking(b,this->fragmentNumberBitmap,256);
    doParsimUnpacking(b,this->GeneralFragmentSize);
    doParsimUnpacking(b,this->publisherSendTime);
    doParsimUnpacking(b,this->writerSendTime);
    doParsimUnpacking(b,this->readerReceiveTime);
    doParsimUnpacking(b,this->fragCount);
    doParsimUnpacking(b,this->sentFragments);
}

int RtpsInetPacket::getPayloadSize() const
{
    return this->payloadSize;
}

void RtpsInetPacket::setPayloadSize(int payloadSize)
{
    this->payloadSize = payloadSize;
}

int RtpsInetPacket::getProtocolOverhead() const
{
    return this->protocolOverhead;
}

void RtpsInetPacket::setProtocolOverhead(int protocolOverhead)
{
    this->protocolOverhead = protocolOverhead;
}

int RtpsInetPacket::getRtpsMessageSize() const
{
    return this->rtpsMessageSize;
}

void RtpsInetPacket::setRtpsMessageSize(int rtpsMessageSize)
{
    this->rtpsMessageSize = rtpsMessageSize;
}

int RtpsInetPacket::getSourceNodeId() const
{
    return this->sourceNodeId;
}

void RtpsInetPacket::setSourceNodeId(int sourceNodeId)
{
    this->sourceNodeId = sourceNodeId;
}

int RtpsInetPacket::getSourceParticipantId() const
{
    return this->sourceParticipantId;
}

void RtpsInetPacket::setSourceParticipantId(int sourceParticipantId)
{
    this->sourceParticipantId = sourceParticipantId;
}

bool RtpsInetPacket::getAckNackSet() const
{
    return this->AckNackSet;
}

void RtpsInetPacket::setAckNackSet(bool AckNackSet)
{
    this->AckNackSet = AckNackSet;
}

int RtpsInetPacket::getReaderId() const
{
    return this->readerId;
}

void RtpsInetPacket::setReaderId(int readerId)
{
    this->readerId = readerId;
}

int RtpsInetPacket::getWriterId() const
{
    return this->writerId;
}

void RtpsInetPacket::setWriterId(int writerId)
{
    this->writerId = writerId;
}

int RtpsInetPacket::getReaderSNStateBase() const
{
    return this->readerSNStateBase;
}

void RtpsInetPacket::setReaderSNStateBase(int readerSNStateBase)
{
    this->readerSNStateBase = readerSNStateBase;
}

int RtpsInetPacket::getReaderSNStateNbrBits() const
{
    return this->readerSNStateNbrBits;
}

void RtpsInetPacket::setReaderSNStateNbrBits(int readerSNStateNbrBits)
{
    this->readerSNStateNbrBits = readerSNStateNbrBits;
}

size_t RtpsInetPacket::getReaderSNStateBitmapArraySize() const
{
    return 256;
}

bool RtpsInetPacket::getReaderSNStateBitmap(size_t k) const
{
    if (k >= 256) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)256, (unsigned long)k);
    return this->readerSNStateBitmap[k];
}

void RtpsInetPacket::setReaderSNStateBitmap(size_t k, bool readerSNStateBitmap)
{
    if (k >= 256) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)256, (unsigned long)k);
    this->readerSNStateBitmap[k] = readerSNStateBitmap;
}

bool RtpsInetPacket::getDataSet() const
{
    return this->DataSet;
}

void RtpsInetPacket::setDataSet(bool DataSet)
{
    this->DataSet = DataSet;
}

int RtpsInetPacket::getWriterSN() const
{
    return this->writerSN;
}

void RtpsInetPacket::setWriterSN(int writerSN)
{
    this->writerSN = writerSN;
}

int RtpsInetPacket::getSampleSize() const
{
    return this->sampleSize;
}

void RtpsInetPacket::setSampleSize(int sampleSize)
{
    this->sampleSize = sampleSize;
}

bool RtpsInetPacket::getDataFragSet() const
{
    return this->DataFragSet;
}

void RtpsInetPacket::setDataFragSet(bool DataFragSet)
{
    this->DataFragSet = DataFragSet;
}

int RtpsInetPacket::getFragmentStartingNum() const
{
    return this->fragmentStartingNum;
}

void RtpsInetPacket::setFragmentStartingNum(int fragmentStartingNum)
{
    this->fragmentStartingNum = fragmentStartingNum;
}

int RtpsInetPacket::getFragmentsInSubmessage() const
{
    return this->fragmentsInSubmessage;
}

void RtpsInetPacket::setFragmentsInSubmessage(int fragmentsInSubmessage)
{
    this->fragmentsInSubmessage = fragmentsInSubmessage;
}

int RtpsInetPacket::getFragmentSize() const
{
    return this->fragmentSize;
}

void RtpsInetPacket::setFragmentSize(int fragmentSize)
{
    this->fragmentSize = fragmentSize;
}

bool RtpsInetPacket::getHeartBeatSet() const
{
    return this->HeartBeatSet;
}

void RtpsInetPacket::setHeartBeatSet(bool HeartBeatSet)
{
    this->HeartBeatSet = HeartBeatSet;
}

int RtpsInetPacket::getFirstSN() const
{
    return this->firstSN;
}

void RtpsInetPacket::setFirstSN(int firstSN)
{
    this->firstSN = firstSN;
}

int RtpsInetPacket::getLastSN() const
{
    return this->lastSN;
}

void RtpsInetPacket::setLastSN(int lastSN)
{
    this->lastSN = lastSN;
}

bool RtpsInetPacket::getHeartBeatFragSet() const
{
    return this->HeartBeatFragSet;
}

void RtpsInetPacket::setHeartBeatFragSet(bool HeartBeatFragSet)
{
    this->HeartBeatFragSet = HeartBeatFragSet;
}

int RtpsInetPacket::getLastFragmentNum() const
{
    return this->lastFragmentNum;
}

void RtpsInetPacket::setLastFragmentNum(int lastFragmentNum)
{
    this->lastFragmentNum = lastFragmentNum;
}

bool RtpsInetPacket::getInfoDestinationSet() const
{
    return this->InfoDestinationSet;
}

void RtpsInetPacket::setInfoDestinationSet(bool InfoDestinationSet)
{
    this->InfoDestinationSet = InfoDestinationSet;
}

int RtpsInetPacket::getDestinationNodeId() const
{
    return this->destinationNodeId;
}

void RtpsInetPacket::setDestinationNodeId(int destinationNodeId)
{
    this->destinationNodeId = destinationNodeId;
}

int RtpsInetPacket::getDestinationParticipantId() const
{
    return this->destinationParticipantId;
}

void RtpsInetPacket::setDestinationParticipantId(int destinationParticipantId)
{
    this->destinationParticipantId = destinationParticipantId;
}

bool RtpsInetPacket::getInfoTimestampSet() const
{
    return this->InfoTimestampSet;
}

void RtpsInetPacket::setInfoTimestampSet(bool InfoTimestampSet)
{
    this->InfoTimestampSet = InfoTimestampSet;
}

::omnetpp::simtime_t RtpsInetPacket::getTimestamp() const
{
    return this->timestamp;
}

void RtpsInetPacket::setTimestamp(::omnetpp::simtime_t timestamp)
{
    this->timestamp = timestamp;
}

bool RtpsInetPacket::getNackFragSet() const
{
    return this->NackFragSet;
}

void RtpsInetPacket::setNackFragSet(bool NackFragSet)
{
    this->NackFragSet = NackFragSet;
}

int RtpsInetPacket::getFragmentNumberStateBase() const
{
    return this->fragmentNumberStateBase;
}

void RtpsInetPacket::setFragmentNumberStateBase(int fragmentNumberStateBase)
{
    this->fragmentNumberStateBase = fragmentNumberStateBase;
}

int RtpsInetPacket::getFragmentNumberStateNbrBits() const
{
    return this->fragmentNumberStateNbrBits;
}

void RtpsInetPacket::setFragmentNumberStateNbrBits(int fragmentNumberStateNbrBits)
{
    this->fragmentNumberStateNbrBits = fragmentNumberStateNbrBits;
}

size_t RtpsInetPacket::getFragmentNumberBitmapArraySize() const
{
    return 256;
}

bool RtpsInetPacket::getFragmentNumberBitmap(size_t k) const
{
    if (k >= 256) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)256, (unsigned long)k);
    return this->fragmentNumberBitmap[k];
}

void RtpsInetPacket::setFragmentNumberBitmap(size_t k, bool fragmentNumberBitmap)
{
    if (k >= 256) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)256, (unsigned long)k);
    this->fragmentNumberBitmap[k] = fragmentNumberBitmap;
}

int RtpsInetPacket::getGeneralFragmentSize() const
{
    return this->GeneralFragmentSize;
}

void RtpsInetPacket::setGeneralFragmentSize(int GeneralFragmentSize)
{
    this->GeneralFragmentSize = GeneralFragmentSize;
}

::omnetpp::simtime_t RtpsInetPacket::getPublisherSendTime() const
{
    return this->publisherSendTime;
}

void RtpsInetPacket::setPublisherSendTime(::omnetpp::simtime_t publisherSendTime)
{
    this->publisherSendTime = publisherSendTime;
}

::omnetpp::simtime_t RtpsInetPacket::getWriterSendTime() const
{
    return this->writerSendTime;
}

void RtpsInetPacket::setWriterSendTime(::omnetpp::simtime_t writerSendTime)
{
    this->writerSendTime = writerSendTime;
}

::omnetpp::simtime_t RtpsInetPacket::getReaderReceiveTime() const
{
    return this->readerReceiveTime;
}

void RtpsInetPacket::setReaderReceiveTime(::omnetpp::simtime_t readerReceiveTime)
{
    this->readerReceiveTime = readerReceiveTime;
}

int RtpsInetPacket::getFragCount() const
{
    return this->fragCount;
}

void RtpsInetPacket::setFragCount(int fragCount)
{
    this->fragCount = fragCount;
}

int RtpsInetPacket::getSentFragments() const
{
    return this->sentFragments;
}

void RtpsInetPacket::setSentFragments(int sentFragments)
{
    this->sentFragments = sentFragments;
}

class RtpsInetPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_payloadSize,
        FIELD_protocolOverhead,
        FIELD_rtpsMessageSize,
        FIELD_sourceNodeId,
        FIELD_sourceParticipantId,
        FIELD_AckNackSet,
        FIELD_readerId,
        FIELD_writerId,
        FIELD_readerSNStateBase,
        FIELD_readerSNStateNbrBits,
        FIELD_readerSNStateBitmap,
        FIELD_DataSet,
        FIELD_writerSN,
        FIELD_sampleSize,
        FIELD_DataFragSet,
        FIELD_fragmentStartingNum,
        FIELD_fragmentsInSubmessage,
        FIELD_fragmentSize,
        FIELD_HeartBeatSet,
        FIELD_firstSN,
        FIELD_lastSN,
        FIELD_HeartBeatFragSet,
        FIELD_lastFragmentNum,
        FIELD_InfoDestinationSet,
        FIELD_destinationNodeId,
        FIELD_destinationParticipantId,
        FIELD_InfoTimestampSet,
        FIELD_timestamp,
        FIELD_NackFragSet,
        FIELD_fragmentNumberStateBase,
        FIELD_fragmentNumberStateNbrBits,
        FIELD_fragmentNumberBitmap,
        FIELD_GeneralFragmentSize,
        FIELD_publisherSendTime,
        FIELD_writerSendTime,
        FIELD_readerReceiveTime,
        FIELD_fragCount,
        FIELD_sentFragments,
    };
  public:
    RtpsInetPacketDescriptor();
    virtual ~RtpsInetPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(RtpsInetPacketDescriptor)

RtpsInetPacketDescriptor::RtpsInetPacketDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::RtpsInetPacket)), "inet::Packet")
{
    propertyNames = nullptr;
}

RtpsInetPacketDescriptor::~RtpsInetPacketDescriptor()
{
    delete[] propertyNames;
}

bool RtpsInetPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<RtpsInetPacket *>(obj)!=nullptr;
}

const char **RtpsInetPacketDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *RtpsInetPacketDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int RtpsInetPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 38+base->getFieldCount() : 38;
}

unsigned int RtpsInetPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_payloadSize
        FD_ISEDITABLE,    // FIELD_protocolOverhead
        FD_ISEDITABLE,    // FIELD_rtpsMessageSize
        FD_ISEDITABLE,    // FIELD_sourceNodeId
        FD_ISEDITABLE,    // FIELD_sourceParticipantId
        FD_ISEDITABLE,    // FIELD_AckNackSet
        FD_ISEDITABLE,    // FIELD_readerId
        FD_ISEDITABLE,    // FIELD_writerId
        FD_ISEDITABLE,    // FIELD_readerSNStateBase
        FD_ISEDITABLE,    // FIELD_readerSNStateNbrBits
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_readerSNStateBitmap
        FD_ISEDITABLE,    // FIELD_DataSet
        FD_ISEDITABLE,    // FIELD_writerSN
        FD_ISEDITABLE,    // FIELD_sampleSize
        FD_ISEDITABLE,    // FIELD_DataFragSet
        FD_ISEDITABLE,    // FIELD_fragmentStartingNum
        FD_ISEDITABLE,    // FIELD_fragmentsInSubmessage
        FD_ISEDITABLE,    // FIELD_fragmentSize
        FD_ISEDITABLE,    // FIELD_HeartBeatSet
        FD_ISEDITABLE,    // FIELD_firstSN
        FD_ISEDITABLE,    // FIELD_lastSN
        FD_ISEDITABLE,    // FIELD_HeartBeatFragSet
        FD_ISEDITABLE,    // FIELD_lastFragmentNum
        FD_ISEDITABLE,    // FIELD_InfoDestinationSet
        FD_ISEDITABLE,    // FIELD_destinationNodeId
        FD_ISEDITABLE,    // FIELD_destinationParticipantId
        FD_ISEDITABLE,    // FIELD_InfoTimestampSet
        FD_ISEDITABLE,    // FIELD_timestamp
        FD_ISEDITABLE,    // FIELD_NackFragSet
        FD_ISEDITABLE,    // FIELD_fragmentNumberStateBase
        FD_ISEDITABLE,    // FIELD_fragmentNumberStateNbrBits
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_fragmentNumberBitmap
        FD_ISEDITABLE,    // FIELD_GeneralFragmentSize
        FD_ISEDITABLE,    // FIELD_publisherSendTime
        FD_ISEDITABLE,    // FIELD_writerSendTime
        FD_ISEDITABLE,    // FIELD_readerReceiveTime
        FD_ISEDITABLE,    // FIELD_fragCount
        FD_ISEDITABLE,    // FIELD_sentFragments
    };
    return (field >= 0 && field < 38) ? fieldTypeFlags[field] : 0;
}

const char *RtpsInetPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "payloadSize",
        "protocolOverhead",
        "rtpsMessageSize",
        "sourceNodeId",
        "sourceParticipantId",
        "AckNackSet",
        "readerId",
        "writerId",
        "readerSNStateBase",
        "readerSNStateNbrBits",
        "readerSNStateBitmap",
        "DataSet",
        "writerSN",
        "sampleSize",
        "DataFragSet",
        "fragmentStartingNum",
        "fragmentsInSubmessage",
        "fragmentSize",
        "HeartBeatSet",
        "firstSN",
        "lastSN",
        "HeartBeatFragSet",
        "lastFragmentNum",
        "InfoDestinationSet",
        "destinationNodeId",
        "destinationParticipantId",
        "InfoTimestampSet",
        "timestamp",
        "NackFragSet",
        "fragmentNumberStateBase",
        "fragmentNumberStateNbrBits",
        "fragmentNumberBitmap",
        "GeneralFragmentSize",
        "publisherSendTime",
        "writerSendTime",
        "readerReceiveTime",
        "fragCount",
        "sentFragments",
    };
    return (field >= 0 && field < 38) ? fieldNames[field] : nullptr;
}

int RtpsInetPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "payloadSize") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "protocolOverhead") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "rtpsMessageSize") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "sourceNodeId") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "sourceParticipantId") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "AckNackSet") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "readerId") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "writerId") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "readerSNStateBase") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "readerSNStateNbrBits") == 0) return baseIndex + 9;
    if (strcmp(fieldName, "readerSNStateBitmap") == 0) return baseIndex + 10;
    if (strcmp(fieldName, "DataSet") == 0) return baseIndex + 11;
    if (strcmp(fieldName, "writerSN") == 0) return baseIndex + 12;
    if (strcmp(fieldName, "sampleSize") == 0) return baseIndex + 13;
    if (strcmp(fieldName, "DataFragSet") == 0) return baseIndex + 14;
    if (strcmp(fieldName, "fragmentStartingNum") == 0) return baseIndex + 15;
    if (strcmp(fieldName, "fragmentsInSubmessage") == 0) return baseIndex + 16;
    if (strcmp(fieldName, "fragmentSize") == 0) return baseIndex + 17;
    if (strcmp(fieldName, "HeartBeatSet") == 0) return baseIndex + 18;
    if (strcmp(fieldName, "firstSN") == 0) return baseIndex + 19;
    if (strcmp(fieldName, "lastSN") == 0) return baseIndex + 20;
    if (strcmp(fieldName, "HeartBeatFragSet") == 0) return baseIndex + 21;
    if (strcmp(fieldName, "lastFragmentNum") == 0) return baseIndex + 22;
    if (strcmp(fieldName, "InfoDestinationSet") == 0) return baseIndex + 23;
    if (strcmp(fieldName, "destinationNodeId") == 0) return baseIndex + 24;
    if (strcmp(fieldName, "destinationParticipantId") == 0) return baseIndex + 25;
    if (strcmp(fieldName, "InfoTimestampSet") == 0) return baseIndex + 26;
    if (strcmp(fieldName, "timestamp") == 0) return baseIndex + 27;
    if (strcmp(fieldName, "NackFragSet") == 0) return baseIndex + 28;
    if (strcmp(fieldName, "fragmentNumberStateBase") == 0) return baseIndex + 29;
    if (strcmp(fieldName, "fragmentNumberStateNbrBits") == 0) return baseIndex + 30;
    if (strcmp(fieldName, "fragmentNumberBitmap") == 0) return baseIndex + 31;
    if (strcmp(fieldName, "GeneralFragmentSize") == 0) return baseIndex + 32;
    if (strcmp(fieldName, "publisherSendTime") == 0) return baseIndex + 33;
    if (strcmp(fieldName, "writerSendTime") == 0) return baseIndex + 34;
    if (strcmp(fieldName, "readerReceiveTime") == 0) return baseIndex + 35;
    if (strcmp(fieldName, "fragCount") == 0) return baseIndex + 36;
    if (strcmp(fieldName, "sentFragments") == 0) return baseIndex + 37;
    return base ? base->findField(fieldName) : -1;
}

const char *RtpsInetPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_payloadSize
        "int",    // FIELD_protocolOverhead
        "int",    // FIELD_rtpsMessageSize
        "int",    // FIELD_sourceNodeId
        "int",    // FIELD_sourceParticipantId
        "bool",    // FIELD_AckNackSet
        "int",    // FIELD_readerId
        "int",    // FIELD_writerId
        "int",    // FIELD_readerSNStateBase
        "int",    // FIELD_readerSNStateNbrBits
        "bool",    // FIELD_readerSNStateBitmap
        "bool",    // FIELD_DataSet
        "int",    // FIELD_writerSN
        "int",    // FIELD_sampleSize
        "bool",    // FIELD_DataFragSet
        "int",    // FIELD_fragmentStartingNum
        "int",    // FIELD_fragmentsInSubmessage
        "int",    // FIELD_fragmentSize
        "bool",    // FIELD_HeartBeatSet
        "int",    // FIELD_firstSN
        "int",    // FIELD_lastSN
        "bool",    // FIELD_HeartBeatFragSet
        "int",    // FIELD_lastFragmentNum
        "bool",    // FIELD_InfoDestinationSet
        "int",    // FIELD_destinationNodeId
        "int",    // FIELD_destinationParticipantId
        "bool",    // FIELD_InfoTimestampSet
        "omnetpp::simtime_t",    // FIELD_timestamp
        "bool",    // FIELD_NackFragSet
        "int",    // FIELD_fragmentNumberStateBase
        "int",    // FIELD_fragmentNumberStateNbrBits
        "bool",    // FIELD_fragmentNumberBitmap
        "int",    // FIELD_GeneralFragmentSize
        "omnetpp::simtime_t",    // FIELD_publisherSendTime
        "omnetpp::simtime_t",    // FIELD_writerSendTime
        "omnetpp::simtime_t",    // FIELD_readerReceiveTime
        "int",    // FIELD_fragCount
        "int",    // FIELD_sentFragments
    };
    return (field >= 0 && field < 38) ? fieldTypeStrings[field] : nullptr;
}

const char **RtpsInetPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *RtpsInetPacketDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int RtpsInetPacketDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    RtpsInetPacket *pp = omnetpp::fromAnyPtr<RtpsInetPacket>(object); (void)pp;
    switch (field) {
        case FIELD_readerSNStateBitmap: return 256;
        case FIELD_fragmentNumberBitmap: return 256;
        default: return 0;
    }
}

void RtpsInetPacketDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    RtpsInetPacket *pp = omnetpp::fromAnyPtr<RtpsInetPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'RtpsInetPacket'", field);
    }
}

const char *RtpsInetPacketDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    RtpsInetPacket *pp = omnetpp::fromAnyPtr<RtpsInetPacket>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string RtpsInetPacketDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    RtpsInetPacket *pp = omnetpp::fromAnyPtr<RtpsInetPacket>(object); (void)pp;
    switch (field) {
        case FIELD_payloadSize: return long2string(pp->getPayloadSize());
        case FIELD_protocolOverhead: return long2string(pp->getProtocolOverhead());
        case FIELD_rtpsMessageSize: return long2string(pp->getRtpsMessageSize());
        case FIELD_sourceNodeId: return long2string(pp->getSourceNodeId());
        case FIELD_sourceParticipantId: return long2string(pp->getSourceParticipantId());
        case FIELD_AckNackSet: return bool2string(pp->getAckNackSet());
        case FIELD_readerId: return long2string(pp->getReaderId());
        case FIELD_writerId: return long2string(pp->getWriterId());
        case FIELD_readerSNStateBase: return long2string(pp->getReaderSNStateBase());
        case FIELD_readerSNStateNbrBits: return long2string(pp->getReaderSNStateNbrBits());
        case FIELD_readerSNStateBitmap: return bool2string(pp->getReaderSNStateBitmap(i));
        case FIELD_DataSet: return bool2string(pp->getDataSet());
        case FIELD_writerSN: return long2string(pp->getWriterSN());
        case FIELD_sampleSize: return long2string(pp->getSampleSize());
        case FIELD_DataFragSet: return bool2string(pp->getDataFragSet());
        case FIELD_fragmentStartingNum: return long2string(pp->getFragmentStartingNum());
        case FIELD_fragmentsInSubmessage: return long2string(pp->getFragmentsInSubmessage());
        case FIELD_fragmentSize: return long2string(pp->getFragmentSize());
        case FIELD_HeartBeatSet: return bool2string(pp->getHeartBeatSet());
        case FIELD_firstSN: return long2string(pp->getFirstSN());
        case FIELD_lastSN: return long2string(pp->getLastSN());
        case FIELD_HeartBeatFragSet: return bool2string(pp->getHeartBeatFragSet());
        case FIELD_lastFragmentNum: return long2string(pp->getLastFragmentNum());
        case FIELD_InfoDestinationSet: return bool2string(pp->getInfoDestinationSet());
        case FIELD_destinationNodeId: return long2string(pp->getDestinationNodeId());
        case FIELD_destinationParticipantId: return long2string(pp->getDestinationParticipantId());
        case FIELD_InfoTimestampSet: return bool2string(pp->getInfoTimestampSet());
        case FIELD_timestamp: return simtime2string(pp->getTimestamp());
        case FIELD_NackFragSet: return bool2string(pp->getNackFragSet());
        case FIELD_fragmentNumberStateBase: return long2string(pp->getFragmentNumberStateBase());
        case FIELD_fragmentNumberStateNbrBits: return long2string(pp->getFragmentNumberStateNbrBits());
        case FIELD_fragmentNumberBitmap: return bool2string(pp->getFragmentNumberBitmap(i));
        case FIELD_GeneralFragmentSize: return long2string(pp->getGeneralFragmentSize());
        case FIELD_publisherSendTime: return simtime2string(pp->getPublisherSendTime());
        case FIELD_writerSendTime: return simtime2string(pp->getWriterSendTime());
        case FIELD_readerReceiveTime: return simtime2string(pp->getReaderReceiveTime());
        case FIELD_fragCount: return long2string(pp->getFragCount());
        case FIELD_sentFragments: return long2string(pp->getSentFragments());
        default: return "";
    }
}

void RtpsInetPacketDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RtpsInetPacket *pp = omnetpp::fromAnyPtr<RtpsInetPacket>(object); (void)pp;
    switch (field) {
        case FIELD_payloadSize: pp->setPayloadSize(string2long(value)); break;
        case FIELD_protocolOverhead: pp->setProtocolOverhead(string2long(value)); break;
        case FIELD_rtpsMessageSize: pp->setRtpsMessageSize(string2long(value)); break;
        case FIELD_sourceNodeId: pp->setSourceNodeId(string2long(value)); break;
        case FIELD_sourceParticipantId: pp->setSourceParticipantId(string2long(value)); break;
        case FIELD_AckNackSet: pp->setAckNackSet(string2bool(value)); break;
        case FIELD_readerId: pp->setReaderId(string2long(value)); break;
        case FIELD_writerId: pp->setWriterId(string2long(value)); break;
        case FIELD_readerSNStateBase: pp->setReaderSNStateBase(string2long(value)); break;
        case FIELD_readerSNStateNbrBits: pp->setReaderSNStateNbrBits(string2long(value)); break;
        case FIELD_readerSNStateBitmap: pp->setReaderSNStateBitmap(i,string2bool(value)); break;
        case FIELD_DataSet: pp->setDataSet(string2bool(value)); break;
        case FIELD_writerSN: pp->setWriterSN(string2long(value)); break;
        case FIELD_sampleSize: pp->setSampleSize(string2long(value)); break;
        case FIELD_DataFragSet: pp->setDataFragSet(string2bool(value)); break;
        case FIELD_fragmentStartingNum: pp->setFragmentStartingNum(string2long(value)); break;
        case FIELD_fragmentsInSubmessage: pp->setFragmentsInSubmessage(string2long(value)); break;
        case FIELD_fragmentSize: pp->setFragmentSize(string2long(value)); break;
        case FIELD_HeartBeatSet: pp->setHeartBeatSet(string2bool(value)); break;
        case FIELD_firstSN: pp->setFirstSN(string2long(value)); break;
        case FIELD_lastSN: pp->setLastSN(string2long(value)); break;
        case FIELD_HeartBeatFragSet: pp->setHeartBeatFragSet(string2bool(value)); break;
        case FIELD_lastFragmentNum: pp->setLastFragmentNum(string2long(value)); break;
        case FIELD_InfoDestinationSet: pp->setInfoDestinationSet(string2bool(value)); break;
        case FIELD_destinationNodeId: pp->setDestinationNodeId(string2long(value)); break;
        case FIELD_destinationParticipantId: pp->setDestinationParticipantId(string2long(value)); break;
        case FIELD_InfoTimestampSet: pp->setInfoTimestampSet(string2bool(value)); break;
        case FIELD_timestamp: pp->setTimestamp(string2simtime(value)); break;
        case FIELD_NackFragSet: pp->setNackFragSet(string2bool(value)); break;
        case FIELD_fragmentNumberStateBase: pp->setFragmentNumberStateBase(string2long(value)); break;
        case FIELD_fragmentNumberStateNbrBits: pp->setFragmentNumberStateNbrBits(string2long(value)); break;
        case FIELD_fragmentNumberBitmap: pp->setFragmentNumberBitmap(i,string2bool(value)); break;
        case FIELD_GeneralFragmentSize: pp->setGeneralFragmentSize(string2long(value)); break;
        case FIELD_publisherSendTime: pp->setPublisherSendTime(string2simtime(value)); break;
        case FIELD_writerSendTime: pp->setWriterSendTime(string2simtime(value)); break;
        case FIELD_readerReceiveTime: pp->setReaderReceiveTime(string2simtime(value)); break;
        case FIELD_fragCount: pp->setFragCount(string2long(value)); break;
        case FIELD_sentFragments: pp->setSentFragments(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RtpsInetPacket'", field);
    }
}

omnetpp::cValue RtpsInetPacketDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    RtpsInetPacket *pp = omnetpp::fromAnyPtr<RtpsInetPacket>(object); (void)pp;
    switch (field) {
        case FIELD_payloadSize: return pp->getPayloadSize();
        case FIELD_protocolOverhead: return pp->getProtocolOverhead();
        case FIELD_rtpsMessageSize: return pp->getRtpsMessageSize();
        case FIELD_sourceNodeId: return pp->getSourceNodeId();
        case FIELD_sourceParticipantId: return pp->getSourceParticipantId();
        case FIELD_AckNackSet: return pp->getAckNackSet();
        case FIELD_readerId: return pp->getReaderId();
        case FIELD_writerId: return pp->getWriterId();
        case FIELD_readerSNStateBase: return pp->getReaderSNStateBase();
        case FIELD_readerSNStateNbrBits: return pp->getReaderSNStateNbrBits();
        case FIELD_readerSNStateBitmap: return pp->getReaderSNStateBitmap(i);
        case FIELD_DataSet: return pp->getDataSet();
        case FIELD_writerSN: return pp->getWriterSN();
        case FIELD_sampleSize: return pp->getSampleSize();
        case FIELD_DataFragSet: return pp->getDataFragSet();
        case FIELD_fragmentStartingNum: return pp->getFragmentStartingNum();
        case FIELD_fragmentsInSubmessage: return pp->getFragmentsInSubmessage();
        case FIELD_fragmentSize: return pp->getFragmentSize();
        case FIELD_HeartBeatSet: return pp->getHeartBeatSet();
        case FIELD_firstSN: return pp->getFirstSN();
        case FIELD_lastSN: return pp->getLastSN();
        case FIELD_HeartBeatFragSet: return pp->getHeartBeatFragSet();
        case FIELD_lastFragmentNum: return pp->getLastFragmentNum();
        case FIELD_InfoDestinationSet: return pp->getInfoDestinationSet();
        case FIELD_destinationNodeId: return pp->getDestinationNodeId();
        case FIELD_destinationParticipantId: return pp->getDestinationParticipantId();
        case FIELD_InfoTimestampSet: return pp->getInfoTimestampSet();
        case FIELD_timestamp: return pp->getTimestamp().dbl();
        case FIELD_NackFragSet: return pp->getNackFragSet();
        case FIELD_fragmentNumberStateBase: return pp->getFragmentNumberStateBase();
        case FIELD_fragmentNumberStateNbrBits: return pp->getFragmentNumberStateNbrBits();
        case FIELD_fragmentNumberBitmap: return pp->getFragmentNumberBitmap(i);
        case FIELD_GeneralFragmentSize: return pp->getGeneralFragmentSize();
        case FIELD_publisherSendTime: return pp->getPublisherSendTime().dbl();
        case FIELD_writerSendTime: return pp->getWriterSendTime().dbl();
        case FIELD_readerReceiveTime: return pp->getReaderReceiveTime().dbl();
        case FIELD_fragCount: return pp->getFragCount();
        case FIELD_sentFragments: return pp->getSentFragments();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'RtpsInetPacket' as cValue -- field index out of range?", field);
    }
}

void RtpsInetPacketDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RtpsInetPacket *pp = omnetpp::fromAnyPtr<RtpsInetPacket>(object); (void)pp;
    switch (field) {
        case FIELD_payloadSize: pp->setPayloadSize(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_protocolOverhead: pp->setProtocolOverhead(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_rtpsMessageSize: pp->setRtpsMessageSize(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sourceNodeId: pp->setSourceNodeId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sourceParticipantId: pp->setSourceParticipantId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_AckNackSet: pp->setAckNackSet(value.boolValue()); break;
        case FIELD_readerId: pp->setReaderId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_writerId: pp->setWriterId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_readerSNStateBase: pp->setReaderSNStateBase(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_readerSNStateNbrBits: pp->setReaderSNStateNbrBits(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_readerSNStateBitmap: pp->setReaderSNStateBitmap(i,value.boolValue()); break;
        case FIELD_DataSet: pp->setDataSet(value.boolValue()); break;
        case FIELD_writerSN: pp->setWriterSN(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sampleSize: pp->setSampleSize(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_DataFragSet: pp->setDataFragSet(value.boolValue()); break;
        case FIELD_fragmentStartingNum: pp->setFragmentStartingNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_fragmentsInSubmessage: pp->setFragmentsInSubmessage(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_fragmentSize: pp->setFragmentSize(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_HeartBeatSet: pp->setHeartBeatSet(value.boolValue()); break;
        case FIELD_firstSN: pp->setFirstSN(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_lastSN: pp->setLastSN(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_HeartBeatFragSet: pp->setHeartBeatFragSet(value.boolValue()); break;
        case FIELD_lastFragmentNum: pp->setLastFragmentNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_InfoDestinationSet: pp->setInfoDestinationSet(value.boolValue()); break;
        case FIELD_destinationNodeId: pp->setDestinationNodeId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destinationParticipantId: pp->setDestinationParticipantId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_InfoTimestampSet: pp->setInfoTimestampSet(value.boolValue()); break;
        case FIELD_timestamp: pp->setTimestamp(value.doubleValue()); break;
        case FIELD_NackFragSet: pp->setNackFragSet(value.boolValue()); break;
        case FIELD_fragmentNumberStateBase: pp->setFragmentNumberStateBase(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_fragmentNumberStateNbrBits: pp->setFragmentNumberStateNbrBits(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_fragmentNumberBitmap: pp->setFragmentNumberBitmap(i,value.boolValue()); break;
        case FIELD_GeneralFragmentSize: pp->setGeneralFragmentSize(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_publisherSendTime: pp->setPublisherSendTime(value.doubleValue()); break;
        case FIELD_writerSendTime: pp->setWriterSendTime(value.doubleValue()); break;
        case FIELD_readerReceiveTime: pp->setReaderReceiveTime(value.doubleValue()); break;
        case FIELD_fragCount: pp->setFragCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sentFragments: pp->setSentFragments(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RtpsInetPacket'", field);
    }
}

const char *RtpsInetPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr RtpsInetPacketDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    RtpsInetPacket *pp = omnetpp::fromAnyPtr<RtpsInetPacket>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void RtpsInetPacketDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    RtpsInetPacket *pp = omnetpp::fromAnyPtr<RtpsInetPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RtpsInetPacket'", field);
    }
}

}  // namespace inet

namespace omnetpp {

}  // namespace omnetpp

