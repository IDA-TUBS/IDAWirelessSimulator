/*
 *
 */

#ifndef RTPS_HELPER_RTPSMESSAGEDISPATCHER_H_
#define RTPS_HELPER_RTPSMESSAGEDISPATCHER_H_

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class RtpsMessageDispatcher : public cSimpleModule
{
  public:
    RtpsMessageDispatcher() {}
    virtual ~RtpsMessageDispatcher();

  protected:
    virtual void initialize() override;
   // virtual void handleMessage(cMessage *msg, cGate *inGate);
    virtual void handleMessage(cMessage *msg) override;
};

#endif /* RTPS_HELPER_RTPSMESSAGEDISPATCHER_H_ */
