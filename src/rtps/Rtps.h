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

    // The counters for the entity IDs
    static unsigned int entityIdCounter;
    static std::vector<unsigned int> entityIdPerApp;

    // structure for mapping reader IDs to priorities - used for WiMEP protocol
    static std::vector<unsigned int> mapIdToPriority;

    // parameter used for disabling initialization of global entityId and priority map
    static bool init;

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

    /*
     * get parameter numberReadersPerApp
     *
     * @return numberReadersPerApp
     */
    unsigned int getMaxNumberOfReaders();

    /*
     * set store priority of reader based on the reader's entity ID
     *
     * @param readerID entity id of the reader calling the function
     * @param priority that shall be assigned to the reader
     */
    void setReaderPriority(unsigned int readerID, unsigned int priority);

    /*
     * returns a reader's priority
     *
     * @param readerID entity id of the reader which priority is being requested
     * @param priority (unsigned) integer corresponding to the priority of the reader
     */
    unsigned int getReaderPriority(unsigned int readerID);
  protected:


    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};




#endif /* RTPS_RTPS_H_ */
