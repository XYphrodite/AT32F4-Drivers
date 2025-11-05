#include "WatchDog.h"

// Forward declarations from existing implementations
extern "C"
{
    void simple_wdt_init(void);
    void simple_wdt_reload(void);
    void wdt_init(void);
    void wdt_try_reload(void);
    void* memset(void*, int, unsigned long);  // memset from MCU headers
    void wdt_notify_to_subject(WatchDogSubject_t s);
}

// Choose implementation at compile time
#define USE_HEALTH_CHECK_WDT  // Comment out for simple WDT

void watchdog_init(void)
{
#ifdef USE_HEALTH_CHECK_WDT
    wdt_init();
#else
    simple_wdt_init();
#endif
}

void watchdog_reload(void)
{
#ifdef USE_HEALTH_CHECK_WDT
    wdt_try_reload();
#else
    simple_wdt_reload();
#endif
}

void watchdog_mark_healthy(WatchDogComponent_t component)
{
#ifdef USE_HEALTH_CHECK_WDT
    WatchDogSubject_t s;
    memset(&s, 0, sizeof(WatchDogSubject_t));
    
    // Set specific bit using bit manipulation
    *(uint8_t *)&s = (uint8_t)(1 << component);
    
    wdt_notify_to_subject(s);
#endif
}

void watchdog_notify_custom(WatchDogSubject_t subject)
{
#ifdef USE_HEALTH_CHECK_WDT
    wdt_notify_to_subject(subject);
#endif
}

