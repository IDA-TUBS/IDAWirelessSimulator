/*
 *
 */

#include "reader.h"

using namespace omnetpp;

Define_Module(Reader);

void Reader::initialize()
{
    /// Initialize RTPS context
    appID = par("appID");
    rtpsParent = dynamic_cast<Rtps*>(getParentModule());
    entityId = rtpsParent->getNextEntityId(appID, false);

    // process destination addresses
    const char *destAddrs = par("destAddresses");
    destinationAddresses = cStringTokenizer(destAddrs).asVector();

    sizeCache = par("historySize");

    writerProxy = new WriterProxy(this->sizeCache);

    responseDelay = 0;
}


void Reader::finish()
{

}

void Reader::handleMessage(cMessage *msg)
{
    // Check message type
    if((dynamic_cast<RtpsInetPacket*>(msg) != NULL) && !(msg->isSelfMessage()))
    {
        RtpsInetPacket *rtpsMsg = check_and_cast<RtpsInetPacket*>(msg);

        // first check whether appID is corresponding to the reader's appID
        if(rtpsMsg->getAppId() != this->appID)
        {
            return;
        }

        if(rtpsMsg->getDataFragSet())
        {
            // if DataFrag, update cache
            // create new change
            auto change = new CacheChange(rtpsMsg->getWriterSN(), rtpsMsg->getSampleSize(), rtpsMsg->getFragmentSize(), rtpsMsg->getArrivalTime());
            writerProxy->addChange(*change); // only adds change if new, else WriterProxy does nothing here

            // mark fragment as received
            unsigned int fn = rtpsMsg->getFragmentStartingNum();
            writerProxy->updateFragmentStatus(RECEIVED, change->sequenceNumber, fn);

            writerProxy->checkSampleCompleteness(change->sequenceNumber);
        }

        if(rtpsMsg->getHeartBeatFragSet())
        {
            // if HB or HBFrag
            // respond with NackFrag
            auto nackFrag = generateNackFrag(rtpsMsg);
            sendMessage(nackFrag);
        }
    }

    if (msg->isSelfMessage()) {
        // self message for delaying answering (reader response delay)
        RtpsInetPacket *rtpsMsg = check_and_cast<RtpsInetPacket*>(msg);
        send(rtpsMsg, "dispatcher_out");
    }

    delete msg;
}

void Reader::sendMessage(RtpsInetPacket* rtpsMsg)
{
    // wait for specified time (corresponding to reader response delay) before actually sending the NackFrag back to the writer
    scheduleAt(simTime() + responseDelay, rtpsMsg);
}


RtpsInetPacket* Reader::generateNackFrag(RtpsInetPacket* hb)
{
    //Create message instance
    RtpsInetPacket* nackFrag = new RtpsInetPacket();

    // set destination address
    if(destinationAddresses.size() > 1)
    {
        throw cRuntimeError("Handling of multiple addresses not implemented yet!");
    }
    std::string addr = destinationAddresses[0];
    nackFrag->setDestinationAddress(addr.c_str());

    // Create header
    this->setGuid(nackFrag);

    // Create NackFrag submessage
    nackFrag->setNackFragSet(true);
    nackFrag->setReaderId(entityId);

    int sequenceNumber = hb->getWriterSN();

    int startFragNum = hb->getFragmentStartingNum();
    int endFragNum = hb->getLastFragmentNum();

    // Define the start and end points of the data field - Ack from the lowest
    // non received fragment number ! Lower frag_Sns have been received!
    // Start number. Minimum Bitmap size is 8 (if possible)


    // relevant history cache entry in WriterProxy
    ChangeForWriter* cfw = writerProxy->getChange(sequenceNumber);
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

    // Get the highest unacknowledged fragment number within the currently send
    int highestUnackedMsg = 0;
    for(int i = 0; i < cfw->numberFragments; i++)
    {
        if(!frags[i]->received){
            highestUnackedMsg = i;
        }
        if(i == endFragNum){
            break;
        }
    }

    if(highestUnackedMsg > endFragNum){
        endFragNum = highestUnackedMsg;
    }

    // Last fill the bitmap up to the next 32 bit
    int nbrPadBits = 32-((endFragNum - startFragNum + 1)%32);

    // use space below
    if(startFragNum >= nbrPadBits){
        startFragNum = startFragNum - nbrPadBits;
        nbrPadBits = 0;
    } else {
        nbrPadBits = nbrPadBits - startFragNum;
        startFragNum = 0;
    }

    // use space up
    if(hb->getLastFragmentNum()- endFragNum >= nbrPadBits){
        startFragNum = startFragNum + nbrPadBits;
        nbrPadBits = 0;
    } else {
        nbrPadBits = nbrPadBits - (hb->getLastFragmentNum()- startFragNum);
        startFragNum = hb->getLastFragmentNum();
    }

    nackFrag->setFragmentNumberStateNbrBits(endFragNum - startFragNum + 1);
    nackFrag->setFragmentNumberStateBase(startFragNum);

    // Pass the values to the bitmap
    for(int i = 0; i < nackFrag->getFragmentNumberStateNbrBits(); i++){
        auto fragStatus = frags[i+startFragNum]->received;
        nackFrag->setFragmentNumberBitmap(i, fragStatus);
    }

    nackFrag->setWriterSN(sequenceNumber);

    // Finally, calculate the overall rtps message size
    nackFrag->setInfoDestinationSet(false);
    calculateRtpsMsgSize(nackFrag);

    return nackFrag;
}
