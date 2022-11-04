/*
 *
 */

#ifndef RTPS_RTPS_H_
#define RTPS_RTPS_H_

#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;

class Rtps : public cSimpleModule
{

  public:

    // The counter for the entity IDs
    unsigned int entityIdCounter;

    Rtps() {}
    virtual ~Rtps();

    // Create entity id number for dispatching
    unsigned int getNextEntityId(){
        return entityIdCounter++;
    }
  protected:


    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};




#endif /* RTPS_RTPS_H_ */
