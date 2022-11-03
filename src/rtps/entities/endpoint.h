/*
 * endpoint.h
 *
 *  Created on: Jan 28, 2020
 *      Author: Jonas Peeck
 */

#ifndef RTPS_ENTITIES_ENDPOINT_H_
#define RTPS_ENTITIES_ENDPOINT_H_

/* WirelessRTPSRetransmissions includes */
#include "./../messages/RtpsInetPacket_m.h"

#include <omnetpp.h>

using namespace omnetpp;
using namespace inet;

//
// The Endpoint class is the super class of the reader and writer classes.
// It integrates general methods and attributes applicable to both.
//
class Endpoint{

public:

    unsigned int entityId;
    unsigned int entityKind;

    Endpoint(){}
    ~Endpoint(){}

    /*
     * This method returns the analysis module in order to send all received and send messages to it for performance analysis.
     *
     * @param parent module of this module
     * @return analysis module
     */
    cModule* getAnalysisModule(cModule* parent);

    /*
     * This method sets the InfoDestination flag in the given message to true.
     *
     * @param rtpsMsg relevant message
     */
    void setGuid(RtpsInetPacket* rtpsMsg);

    /*
     * This method calculates the message size of an rtps message in order to model realistic message sizes.
     * Therefore it checks if certain messages are set.
     *
     * @param rtpsMsg message whose size shall be determined
     */
    void calculateRtpsMsgSize(RtpsInetPacket* rtpsMsg);

protected:

private:


};

#endif /* RTPS_ENTITIES_ENDPOINT_H_ */
