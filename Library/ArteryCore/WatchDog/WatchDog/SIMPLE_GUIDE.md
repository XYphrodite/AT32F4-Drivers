# Simple WatchDog Interface - Quick Start

## Super Simple API

Just 3 functions:

```c
watchdog_init();                        // Call once at startup
watchdog_reload();                      // Call periodically
watchdog_mark_healthy(WDT_COMPONENT);   // Mark component healthy
```

## Example 1: Simple Usage

```cpp
#include "WatchDog.h"

int main(void)
{
    // Initialize watchdog once
    watchdog_init();
    
    while (true)
    {
        // Do your work
        process_tasks();
        
        // Reload watchdog
        watchdog_reload();
        
        delay_ms(100);
    }
}
```

## Example 2: With Health Checking

```cpp
#include "WatchDog.h"

void main_loop(void)
{
    watchdog_init();
    
    while (true)
    {
        // Each component marks itself healthy
        if (blinky_task())
        {
            watchdog_mark_healthy(WDT_BLINKY);
        }
        
        if (uart_task())
        {
            watchdog_mark_healthy(WDT_UART);
        }
        
        if (gateway_task())
        {
            watchdog_mark_healthy(WDT_GATEWAY);
        }
        
        if (usb_task())
        {
            watchdog_mark_healthy(WDT_USB);
        }
        
        // Try reload - only works if all components healthy
        watchdog_reload();
    }
}
```

## Component Identifiers

Available components for `watchdog_mark_healthy()`:

- `WDT_BLINKY`
- `WDT_GATEWAY`
- `WDT_UART`
- `WDT_USB`
- `WDT_END_NODE`
- `WDT_TEMP`
- `WDT_UNIT_CONTROL`

## Example 3: Custom Configuration (Advanced)

```cpp
#include "WatchDog.h"

void advanced_usage(void)
{
    // Directly manipulate the subject struct
    WatchDogSubject_t subject;
    subject.blinky = true;
    subject.uart = true;
    subject.gateway = true;
    
    // Notify with custom configuration
    watchdog_notify_custom(subject);
    
    watchdog_reload();
}
```

## Switching Implementations

In `WatchDog.cpp`:

```cpp
#define USE_HEALTH_CHECK_WDT  // Use health checking
// or
// #undef USE_HEALTH_CHECK_WDT  // Use simple reload
```

That's it! No templates, no CRTP, just simple C-style functions.

