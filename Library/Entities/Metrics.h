#ifndef _METRICS_H_
#define _METRICS_H_

#include "at32f403a_407_conf_template.h"
#include "at32f403a_407_clock.h"
#include <cstring>
#include <limits>
#include <cstdint>
#ifdef END_DEVICE_TEMPERATURE_SENSOR
#include "Voltage.h"
#endif
// #include <math.h>
#include <cmath>

struct MetricsErr
{
    bool tempTop : 1;
    bool tempMain : 1;
    bool accel : 1;
    bool voltages : 1;
    uint8_t reserved : 4;
};

// #pragma pack(push, 4)
struct Metrics //__attribute__((aligned(4)))
{
protected:
    float main_temperature {-100.0};
    float top_temperature {-100.0};
    float v_bat {-100.0};
    float v_bat_1 {-100.0};
    float v_bat_2 {-100.0};
    // float v_internal;

    // uint16_t v_bat_raw;
    // uint16_t v_bat_1_raw;

    // float internal_temperature;
    MetricsErr err;

#ifdef END_DEVICE_TEMPERATURE_SENSOR
private:
    float accelX;
    float accelY;
    float accelZ;
    float accelTemp;

    float angleX;
    float angleZ;

    bool isVertical;
#endif

public:
    Metrics(/* args */);
    ~Metrics();
    float GetTopTemperature(void);
    float GetMainTemperature(void);
    void SetTemperature(float &top, float &main);
    // void SetInternalTemperature(float t);
    void SetVoltage(float &v_internal, float &v_bat, float &v_bat_div_2);
    void SetVoltage(float &v1, float &v2);
    float GetSumVoltage(void);
    float GetVoltage0(void);
    void SerializeMetrics(uint8_t *outBuf, uint16_t *outLen);
    void SetError(MetricsErr newErr);
    void SetAccelData(float x, float y, float z, float t);
    float GetAngleY(void);
    bool IsVertical(float max_lean);
    void AddToBps(void);
    void GetFromBps(void);
#ifdef END_DEVICE_TEMPERATURE_SENSOR
    void SetRawVoltages(uint16_t all, uint16_t half);
#endif
    bool AnyError(void);

    static bool CheckTemperatureRange(float temp);
    static void CheckStability(bool &temp_is_ok, float &temp0, float &temp1, float &temp);
};
// #pragma pack(pop)

extern Metrics metrics;

#endif
