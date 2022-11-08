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
    // total number of apps
    unsigned int numberApps;
    // maximum number of readers per App
    unsigned int numberReadersPerApp;

    // The counter for the entity IDs
    static unsigned int entityIdCounter;
    static std::vector<unsigned int> entityIdPerApp;

    Rtps() {}
    virtual ~Rtps();

    /*
     * Legacy function: Create entity id number for dispatching
     *
     * @return unique entity ID
     */
    unsigned int getNextEntityId();

    /*
     * Create entity id number based on AppID
     * so far only allows for a single writer per App!
     *
     * @param appID
     * @param writer boolean value signaling whether reader or writer
     * @return unique entity ID
     */
    unsigned int getNextEntityId(unsigned int appId, bool writer);
  protected:


    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};




#endif /* RTPS_RTPS_H_ */
