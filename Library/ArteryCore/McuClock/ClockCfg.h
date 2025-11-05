#ifndef _CLOCK_CFG_H_
#define _CLOCK_CFG_H_

#include <at32f403a_407.h>
#include "DelayService.h"

enum MCU_speed : uint8_t
{
    MAX = 0x1,
    _240Mhz = 0x2,
    _24Mhz = 0x3,
    _8Mhz = 0x4,     // usb does not works
    _4dot8Mhz = 0x5, // usb does not works
    _16Mhz = 0x6,
    _12Mhz = 0x7,
    _9dot6Mhz = 0x8, // usb must not works
};

class ClockCfg
{
private:
    /* data */
public:
    ClockCfg(/* args */) = delete;
    ClockCfg(const ClockCfg &obj) = delete;
    ~ClockCfg() = delete;

    void static SetSpeed(MCU_speed s);
};

#endif