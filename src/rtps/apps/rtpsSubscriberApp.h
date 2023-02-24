/*
 *
 */

#ifndef SRC_RTPS_RTPSSUBSCRIBERAPP_H_
#define SRC_RTPS_RTPSSUBSCRIBERAPP_H_


#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;


class RtpsSubscriberApp : public cSimpleModule
{
  private:
    // ---- Internal parameters ----

  public:
    RtpsSubscriberApp() {}
    virtual ~RtpsSubscriberApp();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};


#endif // SRC_RTPS_RTPSSUBSCRIBERAPP_H_
