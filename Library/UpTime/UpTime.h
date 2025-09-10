#ifndef _UPTIME_H_
#define _UPTIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f403a_407.h"


    void init_UpTime_Tmr(void);
    void TMR1_BRK_TMR9_IRQHandler(void);

    extern uint32_t upTime_s;
    extern uint32_t upTime_ms;
    extern uint32_t upTime_100us;

    uint32_t GetUpTimeMs(void);
    uint32_t GetUpTimeS(void);

#ifdef __cplusplus
}
#endif

#endif