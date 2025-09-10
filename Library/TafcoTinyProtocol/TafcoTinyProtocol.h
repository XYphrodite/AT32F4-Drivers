#ifndef _TAFCO_TINY_PROTOCOL_H_
#define _TAFCO_TINY_PROTOCOL_H_

#include "at32f403a_407.h"
#include "Metrics.h"

struct TafcoTinyProtocol
{
    int8_t topTempI : 8;
    int8_t mainTempI : 8;
    uint8_t topTempF : 4;
    uint8_t mainTempF : 4;
    uint8_t voltage_sum : 8;
    uint8_t voltage_devided : 8;
};

TafcoTinyProtocol TafcoTinyProtocol_Create(const float &topTemp, const float &mainTemp, const float &voltage_sum, const float &voltage_devided);
TafcoTinyProtocol TafcoTinyProtocol_Create(Metrics m);
void TafcoTinyProtocol_Parse(TafcoTinyProtocol &data, float &topTemp, float &mainTemp, float &voltage_sum, float &voltage_devided);

#endif
