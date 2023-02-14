/*
 *
 */

#include "reader.h"


/*
*
*/
void Reader::initialize()
{
    /// Initialize RTPS context
    appID = par("appID");

    /// get unique entity ID
    rtpsParent = dynamic_cast<Rtps*>(getParentModule());
    entityId = rtpsParent->getNextEntityId(appID, false);

    // process destination addresses
    const char *destAddrs = par("destAddresses");
    destinationAddresses = cStringTokenizer(destAddrs).asVector();

    deadline = par("deadline");

    sizeCache = par("historySize");

    writerProxy = new WriterProxy(this->sizeCache);

    responseDelay = par("readerResponseDelay");
}

/*
*
*/
void Reader::finish()
{
    // analysis related code
    RTPSAnalysis::calculateViolationRate(this->appID);
    RTPSAnalysis::calculateFER(this->appID);
    recordScalar("deadlineViolationRate", this->violationRate);
    sampleViolationRateVector.record(this->violationRate);
    RTPSAnalysis::finishSampleLatencyRecording();

    EV << "violation rate: "  << this->violationRate << endl;
    EV << "FER:  "  << this->frameErrorRate << endl;
}

void Reader::handleMessage(cMessage *message)
{
    if(message->isSelfMessage())
    {
        dealWithInternalMessage(message);
    }
    else
    {
        dealWithExternalMessage(message);
    }
}

/*
*
*/
void Reader::dealWithInternalMessage(cMessage* message)
{
    // self message for delaying answering (reader response delay)
    RtpsInetPacket *rtpsMessage = check_and_cast<RtpsInetPacket*>(message);
    send(rtpsMessage, "dispatcherOut");
}

/*
*
*/
void Reader::dealWithExternalMessage(cMessage* message)
{
    // Check message type
    if(dynamic_cast<RtpsInetPacket*>(message) != NULL)
    {
        RtpsInetPacket *rtpsMessage = check_and_cast<RtpsInetPacket*>(message);

        // first check whether appID is corresponding to the reader's appID
        if(rtpsMessage->getAppId() != this->appID)
        {
            return;
        }
        else if(rtpsMessage->getDataFragSet())
        {
            dealWithDataFragment(rtpsMessage);
        }
        else if(rtpsMessage->getHeartBeatFragSet())
        {
            dealWithHeartBeatFragment(rtpsMessage);
        }
    }
}

/*
*
*/
void Reader::dealWithDataFragment(RtpsInetPacket* rtpsMessage)
{
    EV << "[RTPSReader] Data Fragment arrived at: " << simTime() << "\n";
    // first check whether existing samples in history expired or are still valid
    checkSampleLiveliness();

    // if DataFrag, update cache
    // create new change
    auto change = new CacheChange(rtpsMessage->getWriterSN(), rtpsMessage->getSampleSize(), rtpsMessage->getFragmentSize(), rtpsMessage->getPublisherSendTime());
    writerProxy->addChange(*change); // only adds change if new, else WriterProxy does nothing here

    // mark fragment as received
    unsigned int fn = rtpsMessage->getFragmentStartingNum();
    writerProxy->updateFragmentStatus(RECEIVED, change->sequenceNumber, fn);

    bool complete = writerProxy->checkSampleCompleteness(change->sequenceNumber);

    // analysis related code
    RTPSAnalysis::recordFragmentReception(fn);
    if(complete)
    {
        RTPSAnalysis::recordSampleLatency(writerProxy->getChange(change->sequenceNumber));
    }
    delete rtpsMessage;
}

/*
*
*/
void Reader::dealWithHeartBeatFragment(RtpsInetPacket* rtpsMessage)
{
    // if HB or HBFrag
    // respond with NackFrag
    EV << "[RTPSReader] Heart Beat Message arrived at: " << simTime() << "\n";
    RtpsInetPacket* nackFrag = generateNackFrag(rtpsMessage);
    if(nackFrag != NULL)
    {
        sendMessage(nackFrag);
    }
    else
    {
        EV << "[RTPSReader] ERROR Message is nullpointer" << "\n";
    }
    //delete nackFrag;

    delete rtpsMessage;
}

/*
*
*/
void Reader::checkSampleLiveliness()
{
    if(writerProxy->getCurrentChange() == nullptr)
    {
        return;
    }

    std::vector<unsigned int> deprecatedSNs;
    // check liveliness of samples in history cache, if deadline expired remove sample from cache and ReaderProxies
    // assumption: samples transmitted in the right order and each sample has the same deadline, hence if the first
    // sample is not expired yet, all other example did not expire too
    while(1)
    {
        auto change = writerProxy->getCurrentChange();

        if(!change->isValid(this->deadline))
        {
            if(!writerProxy->checkSampleCompleteness(change->sequenceNumber))
            {
                recordDeadlineViolation(change);
            }
            deprecatedSNs.push_back(change->sequenceNumber);
            writerProxy->removeChange(change->sequenceNumber);
            if(writerProxy->getSize() == 0)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
}

/*
*
*/
void Reader::sendMessage(RtpsInetPacket* rtpsMessage)
{
    // wait for specified time (corresponding to reader response delay) before actually sending the NackFrag back to the writer
    scheduleAt(simTime() + responseDelay, rtpsMessage);
    EV << "[RTPSReader] Schedule NACK Message for simtime: "  << (simTime() + responseDelay) << "\n";
}


RtpsInetPacket* Reader::generateNackFrag(RtpsInetPacket* heartBeatMessage)
{
    // Create message instance
    RtpsInetPacket* nackFrag = new RtpsInetPacket();

    // set destination address
    if(destinationAddresses.size() > 1)
    {
        throw cRuntimeError("Handling of multiple addresses not supported by reader yet!");
    }
    std::string addr = destinationAddresses[0];
    nackFrag->setDestinationAddress(addr.c_str());

    // Create header
    this->setGuid(nackFrag);

    // Create NackFrag submessage
    nackFrag->setNackFragSet(true);
    nackFrag->setReaderId(entityId);
    nackFrag->setAppId(this->appID);

    int sequenceNumber = heartBeatMessage->getWriterSN();
    int startFragNum = heartBeatMessage->getFragmentStartingNum();
    int endFragNum = heartBeatMessage->getLastFragmentNum();

    // Define the start and end points of the data field - Ack from the lowest
    // non received fragment number ! Lower frag_Sns have been received!
    // Start number. Minimum Bitmap size is 8 (if possible)


    // relevant history cache entry in WriterProxy
    ChangeForWriter* cfw = writerProxy->getChange(sequenceNumber);
    if(!cfw)
    {
        // TODO implement properly: reader shall answer with empty bitmap - all fragments missing
        // TODO can be done by just creating the change if it does not exist yet or just respond with "empty" bitmap
        delete nackFrag;
        return nullptr;
    }
    // sample fragment array
    auto frags = cfw->getFragmentArray();

    // The start sequence number is the lowest fragment sequence number, which has not been received yet.
    int lowestUnackedMsg = 0;
    for(int i = 0; i < cfw->numberFragments; i++)
    {
        lowestUnackedMsg = i;

        if(!frags[i]->received)
        {
            break;
        }
    }

    // check if to decrease the start_frag_sn
    if(startFragNum > lowestUnackedMsg)
    {
        startFragNum = lowestUnackedMsg;
    }

    // Get the highest unacknowledged fragment number within the already sent fragments
    int highestUnackedMsg = 0;
    for(int i = 0; i < cfw->numberFragments; i++)
    {
        if(!frags[i]->received)
        {
            highestUnackedMsg = i;
        }
        if(i == endFragNum)
        {
            break;
        }
    }

    if(highestUnackedMsg > endFragNum)
    {
        endFragNum = highestUnackedMsg;
    }

    // Last fill the bitmap up to the next 32 bit
    int nbrPadBits = 32-((endFragNum - startFragNum + 1)%32);

    // use space below
    if(startFragNum >= nbrPadBits)
    {
        startFragNum = startFragNum - nbrPadBits;
        nbrPadBits = 0;
    }
    else
    {
        nbrPadBits = nbrPadBits - startFragNum;
        startFragNum = 0;
    }

    // use space up
    if(heartBeatMessage->getLastFragmentNum() - endFragNum >= nbrPadBits)
    {
        startFragNum = startFragNum + nbrPadBits;
        nbrPadBits = 0;
    }
    else
    {
        nbrPadBits = nbrPadBits - (heartBeatMessage->getLastFragmentNum()- endFragNum);
        endFragNum = heartBeatMessage->getLastFragmentNum();
    }

    nackFrag->setFragmentNumberStateNbrBits(endFragNum - startFragNum + 1);
    nackFrag->setFragmentNumberStateBase(startFragNum);

    // Pass the values to the bitmap
    for(int i = 0; i < nackFrag->getFragmentNumberStateNbrBits(); i++)
    {
        auto fragStatus = frags[i+startFragNum]->received;
        nackFrag->setFragmentNumberBitmap(i, fragStatus);
    }
    nackFrag->setWriterSN(sequenceNumber);

    // used for analysis at the MAC layer:
    nackFrag->setSendTime(simTime());

    // Finally, calculate the overall rtps message size
    nackFrag->setInfoDestinationSet(false);

    nackFrag = calculateRtpsMessageSize(nackFrag);

    return nackFrag;
}

/*
 * Function from Endpoint because here a return is required
 */
RtpsInetPacket* Reader::calculateRtpsMessageSize(RtpsInetPacket* rtpsMessage)
{
    int currentSize = 0;
    /* Add the header size */
    currentSize+=20;

    /* DataFrag */
    if(rtpsMessage->getDataFragSet())
    {
        currentSize += 36;
        currentSize += rtpsMessage->getFragmentSize();
    }

    /* HeartBeatFrag */
    if(rtpsMessage->getHeartBeatFragSet())
    {
        currentSize += 28;
    }

//    if(rtpsMsg->getMapEnabledReaders())
//    {
//        currentSize += 4;
//    }

    /* InfoDestination */
    if(rtpsMessage->getInfoDestinationSet())
    {
        currentSize += 16;
    }

    /* InfoTimestamp */
    if(rtpsMessage->getInfoTimestampSet())
    {
        currentSize += 12;
    }

    /* NackFrag */
    if(rtpsMessage->getNackFragSet())
    {
        int bitmapSizeBytes = (rtpsMessage->getFragmentNumberStateNbrBits()+7)/8;
        int int32AllignedBytes = ((bitmapSizeBytes + 3)/4)*4;
        currentSize += 32 + int32AllignedBytes;
    }

    rtpsMessage->setRtpsMessageSize(currentSize);

    return rtpsMessage;
}
