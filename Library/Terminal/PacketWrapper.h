#ifndef _PACKET_WRAPPER_H_
#define _PACKET_WRAPPER_H_

#include "at32f403a_407.h"
#include "CRC.h"
#include "Consts.h"
#include <cstring>

#if (END_DEVICE_TEMPERATURE_SENSOR && !BOOTLOADER)
#include "TafcoTinyProtocol.h"
#include "CayenneLPP.h"
#include "Metrics.h"
#include "ModemConfig.h"
#endif

class PacketWrapper
{
private:
    /* data */
public:
    PacketWrapper(/* args */);
    ~PacketWrapper();
    static void Wrap(const uint8_t *inBuf, uint16_t inLen, uint8_t *outBuf, uint16_t *outLen, uint8_t comCode);
#if (END_DEVICE_TEMPERATURE_SENSOR && !BOOTLOADER)
    static void MakeLoraPacket(uint8_t *outBuff, int32_t &size);
#endif
};

#endif
