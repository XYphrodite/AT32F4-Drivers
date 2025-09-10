#include "TafcoTinyProtocol.h"
#include <cmath>

TafcoTinyProtocol TafcoTinyProtocol_Create(const float &topTemp, const float &mainTemp, const float &voltage_sum, const float &voltage_devided)
{
    TafcoTinyProtocol proto;

    proto.topTempI = static_cast<int8_t>(topTemp);
    proto.topTempF = static_cast<uint8_t>((topTemp - proto.topTempI) * 16.0f);

    proto.mainTempI = static_cast<int8_t>(mainTemp);
    proto.mainTempF = static_cast<uint8_t>((mainTemp - proto.mainTempI) * 16.0f);

    // Handle NaN voltages: if NaN, encode as 0xFF (uint8_t max), for backward compatibility (was 0xFF error code before)
    if (std::isnan(voltage_sum))
        proto.voltage_sum = 0xFF;
    else
        proto.voltage_sum = static_cast<uint8_t>((voltage_sum - 2.0f) * 100.0f);

    if (std::isnan(voltage_devided))
        proto.voltage_devided = 0xFF;
    else
        proto.voltage_devided = static_cast<uint8_t>(voltage_devided * 100.0f);
    return proto;
}

TafcoTinyProtocol TafcoTinyProtocol_Create(Metrics m){
    return TafcoTinyProtocol_Create(m.GetTopTemperature(), m.GetMainTemperature(), m.GetSumVoltage(), m.GetVoltage0());
}

void TafcoTinyProtocol_Parse(TafcoTinyProtocol &data, float &topTemp, float &mainTemp, float &voltage_sum, float &voltage_devided)
{
    // Decode: 0.1°C per unit for temp
    topTemp = data.topTempI + data.topTempF / 16.0f;
    mainTemp = data.mainTempI + data.mainTempF / 16.0f;

    // If encoded as 0xFF, treat as NaN (error value)
    if (data.voltage_sum == 0xFF)
        voltage_sum = std::numeric_limits<float>::quiet_NaN();
    else
        voltage_sum = 2.0f + (data.voltage_sum / 100.0f);

    if (data.voltage_devided == 0xFF)
        voltage_devided = std::numeric_limits<float>::quiet_NaN();
    else
        voltage_devided = (data.voltage_devided / 100.0f);
}
