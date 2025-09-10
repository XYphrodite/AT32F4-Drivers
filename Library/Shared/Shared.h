#ifndef _SHARED_H_
#define _SHARED_H_

#include "at32f403a_407.h"
#include <math.h>
#include "DelayService.h"
#include "Consts.h"
#include "FlashService.h"
#include "PinDefines.h"
#include "gpio.h"
#ifdef END_DEVICE_TEMPERATURE_SENSOR
#include "SimpleWD.h"
#endif

void AddNanToFloat(float *pFloat);
void wait_and_reset(void);
template <typename T>
void WriteToBuffer(uint8_t *&cursor, const T &value);
template <typename T>
void ReadFromBuffer(const uint8_t *&buffer, T &value);
template <typename T>
void ReadFromFlash(const uint32_t &address, T &value);
template <typename T>
void SerialReadFromFlash(uint32_t &address, T &value);

#ifdef END_DEVICE_TEMPERATURE_SENSOR
void blink_after_reset(void);
#endif

// Include the implementation file
#include "Shared.tpp"

#endif
