/*
 *
 */


#include <omnetpp.h>
#include "Rtps.h"

using namespace omnetpp;

Define_Module(Rtps);

unsigned int Rtps::entityIdCounter;
std::vector<unsigned int> Rtps::entityIdPerApp;

Rtps::~Rtps()
{
   // cancelAndDelete(selfMsg);
}

void Rtps::initialize()
{
    numberApps = par("nbrApps");
    numberReadersPerApp = par("nbrReadersPerApp");

    entityIdCounter = 0;


    for(int i = 0; i < numberApps; i++)
    {
        // reader entity IDs per App: [i*numberReadersPerApp+1, (i+1)*numberReadersPerApp -1]
        entityIdPerApp.push_back(i*numberReadersPerApp + 1);
    }
}


unsigned int Rtps::getNextEntityId(){
    return entityIdCounter++;
}

unsigned int Rtps::getNextEntityId(unsigned int appId, bool writer)
{
    if(appId > numberApps-1)
    {
        throw omnetpp::cRuntimeError("App ID exceeds valid range: [0,%d]", numberApps-1);
    }

    if(writer)
    {
        // writer entity IDs per App: appID*numberReadersPerApp
        return appId*numberReadersPerApp;
    }
    else
    {
        // reader entity IDs per App: [appID*numberReadersPerApp+1, (appID+1)*numberReadersPerApp -1]
        if(entityIdPerApp[appId]%numberReadersPerApp == numberReadersPerApp)
        {
            throw omnetpp::cRuntimeError("Cannot register more than %d readers per app", numberReadersPerApp);
        }
        unsigned int id = entityIdPerApp[appId];
        entityIdPerApp[appId]++;
        return id;
    }
}

unsigned int Rtps::getMaxNumberOfReaders()
{
    return this->numberReadersPerApp;
}


void Rtps::handleMessage(cMessage *msg)
{
}
