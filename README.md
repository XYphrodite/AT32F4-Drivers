# AT32F4-Drivers Library

Shared core library for AT32F403A/AT32F407 microcontroller projects.

## Overview

AT32F4-Drivers is a hardware abstraction layer (HAL) and driver collection that provides a unified interface for AT32 microcontroller peripherals. This library is shared across multiple firmware projects in this repository.

## Supported Microcontrollers

- **AT32F403A** - Primary target (AT32F403ACGT7/AT32F403ACGU7)
- **AT32F407** - Secondary target

Both MCUs are based on ARM Cortex-M4F core with FPU support.

## Library Components

### Core MCU Support (`AT32F403A/`)
- **CMSIS** - ARM Cortex Microcontroller Software Interface Standard
  - Core peripheral definitions and startup code
  - Device headers and system initialization
- **Drivers** - Peripheral drivers (51 files)
  - GPIO, UART/USART, I2C, SPI, ADC, DMA, Timers
  - CRM (Clock and Reset Management)
  - Flash memory controller
- **Linker Scripts** - Memory layout definitions for AT32F403AxG
- **SVD Files** - System View Description for debugging
- **Startup Code** - Assembly startup files

### Middleware (`Middlewares/`)

#### FreeRTOS (769 files)
- **Version:** Real-time operating system
- **Heap:** Configurable (Heap_1 to Heap_5)
- **Ports:** GCC_ARM_CM4F
- **Configuration:** Project-specific FreeRTOSConfig.h
- **Usage:** Enabled in lite-gateway firmware

#### USB Stack (42 files)
- **Device Classes:**
  - USB Virtual COM Port (CDC)
  - USB WinDriver
- **Features:**
  - Full-speed USB 2.0
  - Custom vendor-specific protocols
- **Usage:** Debug and communication interface

#### Segger RTT (14 files)
- **Real-Time Transfer** for debug output
- **Features:**
  - High-speed debug logging
  - Minimal CPU overhead
  - JLink debugger support
- **Usage:** Debug printing without UART

### Common Libraries (`Library/`)

#### Communication
- **UART/USART** - Serial communication drivers
  - `UartDrv.cpp/.h` - Low-level UART driver
  - `UartApi.cpp/.h` - High-level UART service
  - `UartStrBuf.cpp` - String buffer utilities
  - Ring buffer implementation with DMA support
  - Configurable baudrate (default 115200)

#### Protocol & Encoding
- **CayenneLPP** - Low Power Payload encoding
  - Standard LoRaWAN data format
  - Type-length-value (TLV) encoding
  - Temperature, voltage, GPS, accelerometer support

#### System Control
- **UnitControl** - Device control and configuration
  - Power management
  - System state machine
  - Configuration persistence

#### Utilities
- **Timers** - Hardware and software timers
- **GPIO** - Pin control abstractions
- **ADC** - Analog-to-digital conversion
- **I2C/SPI** - Bus communication protocols
- **Watchdog** - System reliability
- **RTC** - Real-time clock support

## Build System Integration

### CMake Configuration

Each firmware project includes AT32F4-Drivers via:

```cmake
set(SHARED_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../AT32F4-Drivers)
```

### Compile Definitions

Standard definitions for AT32 projects:
```cmake
-DAT32F403ACGT7      # Target MCU
-DUSE_STDPERIPH_DRIVER
```

### Include Paths

Projects automatically include:
- `AT32F403A/cmsis/cm4/device_support/`
- `AT32F403A/drivers/inc/`
- `Library/` (various subdirectories)

## Usage Example

### UART Initialization
```cpp
#include "UartApi.h"

usart_init_type uart_config = {
    .usart = USART1,
    .baudrate = 115200,
    .data_bit = USART_DATA_8BITS,
    .stop_bit = USART_STOP_1_BIT,
    .parity = USART_PARITY_NONE,
    // ... additional config
};

UartService::Init(&uart_config);
```

### FreeRTOS Task
```cpp
#include <FreeRTOS.h>
#include <task.h>

void myTask(void *parameters) {
    for (;;) {
        // Task code
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

xTaskCreate(myTask, "myTask", 256, NULL, tskIDLE_PRIORITY + 1, NULL);
```

## Important Notes

⚠️ **Legacy Version Warning:**  
This is an older/customized version of AT32F4-Drivers maintained specifically for this project. Exercise caution when integrating newer library versions as compatibility issues may arise.

### Known Modifications
- **UnitControlWave** is disabled in LBS (LoRaWAN Basic Stack)
- Custom peripheral configurations for AT32F403A
- Project-specific middleware adaptations

## Memory Requirements

Typical memory footprint:
- **Flash:** ~100-200KB (depending on enabled features)
- **RAM:** ~20-50KB + FreeRTOS heap (if used)
- **Stack:** Configurable per task (FreeRTOS) or ~2KB (bare metal)

## Compiler Requirements

- **Toolchain:** ARM GCC (arm-none-eabi-gcc)
- **C Standard:** C11
- **C++ Standard:** C++20
- **Optimization:** -O2 (Release), -Og (Debug)

## Related Documentation

- **Main Project README:** `../readme.md`
- **Project Context:** `../ProjectContext.md`
- **AT32 Reference Manual:** Vendor documentation for peripheral details
- **FreeRTOS Documentation:** https://www.freertos.org/

## License

See individual source files for copyright and license information. Components include:
- AT32 CMSIS and drivers: ArteryTek copyright
- FreeRTOS: MIT License
- Segger RTT: Segger Microcontroller license
- USB middleware: Various licenses

---

For project-specific usage and integration details, refer to the main project documentation in the parent directory.
