/*
 *
 */

#include <string.h>
#include <omnetpp.h>

#include "rtpsPublisherApp.h"
#include "./../messages/Sample_m.h"

using namespace omnetpp;

Define_Module(RtpsPublisherApp);

RtpsPublisherApp::~RtpsPublisherApp()
{
   cancelAndDelete(selfEvent);
}


void RtpsPublisherApp::initialize()
{
    // Parse the parameters
    samplePeriod = par("samplePeriod");
    sampleSize = par("sampleSize");
    offset = par("offset");
    jitter = par("jitter");

    c = 0;

    // Create and schedule the self_event for periodic data transmission
    selfEvent = new cMessage("event");
    scheduleAt(simTime(),selfEvent);
}

void RtpsPublisherApp::handleMessage(cMessage *msg)
{
    // Create and send a new sample
    Sample* sample = new Sample();
    sample->setSize(sampleSize);

    if (c == 0)
    {
        c = -1;
        // consider start offset
        scheduleAt(simTime() + offset, selfEvent);
    }
    else
    {
        send(sample, "AppOut");
        // Trigger the next self-event after the defined sample period

        double tmp = 0.0;
        if(jitter.dbl() != 0)
        {
            double rd = ((double)rand()/(double)RAND_MAX);
            tmp = -jitter.dbl() + rd * 2 * jitter.dbl();
        }
        simtime_t jitterOffset = SimTime(tmp);

        scheduleAt(simTime()+ samplePeriod/1000 + jitterOffset/1000, selfEvent);
    }
}
