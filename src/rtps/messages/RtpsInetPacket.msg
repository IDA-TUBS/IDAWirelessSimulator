//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

//cplusplus {{
//#include "inet/common/packet/Packet.h"
//}}
import inet.common.packet.Packet;
//
// TODO generated message class
//
namespace inet;
packet RtpsInetPacket extends Packet {
    
    // ---- Protocol related content ----
    // General
    int payloadSize;
    int protocolOverhead;
    int rtpsMessageSize;
    
    
    // ---------- Header ----------
    int sourceNodeId;
    int sourceParticipantId;
	string destinationAddress;
	int appId;
        
    // AckNack
    bool AckNackSet;
    int  readerId;
    int  writerId;
    int  readerSNStateBase;
    int  readerSNStateNbrBits;
    bool readerSNStateBitmap[256];
        
    // Data
    bool DataSet;
    // readerId (s.o.)
    // writerId (s.o.)
    int writerSN;
    int sampleSize;
    
    // DataFrag
    bool DataFragSet;
    // readerId (s.o.)
    // writerId (s.o.)
    // writerSN (s.o.)
    int fragmentStartingNum;    // Starting Fragment Number of the fragments in serialized data
    int fragmentsInSubmessage;  //Anzahl der fragmente in der DataFrag-Submessage
    int fragmentSize;
    // sampleSize (s.o.)
    
    
    // HeartBeat
    bool HeartBeatSet;
    // readerId (s.o.)
    // writerId (s.o.)
    int firstSN;
    int lastSN;
    

    // HeartBeatFrag
    bool HeartBeatFragSet;
    // readerId (s.o.)
    // writerId (s.o.)
    int lastFragmentNum;
    
    // InfoDestination
    bool InfoDestinationSet;
    int destinationNodeId;
    int destinationParticipantId;
    
    // InfoTimestamp
    bool InfoTimestampSet;
    simtime_t timestamp;
    
    // NackFrag
    bool NackFragSet;
    // readerId (s.o.)
    // writerId (s.o.)
    // writerSN (s.o.)    
    int  fragmentNumberStateBase;
    int  fragmentNumberStateNbrBits;
    bool fragmentNumberBitmap[256];
    
    int GeneralFragmentSize;
       
    // ---- Analysis related content ----
    simtime_t publisherSendTime;
    
    // Raw transmission time over the network (reader_receive_time - writer_send_time)
    simtime_t sendTime;
    simtime_t receiveTime;
    
        // Efficency
    int fragCount;
    int sentFragments;
    
}

