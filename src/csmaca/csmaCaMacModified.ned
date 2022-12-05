//
// Copyright (C) 2016 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


package src.csmaca;

import inet.queueing.contract.IPacketQueue;
import inet.linklayer.base.MacProtocolBase;
import inet.linklayer.contract.IMacProtocol;
import inet.linklayer.csmaca.CsmaCaMac;

//
// Implements an imaginary CSMA/CA-based MAC protocol with optional
// acknowledgements and a retry mechanism. With the appropriate settings,
// it can approximate basic 802.11b ad-hoc mode operation.
//
// Parameters include:
// - acknowledgements on/off
// - bit rate (this is used for both data and ACK frames)
// - protocol overhead: MAC header length, ACK frame length
// - backoff parameters: minimum/maximum contention window (in slots),
//   slot time, maximum retry count
// - timing: interval to wait before transmitting ACK frame (SIFS) and
//   before data frames in addition to the backoff slots (DIFS)
//
// When acknowledgements are turned off, the MAC assumes that its transmissions
// are successful, so no frame is ever retransmitted.
//
// When acknowledgements are turned on, the operation is as follows.
// On the receiver side, the change is quite simple: when the MAC correctly
// receives a data frame addressed to it, it responds with an ACK frame after
// a fixed-length gap (SIFS). If the originator of the data frame does not
// receive the ACK correctly within due time, it will initiate a
// retransmission. The contention window (from which the random backoff period is
// drawn) will be doubled for each retransmission until it reaches the maximum
// (and then it will stay constant for further retransmissions). After a given
// number of unsuccessful retries, the MAC will give up and discard the data
// frame, and will take the next data frame from the queue. The next frame
// will start with a clean slate (i.e. the contention window and the retry
// count will be reset). Giving up a frame will cause a link break signal
// to be emitted.
//
// Note: This module does not contain a duplicate detection algorithm in order
// to keep its code simple and accessible, so lost ACKs will cause duplicates
// in the stream of packets sent up to higher layers.
//
module CsmaCaMacModified extends CsmaCaMac like IMacProtocol
{
    parameters:
        
        double bitErrorRate = default(0);
        double ta = default(84.0);
        bool ta_enable = default(false);
        
        bool burstErrors = default(false);
        double geBurstParamP = default(0.0515509);
        double geBurstParamR = default(0.946863);
        
        bool dependentSignalPaths = default(false);

        @class(CsmaCaMacModified);
        @signal[linkBroken](type=inet::Packet);
        @statistic[linkBroken](title="link break"; source=linkBroken; record=count; interpolationmode=none);
        @statistic[packetDropIncorrectlyReceived](title="packet drop: incorrectly received"; source=packetDropReasonIsIncorrectlyReceived(packetDropped); record=count,sum(packetBytes),vector(packetBytes); interpolationmode=none);
        @statistic[packetDropNotAddressedToUs](title="packet drop: not addressed to us"; source=packetDropReasonIsNotAddressedToUs(packetDropped); record=count,sum(packetBytes),vector(packetBytes); interpolationmode=none);
        @statistic[packetDropQueueOverflow](title="packet drop: queue overflow"; source=packetDropReasonIsQueueOverflow(packetDropped); record=count,sum(packetBytes),vector(packetBytes); interpolationmode=none);
        @statistic[packetDropRetryLimitReached](title="packet drop: retry limit reached"; source=packetDropReasonIsRetryLimitReached(packetDropped); record=count,sum(packetBytes),vector(packetBytes); interpolationmode=none);
}

