# Changelog

All notable changes to AT32F4-Drivers will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0/).

## [1.0.0] - 2026-09-15

### Added
- Initial public release of AT32F4-Drivers (renamed from `TafcoMcuCore`)
- Support for AT32F403A (primary) and AT32F407 (Cortex-M4F + FPU)
- 51 peripheral drivers (GPIO, USART, I2C, SPI, ADC, DMA, TMR, CRM, Flash, etc.)
- CMSIS + startup + linker scripts + SVD for AT32F403AxG
- Middleware: FreeRTOS (769 files), USB FS Device (42), Segger RTT (14)
- Common libraries: Uart/FlashService/Lora/CayenneLPP/TafcoCrypt/Terminal/Timer/RTC/Watchdog and 20+ more
- Submodules: `sx126x_driver`, `llcc68_driver`, `CANopenNode`, `lvgl`
- MIT License (`Copyright (c) 2025 XYphrodite / TAFCO`)
- Bilingual documentation: `README.md` / `README_ru.md`, `ProjectContext.md` / `ProjectContext_ru.md`, `Library/Terminal/PROTOCOLS.md` / `PROTOCOLS_ru.md`

### Changed
- Renamed `TafcoMcuCore` → `AT32F4-Drivers` (path, `.gitmodules`, `SHARED_LIB_PATH`, docs)
- `at32f403a_407.h` now covers both 403A/407

### Notes
- Legacy/customized version for Tafco projects — production-ready, exercise caution when merging upstream Artery BSP
- `cmake_shared.cmake` is template (commented) — integration via per-firmware `CMakeLists.txt`
- `UartService` is DEBUG `USART1` only (DMA `DMA1_CHANNEL1` hard-coded)

[1.0.0]: https://github.com/XYphrodite/AT32F4-Drivers/releases/tag/v1.0.0
