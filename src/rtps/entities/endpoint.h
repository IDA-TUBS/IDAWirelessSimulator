/*
 *
 */

#ifndef RTPS_ENTITIES_ENDPOINT_H_
#define RTPS_ENTITIES_ENDPOINT_H_

#include "sampleFragment.h"
#include "cacheChange.h"
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
    /// entity ID of instance
    unsigned int entityId;
    /// entity kind of instance
    unsigned int entityKind;

    /*
     * empty default constructor
     */
    Endpoint(){};
    /*
     * empty default destructor
     */
    ~Endpoint(){};

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
     * Method building constructs a complete RTPS inet packet from any sample fragment
     *
     * @param sampleFragment pointer to sample fragment that shall be packaged
     * @param entityID entity ID of the transmitting entity (writer)
     * @param fragmentSize actual size of the fragment
     * @param addr destination address
     * @param appId application id used for stream differentiation
     * @return sample fragment packaged in as an RtpsInetPacket
     */
    RtpsInetPacket* createRtpsMsgFromFragment(SampleFragment* sampleFragment, unsigned int entityID, unsigned int fragmentSize, std::string addr, unsigned int appId);

    /*
     * Method for adding a HBFrag to an already existing data frag msg
     *
     * @param rtpsMsg message to which to append the heartbeat frag
     * @param highestFragmentNumber highest yet transmitted fragment number
     */
    void addHBFrag(RtpsInetPacket* rtpsMsg, unsigned int highestFragmentNumber);


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
