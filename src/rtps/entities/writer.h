/*
 *
 */

#ifndef RTPS_ENTITIES_WRITER_H__
#define RTPS_ENTITIES_WRITER_H__


#include <omnetpp.h>
#include "./../messages/rtpsInetPacket_m.h"

#include "endpoint.h"
#include "changeForReader.h"
#include "sampleFragment.h"


class Writer : public cSimpleModule, Endpoint {
    public:
        /*
         * empty default constructor
         */
        Writer() {};

        /*
         * empty default destructor
         */
        virtual ~Writer();

    private:
        // ==============================
        // ==== rtps parametrization ====
        // ==============================

        /// Fragment size
        unsigned int fragmentSize;
        /// Sample latency deadline
        simtime_t deadline;
        /// nack suppression duration
        simtime_t nackSuppressionDuration;
        /// max number of cache changes to be stored simultaneously
        unsigned int sizeCache;

        /// number of subscribers/readers
        unsigned int numReaders;

        /// application ID
        unsigned int appID;

        /// TODO destination address(es)


        // ===========================
        // ==== protocol entities ====
        // ===========================

        std::vector<ReaderProxy*> *matchedReaders;
        std::list<SampleFragment*> sendQueue;


        // =======================
        // ==== self messages ====
        // =======================
        /// TODO description
        cMessage *sendEvent;
        /// TODO description
        cMessage *hbEvent;
        /// TODO description
        cMessage *nextTimeoutEvent;
        /// TODO description
        cMessage *scheduleEvent;


    protected:
        /*
         * Method that is triggered according to some schedule. At the end, sends a packaged sample fragment down towards the UDP/IP stack
         *
         * @return true on success, else false
         */
        bool sendMessage();

        /*
         * Method that is used for "default" RTPS implementation to mimic only sending a heartbeart message - without any data attached
         *
         * @return true on success, else false
         */
        bool sendHeartbeatMsg();

        /*
         * Method for reacting to NackFrags received from readers
         *
         * @param nackFrag message containing the ack/nack bitmap
         */
        void handleNackFrag(RtpsInetPacket* nackFrag);

//        /*
//         * Method for evaluating whether a reader has received all fragments of a sample
//         *
//         * @param rp pointer to ReaderProxy corresponding to the reader whose status of the current sample shall be checked
//         * @return true if complete, else false
//         */
//        bool checkSampleCompletenessAtReader(ReaderProxy* rp);




        /*
         * Method building constructs a complete RTPS inet packet from any sample fragment
         *
         * @param sampleFragment pointer to sample fragment that shall be packaged
         * @return sample fragment packaged in as an RtpsInetPacket
         */
        RtpsInetPacket* create_rtps_msg_from_fragment(SampleFragment* sampleFragment);

        /*
         * Method for selecting which fragment (missing at a specific reader) to transmit next
         *
         * @param rp pointer to reader proxy used for fragment selection
         * @return the sample fragment to be transmitted next
         */
        virtual SampleFragment* select_next_fragment(ReaderProxy *rp);


        /*
         * Overwritten method, initializes modules after its instantiation
         */
        virtual void initialize() override;

        /*
         * Overwritten method, trigger either on arrival of a new sample (from app) or by self-messages
         *
         * @param msg each incoming (external or self) message is interpreted as a stimulus
         */
        virtual void handleMessage(cMessage *msg) override;
        /*
         * Overwritten method, called at the end of the simulation
         */

        virtual void finish() override;

};

#endif //RTPS_ENTITIES_WRITER_H__
