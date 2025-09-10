#ifndef _TRACING_H_
#define _TRACING_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "at32f403a_407.h"
#include "usb_app.h"
#include <stdarg.h>
#include "UartDrv.h"
// #include <SEGGER_RTT.h>

    void hal_print_trace(const char *fmt, ...);

#define HAL_TRACE_PRINTF(...) hal_print_trace(__VA_ARGS__)

#define HAL_TRACE_INFO(...)                     \
    do                                          \
    {                                           \
        HAL_TRACE_PRINTF("INFO: " __VA_ARGS__); \
    } while (0);

#define HAL_TRACE_ARRAY(msg, array, len)                             \
    do                                                               \
    {                                                                \
        HAL_TRACE_PRINTF("%s - (%lu bytes):\n", msg, (uint32_t)len); \
        for (uint32_t i = 0; i < (uint32_t)len; i++)                 \
        {                                                            \
            if (((i % 16) == 0) && (i > 0))                          \
            {                                                        \
                HAL_TRACE_PRINTF("\n");                              \
            }                                                        \
            HAL_TRACE_PRINTF(" %02X", (array)[i]);                   \
        }                                                            \
        HAL_TRACE_PRINTF("\n");                                      \
    } while (0);

#ifdef __cplusplus
}
#endif

#endif

