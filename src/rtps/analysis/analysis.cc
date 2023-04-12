/*
 *
 */


#include <omnetpp.h>
#include "analysis.h"

sampleVectorMap RTPSAnalysis::transmittedSamplesByAppId;

fragmentVectorMap RTPSAnalysis::transmittedFragmentsByAppId;

appToSampleMap RTPSAnalysis::countMap;

sampleEfficiencyVectorMap RTPSAnalysis::sampleEfficienciesByAppId;
