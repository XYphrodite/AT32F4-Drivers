
#include "Metrics.h"

Metrics::Metrics(/* args */)
{
    top_temperature = 0;
    main_temperature = 0;
    // v_internal = 0;
    // v_bat_raw = 0;
    // v_bat_1_raw = 0;
    v_bat = 0;
    v_bat_1 = 0;
    v_bat_2 = 0;
    // internal_temperature = 0;
#ifdef END_DEVICE_TEMPERATURE_SENSOR
    accelX = 0;
    accelY = 0;
    accelZ = 0;
    accelTemp = 0;
    angleX = 0;
    angleZ = 0;
    isVertical = false;
#endif
    err = MetricsErr{};
}

Metrics::~Metrics() {}

float Metrics::GetMainTemperature(void) { return !err.tempMain ? main_temperature : -100.0; }

float Metrics::GetTopTemperature(void) { return !err.tempTop ? top_temperature : -100.0; }

void Metrics::SetTemperature(float &top, float &main)
{
    top_temperature = top;
    main_temperature = main;
}

// void Metrics::SetInternalTemperature(float t) { internal_temperature = t; }

/// @brief
/// @param v1 internal v
/// @param v2 v of all bats
/// @param v3 v of one of bat
void Metrics::SetVoltage(float &v1, float &v2, float &v3)
{
    // v_internal = v1;
    v_bat = v2;
    v_bat_1 = v3;
    v_bat_2 = v2 - v3;
}

/// @brief Sets the voltage metrics for the device.
/// @details This function checks if the provided voltages are within valid ranges. If not, it sets error flags and marks the voltages as invalid (NaN). Otherwise, it updates the voltage metrics.
/// @param v1 Reference to the voltage value of all batteries (expected range: 2.0V to 3.6V).
/// @param v2 Reference to the voltage value of one battery (expected range: 0.8V to 1.8V).
void Metrics::SetVoltage(float &v1, float &v2)
{

    if (v1 < 2.0 || v1 > 3.6 || v2 < 0.8 || v2 > 1.8)
    {
        err.voltages = true;
        v_bat = std::numeric_limits<float>::quiet_NaN();
        v_bat_1 = std::numeric_limits<float>::quiet_NaN();
    }
    else
    {
        float vbat = v1;
        float vbat1 = v2;
        this->v_bat = vbat; // HARD FAULT HERE
        this->v_bat_1 = vbat1;
        // v_bat_2 = v_bat - v_bat_1;
    }
}

float Metrics::GetSumVoltage(void) { return v_bat; }
float Metrics::GetVoltage0(void) { return v_bat_1; }

void Metrics::SerializeMetrics(uint8_t *outBuf, uint16_t *outLen)
{
    uint8_t *cursor = outBuf;

    if (err.tempMain)
        main_temperature = std::numeric_limits<float>::quiet_NaN();
    memcpy(cursor, &main_temperature, sizeof(main_temperature));
    *outLen += sizeof(main_temperature);
    cursor += sizeof(main_temperature);

    if (err.tempTop)
        top_temperature = std::numeric_limits<float>::quiet_NaN();
    memcpy(cursor, &top_temperature, sizeof(top_temperature));
    *outLen += sizeof(top_temperature);
    cursor += sizeof(top_temperature);

    if (err.voltages)
    {
        v_bat = std::numeric_limits<float>::quiet_NaN();
        v_bat_1 = std::numeric_limits<float>::quiet_NaN();
        v_bat_2 = std::numeric_limits<float>::quiet_NaN();
    }
    memcpy(cursor, &v_bat, sizeof(v_bat));
    cursor += sizeof(v_bat);
    *outLen += sizeof(v_bat);
    memcpy(cursor, &v_bat_1, sizeof(v_bat_1));
    cursor += sizeof(v_bat_1);
    *outLen += sizeof(v_bat_1);
    v_bat_2 = v_bat - v_bat_1;
    memcpy(cursor, &v_bat_2, sizeof(v_bat_2));
    cursor += sizeof(v_bat_2);
    *outLen += sizeof(v_bat_2);

#ifdef END_DEVICE_TEMPERATURE_SENSOR

    if (err.accel)
    {
        accelX = std::numeric_limits<float>::quiet_NaN();
        accelY = std::numeric_limits<float>::quiet_NaN();
        accelZ = std::numeric_limits<float>::quiet_NaN();
        accelTemp = std::numeric_limits<float>::quiet_NaN();
        angleX = std::numeric_limits<float>::quiet_NaN();
        angleZ = std::numeric_limits<float>::quiet_NaN();
        isVertical = false;
    }
    memcpy(cursor, &accelX, sizeof(accelX));
    *outLen += sizeof(accelX);
    cursor += sizeof(accelX);

    memcpy(cursor, &accelY, sizeof(accelY));
    *outLen += sizeof(accelY);
    cursor += sizeof(accelY);

    memcpy(cursor, &accelZ, sizeof(accelZ));
    *outLen += sizeof(accelZ);
    cursor += sizeof(accelZ);

    memcpy(cursor, &accelTemp, sizeof(accelTemp));
    *outLen += sizeof(accelTemp);
    cursor += sizeof(accelTemp);

    memcpy(cursor, &angleX, sizeof(angleX));
    *outLen += sizeof(angleX);
    cursor += sizeof(angleX);

    memcpy(cursor, &angleZ, sizeof(angleZ));
    *outLen += sizeof(angleZ);
    cursor += sizeof(angleZ);

    memcpy(cursor, &isVertical, sizeof(isVertical));
    *outLen += sizeof(isVertical);
    cursor += sizeof(isVertical);

#endif
}

void Metrics::SetError(MetricsErr newErr)
{
    // float float_nan   { NAN };        /* 00 00 C0 7F  */
    uint8_t err_8 = *reinterpret_cast<uint8_t *>(&err);
    uint8_t new_err_8 = *reinterpret_cast<uint8_t *>(&newErr);
    err_8 = err_8 | new_err_8;
    err = *reinterpret_cast<MetricsErr *>(&err_8);
}

#ifdef END_DEVICE_TEMPERATURE_SENSOR

void Metrics::SetAccelData(float x, float y, float z, float t)
{
    accelX = x;
    accelY = y;
    accelZ = z;

    // Roll (X-axis)
    // angleZ = std::atan2(y, z) * (180.0f / M_PI);

    // Pitch (Y-axis)
    // angleX = std::atan2(y, x) * (180.0f / M_PI);
}

// float Metrics::GetAngleY(void)
// {
//     return angleZ;
// }

bool Metrics::IsVertical(float max_lean)
{
    // isVertical = std::fabs(angleX) <= max_lean && std::fabs(angleZ) <= max_lean;
    isVertical = accelY > 724; // square root of 2 divide by two and multiply by 1024
    return isVertical;
}
#endif

void Metrics::AddToBps(void)
{
    if (AnyError())
        return;
    add_new_main_temperature(main_temperature);
    add_new_top_temperature(top_temperature);
    // add_new_raw_voltages(v_bat_raw, v_bat_1_raw);
    increase_measurementc_cnt();
}

bool Metrics::AnyError(void)
{
    return err.tempMain || err.tempTop || err.voltages || err.accel;
}

#ifdef END_DEVICE_TEMPERATURE_SENSOR
void Metrics::GetFromBps(void)
{
    // voltages
    uint16_t v_all_16 = static_cast<uint16_t>(voltages_raw_all_sum / measurements_amount);
    v_bat = CONVERT_UINT16_TO_VOLTAGE(v_all_16);
    uint16_t v_1_16 = static_cast<uint16_t>(voltages_raw_div_2_sum / measurements_amount);
    v_bat_1 = CONVERT_UINT16_TO_VOLTAGE(v_1_16);
    uint16_t v_2_16 = v_all_16 - v_1_16;
    v_bat_2 = CONVERT_UINT16_TO_VOLTAGE(v_2_16);
    // temperatures
    main_temperature = (main_temperatures_sum / measurements_amount / 10000.0);
    top_temperature = (top_temperatures_sum / measurements_amount / 10000.0);
}

void Metrics::SetRawVoltages(uint16_t all, uint16_t half)
{
    // v_bat_raw = all;
    // v_bat_1_raw = half;
}

/// @brief Checks if a temperature value is within the valid range.
/// @details Valid temperature range is -40.0°C to 125.0°C (exclusive).
/// @param temp The temperature value to check.
/// @return true if the temperature is within the valid range, false otherwise.
bool Metrics::CheckTemperatureRange(float temp)
{
    if (temp < 125.0 && temp > -40.0)
        return true;
    return false;
}

/// @brief Checks the stability of two temperature readings and computes the result.
/// @details Sets temp_is_ok to true if both temperatures are in range and their difference is less than or equal to 0.1. If stable, sets temp to temp0.
/// @param[out] temp_is_ok Reference to a boolean that will be set to true if the temperatures are stable and in range, false otherwise.
/// @param[in] temp0 Reference to the first temperature value.
/// @param[in] temp1 Reference to the second temperature value.
/// @param[out] temp Reference to the resulting temperature value if stable (set to temp0).
void Metrics::CheckStability(bool &temp_is_ok, float &temp0, float &temp1, float &temp)
{
    temp_is_ok = CheckTemperatureRange(temp0) && CheckTemperatureRange(temp1);
    if (temp_is_ok)
        fabs(temp0 - temp1) > 0.2 ? temp_is_ok = false : temp_is_ok = true;
    if (temp_is_ok)
        temp = (temp0);
}

Metrics metrics = Metrics();

#endif
