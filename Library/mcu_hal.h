#ifndef _MCU_HAL_H_
#define _MCU_HAL_H_

#include <stdio.h>
#include "at32f403a_407.h"
#include "at32f403a_407_rtc.h"
#include "at32f403a_407_pwc.h"
#include "at32f403a_407_wk_config.h"
#include "wk_system.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define mcu_reset() nvic_system_reset()

// Data Synchronization Barrier
// Instruction Synchronization Barrier
#define mcu_safe_reset()     \
    do                       \
    {                        \
        __DSB();             \
        __ISB();             \
        nvic_system_reset(); \
    } while (0);

/*!
 * Panic function for mcu issues
 */
#define mcu_panic(...)                           \
    do                                           \
    {                                            \
        SMTC_MODEM_HAL_TRACE_ERROR(__VA_ARGS__); \
        mcu_safe_reset();                        \
    } while (0);

#define TRACE_ERROR(...) mcu_panic(__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif
