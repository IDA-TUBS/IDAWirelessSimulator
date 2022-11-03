//
// Copyright (C) 2016 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

package src.inet_ref;

import inet.queueing.contract.IPacketQueue;
import inet.linklayer.base.MacProtocolBase;
import inet.linklayer.contract.IMacProtocol;


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
module CsmaCaMac_mod extends MacProtocolBase like IMacProtocol
{
    submodules:
        queue: <default("DropTailQueue")> like IPacketQueue {
            parameters:
                packetCapacity = default(-1);
                @display("p=100,100;q=l2queue");
        }
}