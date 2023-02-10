/*
 *
 */

#ifndef RTPS_ANALYSIS_H_
#define RTPS_ANALYSIS_H_

#include <omnetpp.h>
#include <map>
#include <algorithm>

#include "./../entities/changeForWriter.h"
#include "./../entities/changeForReader.h"

typedef std::vector<unsigned int> sampleVector;
typedef std::map<unsigned int, sampleVector> sampleVectorMap;

typedef std::vector<unsigned int> fragmentVector;
typedef std::map<unsigned int, fragmentVector> fragmentVectorMap;

class RTPSAnalysis
{
  public:
    /// Vector for storing sample latencies at the reader
    cOutVector sampleLatenciesVector;
    // Vector for storing the deadline violation rate at the reader
    cOutVector sampleViolationRateVector;
    // Vector for storing the deadline violation rate at the reader
    cOutVector missingSamplesVector;
    // Vector for storing the first time a sample begins being transmitted
    cOutVector sampleTxStartVector;
    /// Vector for storing the sequence numbers successfully received in time at the reader
    std::vector<unsigned int> completeSamples;
    /// Vector for storing a fragment trace at the reader
    std::vector<unsigned int> fragmentTrace;
    /// variable for storing the deadline violation rate at a reader
    double violationRate;
    /// variable for storing the frame error rate at a reader
    double frameErrorRate;


    /// count samples (at all readers) marked as complete prior to the deadline elapsing
    unsigned int counterCompleteSamples;
    /// count samples (at all readers) that violated their respective deadline
    unsigned int counterIncompleteSamples;
    /// combined deadline violation rate as 'measured' at the writer, includes all readers in case of multicast
    double combinedViolationRate;
    /// Vector storing a list of all sample sequence numbers transmitted by a writer based on the appID
    static sampleVectorMap transmittedSamplesByAppId;
    /// Vector storing a list of all fragment numbers transmitted by a writer based on the appID
    static fragmentVectorMap transmittedFragmentsByAppId;



    /*
     * empty default constructor
     */
    RTPSAnalysis()
    {
        sampleLatenciesVector.setName("sampleLatencies");
        sampleTxStartVector.setName("sampleTxStartpoints");
        missingSamplesVector.setName("missingSamples");
        sampleViolationRateVector.setName("sampleViolationRateVector");

        counterCompleteSamples = 0;
        counterIncompleteSamples = 0;
    };

    /*
     * default destructor
     */
    ~RTPSAnalysis()
    {
        transmittedSamplesByAppId.clear();
        completeSamples.clear();
    };

    /*
     * record timestamp for first tx of a new sample
     *
     * @param seqNum sequence number of the given sample
     */
    void recordSampleTxStart(unsigned int seqNum)
    {
        sampleTxStartVector.record(seqNum);
    };

    /*
     * record successful sample transmission
     * stores sample latency as well as the sequence number of the sample
     *
     * @param change pointer to relevant ChangeForWriter object, used to determine latency
     */
    void recordSampleLatency(ChangeForWriter* change)
    {
        if(std::find(completeSamples.begin(), completeSamples.end(), change->sequenceNumber) != completeSamples.end())
        {
            // avoid sample being considered multiple times
            return;
        }
        simtime_t tmp = simTime() - change->arrivalTime;
        unsigned int sn = change->sequenceNumber;
        simtime_t x = simTime();
        double t = x.dbl();
        double arrivalTime = change->arrivalTime.dbl();
        double latency = tmp.dbl();

        sampleLatenciesVector.record(latency);
        completeSamples.push_back(change->sequenceNumber);
    };

    /*
     * record sample with deadline violations
     *
     * @param change pointer to relevant ChangeForWriter object
     */
    void recordDeadlineViolation(ChangeForWriter* change)
    {
        missingSamplesVector.record(change->sequenceNumber);
    };

    /*
     * record successful fragment transmission
     * stores fragment number
     *
     * @param fragmentNumber fragment number
     */
    void recordFragmentReception(unsigned int fragmentNumber)
    {
        fragmentTrace.push_back(fragmentNumber);
    };

    /*
     * if the cOutVector is empty, it will not occur in the exported
     * to solve this issue add a noticeable identifier (NaN) to the list
     * to be able to identify those situations
     */
    void finishSampleLatencyRecording()
    {
        if(sampleLatenciesVector.getValuesReceived() == 0)
        {
            double latency = std::nan("1");
            sampleLatenciesVector.record(latency);
        }
    }

    /*
     * calculate deadline violation rate based on received samples and the
     * number of samples transmitted via the given appId
     *
     * @param appId unique application identifier
     */
    void calculateViolationRate(unsigned int appId)
    {
//        EV << "sent by writer: " << transmittedSamplesByAppId[appId].size() << "\n";
//        EV << "num received: " << completeSamples.size() << "\n";
        EV << "[Reader] Complete Samples: " << (completeSamples.size()) << ",\tSend Samples: " << (transmittedSamplesByAppId[appId].size()) << endl;

        violationRate = 1 - (double(completeSamples.size()) / double(transmittedSamplesByAppId[appId].size()));
    };


    /*
     * calculate frame error rate based on received fragments and the
     * number of fragments transmitted via the given appId
     *
     * @param appId unique application identifier
     */
    void calculateFER(unsigned int appId)
    {
        frameErrorRate = 1 - (double(fragmentTrace.size()) / double(transmittedFragmentsByAppId[appId].size()));
    };

    /*
     * calculate deadline violation rate as perceived by the writer
     */
    void calculateCombinedViolationRate()
    {
        EV << "[Writer] Complete Samples: " << counterCompleteSamples << ",\tIncomplete Samples: " << counterIncompleteSamples << endl;
        combinedViolationRate = double(counterIncompleteSamples) / (double(counterIncompleteSamples) + double(counterCompleteSamples));
    };

    /*
     * depending on the sample period it can happen that there are still valid
     * samples that have not been transmitted to all readers remaining. In that
     * case, a deadline violation would be recorded even if not happening. Here,
     * we correct for such scenarios
     *
     * @param appID unique application identifier
     */
    void handleIncompleteButValidSamples(unsigned int appId)
    {
        // remove the last samples from the analysis vector to account for still valid samples
        transmittedSamplesByAppId[appId].resize(transmittedSamplesByAppId[appId].size() - 1);
    }

    /*
     * increment the counter used for tracking number of sample perceived as complete by the writer
     */
    void incrementCompleteCounter()
    {
        counterCompleteSamples++;
    }

    /*
     * increment the counter used for tracking number of sample perceived as incomplete by the writer
     */
    void incrementIncompleteCounter()
    {
        counterIncompleteSamples++;
    }

    /*
     * register appId at the analysis class to prepare vector for storing sample
     * sequence numbers transmitted by the writer - a.k.a. samples that should
     * have been received by the reader
     *
     * @param appId unique application identifier
     */
    void registerAppID(unsigned int appId)
    {
        transmittedSamplesByAppId.insert(sampleVectorMap::value_type(appId, sampleVector()));
        // or
//        transmittedSamplesByAppId[appId] = sampleVector();

        transmittedFragmentsByAppId.insert(sampleVectorMap::value_type(appId, sampleVector()));
    };

    /*
     * method for writer to call if a new sample is being transmitted
     * stores sequence number based on appId
     *
     * @param appId unique application identifier
     * @param change pointer to relevant new CacheChange object
     */
    void addSample(unsigned int appId, CacheChange* change)
    {
        transmittedSamplesByAppId[appId].push_back(change->sequenceNumber);
    };

    /*
     * method for writer to call if a new fragment is being transmitted
     * stores sequence number based on appId
     *
     * @param appId unique application identifier
     * @param sf pointer to relevant new SampleFragment object
     */
    void addFragment(unsigned int appId, SampleFragment* sf)
    {
        transmittedFragmentsByAppId[appId].push_back(sf->fragmentStartingNum);
    };

};


#endif //RTPS_ANALYSIS_H_
