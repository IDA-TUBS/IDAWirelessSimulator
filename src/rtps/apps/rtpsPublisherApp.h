/*
 *
 */

#ifndef SRC_RTPS_RTPSPUBLISHERAPP_H_
#define SRC_RTPS_RTPSPUBLISHERAPP_H_

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class RtpsPublisherApp : public cSimpleModule
{
  private:

    /// The sample period
    simtime_t samplePeriod;
    /// application start offset
    simtime_t offset;
    /// application start offset
    simtime_t jitter;

    /// The size of a sample in byte
    int sampleSize;

    /// Self-message to trigger the next sample
    cMessage *selfEvent;

    /// misc
    int c;
    /// sequence number counter
    unsigned int sequenceNumber;

  public:
    RtpsPublisherApp() {}
    virtual ~RtpsPublisherApp();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};


#endif // SRC_RTPS_RTPSPUBLISHERAPP_H_
