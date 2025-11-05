#ifndef _WATCH_DOG_H_
#define _WATCH_DOG_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Simple unified interface for all watchdog operations
 */

// Watchdog subject bitfield structure
typedef struct
{
    bool unit_control : 1;
    bool blinky : 1;
    bool gateway : 1;
    bool uart : 1;
    bool end_node : 1;
    bool temp : 1;
    bool usb : 1;
} WatchDogSubject_t;

// Component identifiers for health monitoring
typedef enum
{
    WDT_UNIT_CONTROL = 0,
    WDT_BLINKY       = 1,
    WDT_GATEWAY      = 2,
    WDT_UART         = 3,
    WDT_END_NODE     = 4,
    WDT_TEMP         = 5,
    WDT_USB          = 6
} WatchDogComponent_t;

// Initialize watchdog (call once at startup)
void watchdog_init(void);

// Reload/kick the watchdog (call periodically)
void watchdog_reload(void);

// Mark specific component as healthy
void watchdog_mark_healthy(WatchDogComponent_t component);

// Advanced: Notify with custom subject configuration
void watchdog_notify_custom(WatchDogSubject_t subject);

#ifdef __cplusplus
}
#endif

#endif // _WATCH_DOG_H_

