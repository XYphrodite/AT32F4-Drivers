#ifndef _DELAY_SERVICE_H_
#define _DELAY_SERVICE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "at32f403a_407.h"

#define PROC_HZ 240000000

    void delay_ticks(uint64_t ticks);
    void delay_us(uint64_t us);
    void delay_ms(uint64_t ms);

    extern uint32_t cpu_speed;

#ifdef __cplusplus
}
#endif

#endif
