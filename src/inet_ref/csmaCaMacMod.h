/*
 *
 */

#ifndef INET_CSMAMACMOD_H_
#define INET_CSMAMACMOD_H_

#include "inet/common/FSMA.h"
#include "inet/common/packet/Packet.h"
#include "inet/queueing/contract/IPacketQueue.h"
#include "inet/linklayer/base/MacProtocolBase.h"
#include "inet/linklayer/csmaca/CsmaCaMacHeader_m.h"
#include "inet/physicallayer/contract/packetlevel/IRadio.h"

using namespace inet;

class CsmaCaMacMod : public CsmaCaMac, public MacProtocolBase
{
// TODO implement/overwrite all necessary functions
};

#endif //INET_CSMAMACMOD_H_
