# AT32F4-Drivers — HAL & Drivers for AT32F403A / AT32F407 (Cortex-M4F) | STM32-Compatible

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Language: C/C++](https://img.shields.io/badge/language-C%2FC++-blue.svg)](AT32F403A/drivers/inc/)
[![MCU: AT32F403A/407](https://img.shields.io/badge/MCU-AT32F403A%20%7C%20AT32F407-green.svg)](#supported-microcontrollers)
[![Core: Cortex-M4F](https://img.shields.io/badge/core-Cortex--M4F%20%40%20240MHz-red.svg)](#supported-microcontrollers)
[![Context7: at32f403a-407-docs](https://img.shields.io/badge/Context7-at32f403a--407--docs-blueviolet.svg)](https://context7.com/xyphrodite/at32f403a-407-docs)
[![LoRa: SX126x/LLCC68](https://img.shields.io/badge/LoRa-SX126x%20%7C%20LLCC68-orange.svg)](#common-libraries-library)

[English](README.md) | [Русский](README_ru.md) | [Docs: Getting Started](Docs/Getting_Started.md) | [Context7 Docs](https://context7.com/xyphrodite/at32f403a-407-docs)

**Production-grade HAL and drivers for ArteryTek AT32F403A / AT32F407 (ARM Cortex-M4F)** — GPIO, USART, SPI, I2C, ADC, DMA, TMR, CRM, Flash, FreeRTOS, USB FS, Segger RTT, LoRa SX126x/LLCC68, CayenneLPP. **STM32-compatible, CMSIS, CMake, ARM GCC.** Bilingual EN/RU.

> **Keywords for search:** `at32`, `at32f403a`, `at32f407`, `arterytek`, `artery tek`, `cortex-m4`, `cortex-m4f`, `stm32 compatible`, `stm32f4`, `hal`, `cmsis`, `embedded`, `mcu`, `microcontroller`, `arm gcc`, `freertos`, `lora`, `sx126x`, `llcc68`, `usb fs`, `segger rtt`, `gpio`, `usart`, `uart`, `spi`, `i2c`, `adc`, `dma`, `hal driver`, `bare metal`

## Why this library

- **51 peripheral drivers** + CMSIS + startup + linker scripts — flash and run
- **STM32-compatible API** — migrate STM32F4 code to AT32 with minimal changes
- **Batteries included:** FreeRTOS (769 files), USB FS (CDC), Segger RTT, CayenneLPP, LoRa
- **Proven in field:** powers TAFCO LoRaWAN sensor network (end-device + gateway + bootloader)
- **Context7-optimized docs:** 1,985 working examples + errata (41 issues) at [`at32f403a-407-docs`](https://github.com/XYphrodite/at32f403a-407-docs)

## Supported Microcontrollers

- **AT32F403A** — primary (AT32F403ACGT7 / AT32F403ACGU7, 1 MB Flash, 224 KB RAM, 240 MHz)
- **AT32F407** — secondary, same Cortex-M4F + FPU

Both: ARM Cortex-M4F + FPU, STM32F4-compatible peripheral set.

## Library Components

### Core MCU Support (`AT32F403A/`)
- **CMSIS** — core definitions, startup, system init
- **Drivers** — 51 files: GPIO, UART/USART, I2C, SPI, ADC, DMA, TMR, CRM, Flash
- **Linker Scripts** — `ldscripts/` for AT32F403AxG (preboot / boot / app)
- **SVD** — System View Description for debug
- **Startup** — `startup_at32f403a_407.s`

### Middleware (`Middlewares/`)
- **FreeRTOS** (769 files) — Heap_1..5, GCC_ARM_CM4F, `FreeRTOSConfig.h` — used in lite-gateway
- **USB Stack** (42 files) — CDC Virtual COM, WinUSB, FS 2.0
- **Segger RTT** (14 files) — zero-UART debug via J-Link

### Common Libraries (`Library/`)
- **UART/USART** — `UartDrv` / `UartApi` / `UartStrBuf`, DMA + ring buffer, 115200 default
- **CayenneLPP** — LoRaWAN TLV (temp/voltage/GPS/accel)
- **UnitControl** — power, state machine, config persistence
- **Timers / GPIO / ADC / I2C / SPI / WDT / RTC**

See also: [`Library/ArteryCore/`](Library/ArteryCore/) (ArteryCore HAL), [`Library/Lora/`](Library/Lora/), [`Library/TafcoCrypt/`](Library/TafcoCrypt/).

## Build System Integration

```cmake
# CMake — add to your firmware
set(SHARED_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../AT32F4-Drivers)
include(${SHARED_LIB_PATH}/cmake_shared.cmake)
include(${SHARED_LIB_PATH}/Library/CayenneLPP/lpp.cmake)
```
```cmake
# Definitions
-DAT32F403ACGT7 -DUSE_STDPERIPH_DRIVER -DREGION_RU_864 -DSX126X
# or -DUSE_LLCC68
```

Include paths auto-added: `AT32F403A/cmsis/cm4/device_support/`, `AT32F403A/drivers/inc/`, `Library/`.

## Usage Example

### UART
```cpp
#include "UartApi.h"
usart_init_type cfg = { .usart = USART1, .baudrate = 115200,
  .data_bit = USART_DATA_8BITS, .stop_bit = USART_STOP_1_BIT, .parity = USART_PARITY_NONE };
UartService::Init(&cfg);
```

### FreeRTOS
```cpp
#include <FreeRTOS.h>
#include <task.h>
void myTask(void*) { for(;;) vTaskDelay(pdMS_TO_TICKS(100)); }
xTaskCreate(myTask, "myTask", 256, nullptr, 1, nullptr);
```

More: [`Docs/Getting_Started.md`](Docs/Getting_Started.md) · [`example`](Library/ArteryCore/)

## Documentation

- **Getting Started:** [`Docs/Getting_Started.md`](Docs/Getting_Started.md) | [RU](Docs/Getting_Started_ru.md)
- **Context7 AI docs (1,985 examples):** https://context7.com/xyphrodite/at32f403a-407-docs
- **Project Context:** [`ProjectContext.md`](ProjectContext.md)
- **Protocols:** [`Library/Terminal/PROTOCOLS.md`](Library/Terminal/PROTOCOLS.md)
- **Errata (41 issues):** [`at32f403a-407-docs`](https://github.com/XYphrodite/at32f403a-407-docs) (Context7 + YAML taxonomy)

## Context7 / LLM Search

This repo is indexed for AI assistants:
- **Context7 libraryId:** `/xyphrodite/at32f403a-407-docs` (firmware library + errata, 1,985 examples)
- Add to your prompt: `use library /xyphrodite/at32f403a-407-docs` or `/xyphrodite/at32f4-drivers`
- `llms.txt` at root lists all entry points for ingestion

## Important Notes

⚠️ **Legacy-customized** for TAFCO production — pinned to tested CMSIS/Drivers. Check compatibility before bumping. `UnitControlWave` disabled in LBS.

## Memory / Requirements

- Flash ~100–200 KB, RAM ~20–50 KB + FreeRTOS heap, stack 2 KB (bare) / per-task
- Toolchain: `arm-none-eabi-gcc`, C11, C++20, `-O2`/`-Og`

## License

Per-file copyright (ArteryTek CMSIS/drivers, MIT FreeRTOS, Segger RTT). See headers.

---

For TAFCO LoRaWAN integration see parent repo [`XYphrodite/TAFCO-Lora-projects`](https://github.com/XYphrodite/TAFCO-Lora-projects) (if public) and [`Docs/`](Docs/).

<!--
GitHub search: at32 at32f403a at32f407 arterytek cortex-m4 stm32 hal cmsis embedded mcu arm gcc freertos lora sx126x llcc68
Context7: /xyphrodite/at32f403a-407-docs /xyphrodite/at32f4-drivers
-->
