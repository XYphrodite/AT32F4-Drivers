#include "PacketWrapper.h"

// choose one of them

// #define CayenneLPP_PACKET
// #define TINY_PACKET
#define EXTRA_SMALL_PACKET

PacketWrapper::PacketWrapper(/* args */)
{
}

PacketWrapper::~PacketWrapper()
{
}

void PacketWrapper::Wrap(const uint8_t *inBuf, uint16_t inLen, uint8_t *outBuf, uint16_t *outLen, uint8_t comCode)
{
    *outLen = inLen + 3;
    outBuf[0] = RESPONSE_COM_PREFIX;  // prefix
    outBuf[1] = comCode;              // type
    memcpy(&outBuf[2], inBuf, inLen); // body
    outBuf[2 + inLen] = count_CRC(outBuf, 2 + inLen);
}

#if (END_DEVICE_TEMPERATURE_SENSOR && !BOOTLOADER)
void PacketWrapper::MakeLoraPacket(uint8_t *outBuff, int32_t &size)
{
    size = 0;

#if CayenneLPP_PACKET
    CayenneLPP lpp = CayenneLPP(); // v_all 2 + v_bat0 2 + main_temp 2 +
                                   //  top_temp 2 + accel 6 + accel_temp 2 = 16

    lpp.addTemperature(1, metrics.GetMainTemperature());
    lpp.addVoltage(1, metrics.GetSumVoltage());
    lpp.addTemperature(2, metrics.GetTopTemperature());
    lpp.addVoltage(2, metrics.GetVoltage0());
#elif (TINY_PACKET || (defined EXTRA_SMALL_PACKET))
    TafcoTinyProtocol data = TafcoTinyProtocol_Create(metrics);
#endif
    uint8_t *cursor = outBuff;

#if (TINY_PACKET || CayenneLPP_PACKET)
    uint8_t code = 0x10U;
#elif defined EXTRA_SMALL_PACKET
    uint8_t code = 0x11U;
#endif
    memcpy(cursor, &code, sizeof(code));
    cursor += sizeof(code);
    size += sizeof(code);

#if CayenneLPP_PACKET
    uint8_t version = 1U;
#elif defined TINY_PACKET
    uint8_t version = 2U;
#endif

#if (CayenneLPP_PACKET || TINY_PACKET)
    memcpy(cursor, &version, sizeof(version));
    cursor += sizeof(version);
    size += sizeof(version);
#endif

#if (CayenneLPP_PACKET || TINY_PACKET)
    uint32_t serialNum = ModemConfig::GetSerialNum();
#elif defined EXTRA_SMALL_PACKET
    uint16_t serialNum = ModemConfig::GetSerialNum();
#endif
    memcpy(cursor, &serialNum, sizeof(serialNum));
    cursor += sizeof(serialNum);
    size += sizeof(serialNum);
#if CayenneLPP_PACKET
    memcpy(cursor, lpp.getBuffer(), lpp.getSize());
    cursor += lpp.getSize();
    size += lpp.getSize();
#else
    memcpy(cursor, &data, sizeof(TafcoTinyProtocol));
    cursor += sizeof(TafcoTinyProtocol);
    size += sizeof(TafcoTinyProtocol);

    uint8_t crc = count_CRC(outBuff, size);
    memcpy(cursor, &crc, sizeof(crc));
    size += sizeof(crc);
#endif
}
#endif
