/*
 *
 */


#include <omnetpp.h>
#include "Rtps.h"

using namespace omnetpp;

Define_Module(Rtps);

unsigned int Rtps::entityIdCounter;
std::vector<unsigned int> Rtps::entityIdPerApp;
std::vector<unsigned int> Rtps::mapIdToPriority;
bool Rtps::init = true;

Rtps::~Rtps()
{
   init = true;

   Rtps::entityIdPerApp.clear();
   Rtps::mapIdToPriority.clear();
}

void Rtps::initialize()
{
    numberApps = par("nbrApps");
    numberReadersPerApp = par("nbrReadersPerApp");

    if(init)
    {
        entityIdCounter = 0;


        for(int i = 0; i < numberApps; i++)
        {
            // reader entity IDs per App: [i*numberReadersPerApp+1, (i+1)*numberReadersPerApp -1]
            entityIdPerApp.push_back(i*numberReadersPerApp + 1);
        }

        for(int i = 0; i < numberApps; i++)
        {
            //create list (vector) containing all possible reader IDs
            mapIdToPriority.push_back(std::numeric_limits<unsigned int>::max()); // ID reserved for writer
            for(int i = 0; i < numberReadersPerApp; i++)
            {
                // fill in placeholders for reader priorities
                mapIdToPriority.push_back(0);
            }
        }

        init = false;
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

void Rtps::setReaderPriority(unsigned int readerID, unsigned int priority)
{
    mapIdToPriority[readerID] = priority;
}

unsigned int Rtps::getReaderPriority(unsigned int readerID)
{
    return mapIdToPriority[readerID];
}


void Rtps::handleMessage(cMessage *msg)
{
}
