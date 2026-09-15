# AT32F4-Drivers — Общая библиотека HAL для микроконтроллеров

[Русский](ProjectContext_ru.md) | [English](ProjectContext.md)

## Обзор

AT32F4-Drivers — это комплексный уровень аппаратных абстракций (HAL) и набор драйверов для микроконтроллеров Artery AT32F403A/407 (ARM Cortex-M4F). Библиотека служит фундаментом для нескольких проектов прошивок в экосистеме Tafco, предоставляя единые интерфейсы для управления периферией, интеграции middleware и системных сервисов.

**Тип репозитория**: Общая библиотека  
**Целевые МК**: AT32F403A, AT32F407 (ARM Cortex-M4F с FPU)  
**Основные проекты, использующие библиотеку**:
- P-Block (Modbus RTU — основное приложение)
- Bootloader0 (загрузчик)
- Prebootload (предзагрузчик)
- USB-CAN Converter (утилита)

**Статус** (25 ноября 2025):
- **Версия**: Legacy/кастомизированная для проектов Tafco
- **Стабильность**: Production-ready
- **Документация**: полная, с интеграцией Context7
- **Последнее дополнение**: USB-CAN Converter (24 ноября 2025)

> **Примечание:** Полные таблицы периферии, API и структура каталогов — без перевода (оригинал на английском) для точности. Ниже — перевод ключевых разделов. Оригинал: `ProjectContext.md`.

---
## Supported Microcontrollers

### AT32F403A Series
- **Models**: AT32F403ACGT7, AT32F403ACGU7
- **Core**: ARM Cortex-M4F @ 240 MHz (max)
- **Flash**: Up to 1024 KB
- **RAM**: 96 KB SRAM
- **FPU**: Single-precision floating-point unit
- **Peripherals**: Full AT32 peripheral set

### AT32F407 Series
- **Models**: AT32F407VGT7
- **Core**: ARM Cortex-M4F @ 240 MHz (max)
- **Flash**: Up to 1024 KB
- **RAM**: 96 KB SRAM
- **FPU**: Single-precision floating-point unit
- **Peripherals**: Enhanced AT32 peripheral set

**Common Features**:
- ARM Cortex-M4F core with DSP instructions
- Single-precision FPU (Floating Point Unit)
- Nested Vectored Interrupt Controller (NVIC)
- System timer (SysTick)
- Debug support (SWD, JTAG)

---

## Architecture

### Directory Structure

```
AT32F4-Drivers/
├── AT32F403A/                          # Primary MCU support
│   ├── cmsis/                          # CMSIS standard interface
│   │   ├── cm4/                        # Cortex-M4 core support
│   │   │   ├── core_support/           # ARM CMSIS headers
│   │   │   └── device_support/         # AT32 device headers
│   │   └── dsp/                        # ARM DSP library
│   ├── drivers/                        # Peripheral drivers (51 files)
│   │   ├── inc/                        # Driver headers (26 peripherals)
│   │   └── src/                        # Driver implementations
│   ├── ldscript/                       # Device-specific linker scripts
│   ├── Middlewares/                    # Middleware stack
│   │   ├── FreeRTOS/                   # Real-time operating system
│   │   ├── SeggerRTT/                  # Real-time transfer for debugging
│   │   └── USB/                        # USB device stack
│   ├── startup/                        # Startup assembly files
│   └── svd/                            # System View Description files
│
├── AT32F435/                           # Future MCU support (placeholder)
│
├── Library/                            # Shared library components
│   ├── ArteryCore/                     # Core system services
│   │   ├── BprDriver/                  # Backup register driver
│   │   ├── McuClock/                   # Clock configuration
│   │   ├── Periphery/                  # Peripheral abstractions
│   │   ├── Rtc/                        # Real-time clock
│   │   ├── SystemApi/                  # Unified MCU initialization
│   │   └── WatchDog/                   # Watchdog implementations
│   ├── Bootload/                       # Bootloader utilities
│   ├── CayenneLPP/                     # LoRaWAN payload encoding
│   ├── Command/                        # Command processing
│   ├── CRC/                            # CRC calculation utilities
│   ├── ErrorJournal/                   # Error logging system
│   ├── FlashService/                   # Flash memory operations
│   ├── Lora/                           # LoRa radio support
│   ├── RTC/                            # RTC abstraction
│   ├── TafcoCrypt/                     # Cryptography utilities
│   ├── TafcoTinyProtocol/              # Custom communication protocol
│   ├── Terminal/                       # Terminal protocols
│   ├── Timer/                          # Timer drivers
│   ├── Uart/                           # UART/USART services
│   ├── UnitControl/                    # System control
│   ├── UpTime/                         # System uptime tracking
│   ├── Usb/                            # USB service layer
│   └── externals/                      # External libraries (submodules)
│       └── CANopenNode/                # CANopen protocol stack
│
├── ldscripts/                          # Centralized linker scripts
│   ├── 0-preboot/                      # Pre-bootloader scripts (4 KB)
│   ├── 400-boot/                       # Bootloader scripts (32 KB)
│   └── 1000-main-app/                  # Main application scripts (256 KB)
│
├── utils/                              # Utility applications
│   └── usb-can-converter/              # USB-CAN bridge converter
│
├── at32f403a-407-docs/                 # Documentation submodule (Context7)
│   ├── AT32F403A_407_Firmware_Library/ # 1,985 example files
│   ├── docs/                           # Taxonomy system
│   ├── FAQ.md                          # Common questions
│   └── README.md                       # Documentation overview
│
├── at32f435-437-docs/                  # Future documentation
├── Docs/                               # General documentation
├── README.md                           # Library overview
├── cmake_shared.cmake                  # Shared CMake configuration
├── .gitmodules                         # Git submodule definitions
└── ADD_DOCS_AS_SUBMODULE.md            # Submodule setup guide
```

---

## Core Components

### 1. AT32F403A Drivers (`AT32F403A/drivers/`)

**Peripheral Drivers** (26 peripherals, 51 files total):

| Peripheral | Header | Purpose |
|-----------|--------|---------|
| **ACC** | at32f403a_407_acc.h | Auto Clock Calibration |
| **ADC** | at32f403a_407_adc.h | Analog-to-Digital Converter |
| **BPR** | at32f403a_407_bpr.h | Backup Power Register |
| **CAN** | at32f403a_407_can.h | Controller Area Network |
| **CRC** | at32f403a_407_crc.h | Cyclic Redundancy Check |
| **CRM** | at32f403a_407_crm.h | Clock and Reset Management |
| **DAC** | at32f403a_407_dac.h | Digital-to-Analog Converter |
| **DEBUG** | at32f403a_407_debug.h | Debug interface |
| **DMA** | at32f403a_407_dma.h | Direct Memory Access |
| **EMAC** | at32f403a_407_emac.h | Ethernet MAC |
| **EXINT** | at32f403a_407_exint.h | External Interrupts |
| **FLASH** | at32f403a_407_flash.h | Flash Memory Controller |
| **GPIO** | at32f403a_407_gpio.h | General Purpose I/O |
| **I2C** | at32f403a_407_i2c.h | Inter-Integrated Circuit |
| **MISC** | at32f403a_407_misc.h | Miscellaneous functions |
| **PWC** | at32f403a_407_pwc.h | Power Control |
| **RTC** | at32f403a_407_rtc.h | Real-Time Clock |
| **SDIO** | at32f403a_407_sdio.h | SD/SDIO interface |
| **SPI** | at32f403a_407_spi.h | Serial Peripheral Interface |
| **TMR** | at32f403a_407_tmr.h | Timers (TIM1-8) |
| **USART** | at32f403a_407_usart.h | Universal Sync/Async Receiver/Transmitter |
| **USB** | at32f403a_407_usb.h | Universal Serial Bus |
| **WDT** | at32f403a_407_wdt.h | Watchdog Timer |
| **WWDT** | at32f403a_407_wwdt.h | Window Watchdog Timer |
| **XMC** | at32f403a_407_xmc.h | External Memory Controller |

**Key Features**:
- Complete peripheral register definitions
- Standard peripheral library (SPL) architecture
- Compatible with CMSIS standards
- Optimized for performance and code size

---

### 2. Middleware Stack (`AT32F403A/Middlewares/`)

#### FreeRTOS (769 files)
**Version**: v10.x  
**Purpose**: Real-time operating system for multitasking applications

**Components**:
- Kernel implementation (tasks, queues, semaphores, mutexes)
- Memory management (Heap_1 through Heap_5)
- ARM Cortex-M4F port (GCC_ARM_CM4F)
- Event groups and stream buffers
- Co-routines (optional)

**Usage**:
- Enabled in P-Block main application
- Task scheduling with configurable priorities
- Inter-task communication via queues
- Synchronization primitives

**Configuration**:
- Project-specific `FreeRTOSConfig.h`
- Configurable tick rate (default 1 ms)
- Stack overflow detection
- Task statistics and tracing

#### USB Device Stack (42 files)
**Purpose**: USB 2.0 Full-Speed device implementation

**Device Classes**:
- **CDC (Communications Device Class)**: Virtual COM Port
  - Used in USB-CAN Converter
  - Standard ACM (Abstract Control Model)
  - 115200 baud virtual serial port
- **Composite CDC+Keyboard**: Multi-function USB device
- **Composite CDC+MSC**: USB storage + serial
- **Custom HID**: Human Interface Device
- **HID IAP**: In-Application Programming
- **MSC**: Mass Storage Class
- **WinUSB**: Windows custom driver

**Features**:
- Full-speed USB 2.0 (12 Mbps)
- Standard USB device framework
- Endpoint management
- USB descriptor generation
- Power management

#### Segger RTT (14 files)
**Purpose**: Real-Time Transfer for high-speed debugging

**Features**:
- High-speed debug output (no UART needed)
- Minimal CPU overhead
- JLink debugger support
- Printf-style formatting
- Multi-channel support
- Bi-directional communication

**Usage**:
- Debug logging without UART pins
- Real-time system monitoring
- Performance profiling

---

### 3. System Services (`Library/ArteryCore/`)

#### SystemApi - Unified MCU Initialization
**Location**: `Library/ArteryCore/SystemApi/`  
**Purpose**: Single-entry-point system initialization

**Architecture**:
```cpp
// Basic MCU initialization
SystemApi::Init();

// Selective service initialization
SystemServices_t services;
services.UpTimeTmr = 1;        // Enable uptime timer
services.FlashService = 1;     // Enable flash operations
services.UartService = 1;      // Enable UART service
services.WatchdogTimer = 1;    // Enable watchdog
services.BootloaderConfig = 1; // Enable bootloader support
services.UnitReset = 1;        // Enable unit reset

SystemApi::InitServices(services, &uart_config);
```

**Features**:
- NVIC priority group configuration (NVIC_PRIORITY_GROUP_4)
- Clock speed setup (24 MHz, 120 MHz, 240 MHz)
- GPIO pin remapping for SWD debugging
- Vector table relocation (bootloader support)
- Bitfield-based service configuration (memory-efficient)
- Error handling for missing configurations

#### Peripheral Abstractions (`Library/ArteryCore/Periphery/`)

**ADC Driver**:
- Multi-channel ADC support
- DMA-based continuous conversion
- Configurable sampling time
- Calibration routines
- Documentation: `ADC_DRIVER_SUMMARY.md` (274 lines)

**GPIO Driver**:
- Pin configuration and control
- Input/output mode management
- Pull-up/pull-down configuration
- Alternate function mapping

**I2C HAL**:
- Master/slave mode support
- 7-bit and 10-bit addressing
- DMA support
- Error recovery

**SPI HAL**:
- Master/slave configuration
- Configurable clock polarity/phase
- DMA-based transfers
- Multi-slave support

#### Watchdog System (`Library/ArteryCore/WatchDog/`)

**Three implementations**:

1. **HealthCheckWdt**: Multi-subject monitoring
   - Tracks multiple system components
   - Requires all subjects to report
   - Automatic reset on hang detection
   - Used in P-Block application

2. **SimpleWatchDog**: Basic watchdog
   - Single watchdog reload
   - Configurable timeout
   - Minimal overhead

3. **WatchDogApi**: High-level API
   - Abstraction layer
   - Multiple watchdog modes
   - Documentation: `SIMPLE_GUIDE.md`

#### RTC Support (`Library/ArteryCore/Rtc/`)
- Real-time clock configuration
- Calendar functions
- Alarm support
- Low-power timekeeping
- Battery backup support (BPR registers)

---

### 4. Communication Libraries (`Library/`)

#### UART Service (`Library/Uart/`)
**Components**:
- `UartDrv.cpp/.h`: Low-level UART driver
- `UartApi.cpp/.h`: High-level UART service API
- `UartStrBuf.cpp`: String buffer utilities
- `UartStructures.hpp`: Configuration structures

**Features**:
- Ring buffer implementation
- DMA support for efficient transfers
- Configurable baudrate (default 115200)
- Interrupt-driven operation
- Error detection and handling
- Multiple USART peripheral support

**Usage**:
```cpp
usart_init_type uart_config = {
    .usart = USART1,
    .baudrate = 115200,
    .data_bit = USART_DATA_8BITS,
    .stop_bit = USART_STOP_1_BIT,
    .parity = USART_PARITY_NONE
};
UartService::Init(&uart_config);
```

#### USB Service (`Library/Usb/`)
- `usb_app.cpp/.h`: Application-level USB handling
- `usb_drv.cpp/.h`: Low-level USB driver
- `usb_process.cpp/.h`: USB event processing
- `usbService.cpp/.h`: High-level USB service

**Integration**:
- Works with AT32F403A USB middleware
- CDC class for virtual COM port
- Custom protocols supported
- Power management

#### CayenneLPP (`Library/CayenneLPP/`)
**Purpose**: LoRaWAN Low Power Payload encoding

**Features**:
- Standard LoRaWAN data format
- Type-Length-Value (TLV) encoding
- Supported data types:
  - Temperature, Humidity, Pressure
  - Analog/Digital I/O
  - GPS coordinates
  - Accelerometer data
- Polyline encoding for GPS tracks
- Compact binary format

#### TafcoTinyProtocol (`Library/TafcoTinyProtocol/`)
- Custom binary protocol
- CRC validation
- Command framing
- Used in UART communication

---

### 5. System Utilities (`Library/`)

#### FlashService (`Library/FlashService/`)
**Purpose**: Non-volatile storage management

**Features**:
- Flash sector erase and write
- Data persistence
- CRC validation
- Wear leveling support
- Configuration storage
- Error journal storage

**Usage**:
- Configuration management (ModbusConfig, PBlockConfig)
- Bootloader configuration
- Error logging (ErrorJournal)

#### ErrorJournal (`Library/ErrorJournal/`)
**Purpose**: Structured error recording with timestamps

**Features**:
- Error type classification (WDT, SPI, UART, CAN, MCU)
- Timestamp support (RTC integration)
- Flash-based persistence
- 1280 error record capacity
- Record metadata tracking

#### Command Handler (`Library/Command/`)
**Purpose**: Command processing framework

**Features**:
- Command parsing and validation
- CRC verification
- Response generation
- Extensible command set
- Used in P-Block UART interface (17 commands)

#### UpTime Service (`Library/UpTime/`)
- System uptime tracking
- Millisecond and second counters
- Overflow handling
- Used for diagnostics and timing

#### Timer Service (`Library/Timer/`)
- Hardware timer abstraction
- Software timer emulation
- Timeout management
- PWM generation support

---

### 6. External Libraries (`Library/externals/`)

#### CANopenNode (Git Submodule)
**Source**: https://github.com/CANopenNode/CANopenNode.git  
**Purpose**: CANopen protocol stack implementation

**Components**:
- **301**: Core CANopen features
  - Object Dictionary interface
  - SDO server/client
  - PDO support
  - NMT and Heartbeat
  - Emergency messages
  - SYNC and TIME
- **303**: LED indicators
- **304**: Safety (GFC, SRDO)
- **305**: Layer Setting Services (LSS)
- **309**: Gateway ASCII interface

**Documentation**:
- README.md: Overview
- doc/objectDictionary.md
- doc/deviceSupport.md
- doc/traceUsage.md
- MISRA compliance guide

**Integration**:
- Used in P-Block for CANopen communication
- Object dictionary generation
- Device profiles

---

### 7. LoRa Radio Support (`Library/`)

#### LoRa Modules (`Library/Lora/`)
- SX126x driver integration
- LLCC68 driver integration
- SPI interface abstraction
- IRQ handling
- Configuration macros
- Example options

#### SX126x Driver (Git Submodule)
**Source**: https://github.com/Lora-net/sx126x_driver

#### LLCC68 Driver (Git Submodule)
**Source**: https://github.com/Lora-net/llcc68_driver

**Features**:
- LoRa modulation support
- FSK mode
- Long-range communication
- Low power consumption
- Used in LoRaWAN applications

---

## Linker Scripts (`ldscripts/`)

### Centralized Memory Layout

AT32F4-Drivers provides centralized linker scripts for consistent memory mapping across all projects:

```
ldscripts/
├── 0-preboot/          # Pre-bootloader (4KB @ 0x08000000)
│   ├── AT32F403AxG_FLASH.ld
│   └── AT32F407xG_FLASH.ld
├── 400-boot/           # Bootloader (32KB @ 0x08001000)
│   ├── AT32F403AxG_FLASH.ld
│   └── AT32F407xG_FLASH.ld
└── 1000-main-app/      # Main Application (256KB @ 0x08009000)
    ├── AT32F403AxG_FLASH.ld
    └── AT32F407xG_FLASH.ld
```

### Memory Layout

| Region | Start Address | Size | Usage |
|--------|--------------|------|-------|
| **Pre-bootloader** | 0x08000000 | 4 KB | Initial boot, hardware init |
| **Bootloader** | 0x08001000 | 32 KB | Firmware update, USB/UART |
| **Main Application** | 0x08009000 | 256 KB+ | Primary application |
| **Config Storage** | Sector 253 | 2 KB | Configuration data |
| **Error Journal** | Sector 243 | Variable | Error logging |
| **RAM** | 0x20000000 | 96 KB | All projects |

### Project Usage

- **prebootload**: Uses `0-preboot/` scripts
- **bootloader0**: Uses `400-boot/` scripts
- **P-block**: Uses `1000-main-app/` scripts
- **USB-CAN Converter**: Uses device-specific scripts

### Bootloader Features

- Vector table relocation via `_sboot` and `_sapp` symbols
- Independent build and flash regions
- Seamless application switching
- Boot configuration persistence

---

## Utility Applications (`utils/`)

### USB-CAN Converter
**Location**: `AT32F4-Drivers/utils/usb-can-converter/`  
**Status**: ✅ Complete and production-ready (November 24, 2025)  
**Purpose**: Bidirectional USB Virtual COM Port ↔ CAN bus bridge

**Features**:
- USB CDC Virtual COM Port @ 115200 baud
- CAN bus @ 500 kbps (configurable)
- Text-based ASCII protocol
  - TX: `WRITE:ID:DLC:DATA_HEX[CR/LF]`
  - RX: `READ:ID:DLC:DATA_HEX[CR/LF]`
- Ring buffer message queuing (32 CAN messages)
- ES0002 errata workarounds implemented
- State machine architecture
- LED status indicators
- Comprehensive error handling

**Deliverables**:
- 1,575 lines of production-ready C++ code
- Complete documentation:
  - README.md (550 lines) - User guide & protocol spec
  - BUILD_GUIDE.md (380 lines) - Build system & debugging
  - PROJECT_SUMMARY.md (400 lines) - Architecture reference
  - ProjectContext.md - Complete technical documentation
- CMake build system (Linux/macOS/Windows MSYS2)
- Doxygen-documented API

**Use Cases**:
- CAN bus monitoring and debugging
- Protocol development and testing
- Vehicle diagnostics
- Industrial automation interface

---

## Documentation (`at32f403a-407-docs/`)

### Context7-Optimized Documentation Submodule
**Location**: `AT32F4-Drivers/at32f403a-407-docs/` (Git submodule)  
**Source**: https://github.com/xyphrodite/at32f403a-407-docs.git  
**Status**: ✅ Complete and production-ready  
**Purpose**: Comprehensive MCU documentation for Context7 AI assistant

**Contents**:

#### 1. Firmware Library (1,985 Examples)
- **AT32F403A Examples**: 937 files
- **AT32F407 Examples**: 1,048 files
- **Peripheral Drivers**: Complete API headers and implementations
- **CMSIS Support**: ARM Cortex-M4 core definitions
- **Coverage**: All peripherals (ADC, CAN, Flash, I2C, SPI, TMR, USART, USB, etc.)
- **Size**: ~5-7 MB (93% reduction from original 100 MB)
- **Format**: Context7-optimized (.c/.h/.txt files only)

**Documentation**: `AT32F403A_407_Firmware_Library/README_CONTEXT7.md`

#### 2. Errata Documentation (ES0002 v2.0.11)
- **Total Issues**: 41 documented hardware issues
- **Peripherals Covered**: 15 subsystems
- **Format**: Markdown with embedded Mermaid diagrams
- **Workarounds**: Code examples and mitigation strategies
- **Conversion**: 100% PDF-to-Markdown complete
- **Enhancement**: 4 Mermaid diagrams, enhanced tables

**Note**: For the latest errata sheet, download from [Artery Technology's official website](https://www.arterytek.com/)

#### 3. Context7 Taxonomy System (`docs/`)
- **6 YAML files** enabling semantic search
- **Natural language queries** support
- **FAQ.md**: 264 lines - Common questions and answers
- **Quick reference tables**
- **Peripheral priority matrix**

#### 4. Additional Resources
- **CHANGELOG.md**: Version history
- **CONTRIBUTING.md**: Community contribution guidelines
- **README.md**: 423 lines - Repository overview
- **Setup guides**: GitHub deployment and submodule integration

**Benefits**:
- AI-powered documentation search
- Searchable Markdown format
- Version-controlled documentation
- Community-accessible knowledge base
- Working code examples for all peripherals

---

## Build System Integration

### CMake Configuration

#### Shared Library Path
Projects reference AT32F4-Drivers via:

```cmake
set(SHARED_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../AT32F4-Drivers)
```

#### Standard Compile Definitions

```cmake
add_definitions(
    -DAT32F403ACGT7           # Target MCU
    -DUSE_STDPERIPH_DRIVER    # Enable standard peripheral library
    -DFREE_RTOS_IS_IN_USED    # Enable FreeRTOS (if used)
    -DDEBUG_PRINT_USB         # USB debug output (optional)
    -DGLOB_USE_RTT            # Segger RTT debugging (optional)
    -DUSE_WDT                 # Watchdog enabled (release builds)
)
```

#### Include Paths

```cmake
include_directories(
    ${SHARED_LIB_PATH}/AT32F403A/cmsis/cm4/core_support
    ${SHARED_LIB_PATH}/AT32F403A/cmsis/cm4/device_support
    ${SHARED_LIB_PATH}/AT32F403A/drivers/inc
    ${SHARED_LIB_PATH}/Library/ArteryCore
    ${SHARED_LIB_PATH}/Library/Uart
    ${SHARED_LIB_PATH}/Library/Usb
    ${SHARED_LIB_PATH}/Library/FlashService
    # ... additional library paths
)
```

#### Source Files

Projects typically include:
- CMSIS device support
- AT32 peripheral drivers
- Startup assembly file
- Required library components

### Compiler Requirements

- **Toolchain**: ARM GCC (arm-none-eabi-gcc)
- **C Standard**: C11
- **C++ Standard**: C++20 (or C++17)
- **Optimization**: 
  - Debug: `-Og` (optimize for debugging)
  - Release: `-O1` or `-O2` (optimize for size/speed)
- **Float ABI**: `-mfloat-abi=hard` (hardware FPU)
- **FPU**: `-mfpu=fpv4-sp-d16` (single-precision)

---

## Git Submodules

AT32F4-Drivers uses Git submodules for external dependencies:

```
[submodule "Library/sx126x_driver"]
    path = Library/sx126x_driver
    url = https://github.com/Lora-net/sx126x_driver

[submodule "Library/llcc68_driver"]
    path = Library/llcc68_driver
    url = https://github.com/Lora-net/llcc68_driver

[submodule "at32f403a-407-docs"]
    path = at32f403a-407-docs
    url = https://github.com/xyphrodite/at32f403a-407-docs.git

[submodule "Library/externals/CANopenNode"]
    path = Library/externals/CANopenNode
    url = https://github.com/CANopenNode/CANopenNode.git
```

### Submodule Management

**Initialize after clone**:
```bash
git submodule update --init --recursive
```

**Update submodules**:
```bash
git submodule update --remote
```

**Add new submodule** (example):
```bash
git submodule add <url> <path>
```

---

## Usage Examples

### 1. Basic System Initialization

```cpp
#include "SystemApi.h"

int main(void) {
    // Basic MCU initialization
    SystemApi::Init();
    
    // Configure services
    SystemServices_t services;
    services.UpTimeTmr = 1;
    services.FlashService = 1;
    services.WatchdogTimer = 1;
    
    // Initialize services
    SystemApi::InitServices(services);
    
    // Application code
    while (1) {
        // Main loop
    }
}
```

### 2. UART Communication

```cpp
#include "UartApi.h"

usart_init_type uart_config = {
    .usart = USART1,
    .baudrate = 115200,
    .data_bit = USART_DATA_8BITS,
    .stop_bit = USART_STOP_1_BIT,
    .parity = USART_PARITY_NONE
};

UartService::Init(&uart_config);

// Transmit data
const char* message = "Hello World\r\n";
UartService::Transmit((uint8_t*)message, strlen(message));

// Receive data
uint8_t buffer[64];
uint16_t received = UartService::Receive(buffer, sizeof(buffer));
```

### 3. FreeRTOS Task Creation

```cpp
#include <FreeRTOS.h>
#include <task.h>

void myTask(void *parameters) {
    for (;;) {
        // Task code here
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main(void) {
    // System initialization
    SystemApi::Init();
    
    // Create task
    xTaskCreate(
        myTask,                 // Task function
        "myTask",               // Task name
        256,                    // Stack size (words)
        NULL,                   // Parameters
        tskIDLE_PRIORITY + 1,   // Priority
        NULL                    // Task handle
    );
    
    // Start scheduler
    vTaskStartScheduler();
    
    // Should never reach here
    while (1);
}
```

### 4. Flash Storage

```cpp
#include "FlashService.h"

// Write configuration to flash
typedef struct {
    uint32_t magic;
    uint16_t baudrate;
    uint8_t device_address;
    uint16_t crc;
} Config_t;

Config_t config = {
    .magic = 0xDEADBEEF,
    .baudrate = 9600,
    .device_address = 1
};

// Calculate CRC
config.crc = CRC16(&config, sizeof(config) - sizeof(config.crc));

// Write to flash sector 253
FlashService::Write(FLASH_SECTOR_253, &config, sizeof(config));

// Read from flash
Config_t loaded_config;
FlashService::Read(FLASH_SECTOR_253, &loaded_config, sizeof(loaded_config));

// Verify CRC
uint16_t crc_check = CRC16(&loaded_config, sizeof(loaded_config) - sizeof(loaded_config.crc));
if (crc_check == loaded_config.crc) {
    // Configuration is valid
}
```

### 5. ADC Reading

```cpp
#include "AdcDriver.h"

// Initialize ADC
AdcDriver adc;
adc.Init(ADC1, ADC_CHANNEL_1);

// Read analog value
uint16_t raw_value = adc.Read();

// Convert to millivolts (assuming 3.3V reference)
uint32_t millivolts = (raw_value * 3300) / 4096;
```

---

## Memory Requirements

### Typical Footprint

| Component | Flash Usage | RAM Usage |
|-----------|-------------|-----------|
| **Minimal** (bare metal) | ~20 KB | ~2 KB |
| **+ UART** | +5 KB | +1 KB |
| **+ USB CDC** | +15 KB | +2 KB |
| **+ FreeRTOS** | +10 KB | +5 KB + heap |
| **+ CANopen** | +50 KB | +10 KB |
| **Full System** | 100-200 KB | 20-50 KB |

### FreeRTOS Heap Configuration

- **Heap_1**: Static allocation only
- **Heap_2**: Dynamic allocation, no defragmentation
- **Heap_3**: Uses malloc/free
- **Heap_4**: Coalescence algorithm (recommended)
- **Heap_5**: Multiple non-contiguous memory regions

**Typical heap size**: 10-40 KB (project-specific)

---

## Known Modifications and Limitations

### ⚠️ Important Notes

**Legacy Version Warning**:
This is an older/customized version of AT32F4-Drivers maintained specifically for TafcoPigstore projects. Exercise caution when integrating newer library versions as compatibility issues may arise.

### Project-Specific Modifications

1. **UnitControlWave Disabled in LBS**
   - Legacy LoRaWAN Basic Stack integration
   - Disabled in current builds

2. **Custom Peripheral Configurations**
   - Project-specific clock speeds
   - Custom GPIO mappings
   - Modified bootloader support

3. **Middleware Adaptations**
   - FreeRTOS configuration varies per project
   - USB descriptors customized per application
   - Custom protocol implementations

### Compatibility Notes

- **New Projects**: Recommended to use latest Artery SDK when starting fresh
- **Existing Projects**: Maintain current library version for stability
- **Updates**: Test thoroughly when updating library components
- **Documentation**: Always refer to project-specific documentation

---

## Recent Updates

### November 24-25, 2025: USB-CAN Converter Utility
- ✅ Created complete USB-CAN bridge converter application
- ✅ 1,575 lines of production-ready code
- ✅ Comprehensive documentation (3 major documents)
- ✅ CMake build system for cross-platform development
- ✅ ES0002 errata workarounds implemented
- ✅ Doxygen-documented public API

### October-November 2025: Documentation Enhancement
- ✅ at32f403a-407-docs submodule created and integrated
- ✅ Context7-optimized firmware library (1,985 examples)
- ✅ 100% errata documentation conversion (PDF → Markdown)
- ✅ Taxonomy system for semantic search
- ✅ FAQ and development guides

### Earlier 2025: Core System Updates
- ✅ SystemApi unified initialization system
- ✅ SystemServices bitfield configuration
- ✅ ErrorJournal enhanced with timestamps
- ✅ Flash memory layout reorganization
- ✅ Centralized linker scripts
- ✅ Bootloader build fixes

---

## Projects Using AT32F4-Drivers

### 1. P-Block (Main Application)
**Path**: `../P-block/`  
**Purpose**: Modbus RTU peripheral device for industrial automation  
**Features**:
- 12 relays + 1 emergency relay
- 11 universal inputs (analog/digital/temperature)
- 6 analog outputs (0-10V)
- Modbus RTU @ 9600 baud
- FreeRTOS multitasking
- Watchdog monitoring

**Uses from AT32F4-Drivers**:
- AT32F403A drivers
- FreeRTOS
- UART service
- Flash service
- SystemApi
- Watchdog system
- Command handler

### 2. Bootloader0
**Path**: `../bootloader0/`  
**Purpose**: Firmware bootloader with USB and UART support  
**Features**:
- USB CDC for firmware updates
- UART debugging interface
- Flash memory management
- Vector table relocation
- ~24.3 KB flash usage

**Uses from AT32F4-Drivers**:
- AT32F403A drivers
- USB middleware
- UART service
- Flash service
- Watchdog driver
- Bootloader utilities

### 3. Prebootload
**Path**: `../prebootload/`  
**Purpose**: Pre-bootloader for initial system setup  
**Features**:
- Basic MCU initialization
- Clock configuration
- Flash service
- Watchdog setup
- 4 KB flash region

**Uses from AT32F4-Drivers**:
- AT32F403A drivers
- Clock configuration
- Flash service
- BPR driver
- Simple watchdog

### 4. USB-CAN Converter
**Path**: `utils/usb-can-converter/`  
**Purpose**: USB-CAN bridge converter utility  
**Features**:
- USB CDC Virtual COM Port
- CAN bus @ 500 kbps
- Text-based protocol
- Production-ready

**Uses from AT32F4-Drivers**:
- AT32F403A drivers
- USB middleware
- Clock configuration
- GPIO drivers
- CAN drivers

---

## Development Workflow

### Setting Up a New Project

1. **Create project directory** in parent repository
2. **Reference AT32F4-Drivers**:
   ```cmake
   set(SHARED_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../AT32F4-Drivers)
   ```
3. **Include required components**:
   ```cmake
   include(${SHARED_LIB_PATH}/cmake_shared.cmake)
   ```
4. **Add peripheral drivers** as needed
5. **Configure linker script** from `ldscripts/`
6. **Initialize submodules**:
   ```bash
   git submodule update --init --recursive
   ```

### Building Projects

```bash
# Navigate to project directory
cd P-block  # or bootloader0, prebootload, etc.

# Create build directory
mkdir -p build && cd build

# Configure with CMake
cmake -G "Unix Makefiles" ..

# Build
make

# Output files
ls -lh *.elf *.hex *.bin
```

### Debugging

**Options**:
1. **Segger RTT**: High-speed debug output
2. **UART**: Serial debug printing
3. **USB**: Virtual COM port debugging
4. **GDB**: Source-level debugging via ST-Link/J-Link

**Tools**:
- OpenOCD
- ST-Link utilities
- J-Link GDB server
- Segger Ozone

---

## Related Documentation

### Internal Documentation
- **Main Project README**: `../README.md`
- **Main Project Context**: `../ProjectContext.md`
- **P-Block Documentation**: `../P-block/Docs/`
- **Bootloader Documentation**: `../bootloader0/Docs/`
- **USB-CAN Converter**: `utils/usb-can-converter/README.md`

### External References
- **AT32F403A/407 Documentation**: `at32f403a-407-docs/` (submodule)
- **FreeRTOS Documentation**: https://www.freertos.org/
- **CANopenNode**: https://github.com/CANopenNode/CANopenNode
- **Artery Technology**: https://www.arterytek.com/
- **ARM CMSIS**: https://arm-software.github.io/CMSIS_5/

---

## License and Copyright

See individual source files for copyright and license information. Components include:

- **AT32 CMSIS and Drivers**: © ArteryTek Technology
- **FreeRTOS**: MIT License (Amazon Web Services)
- **Segger RTT**: © Segger Microcontroller GmbH
- **USB Middleware**: Various licenses (see source files)
- **CANopenNode**: Apache License 2.0
- **LoRa Drivers**: Semtech copyright
- **TafcoPigstore Custom Code**: Project-specific license

---

## Contributing

When contributing to AT32F4-Drivers:

1. **Maintain Compatibility**: Ensure changes don't break existing projects
2. **Document Changes**: Update this ProjectContext.md
3. **Test Thoroughly**: Verify with all dependent projects
4. **Follow Standards**: Use existing coding style
5. **Update Examples**: Provide usage examples for new features

---

## Contact and Support

For issues related to:
- **AT32F4-Drivers Library**: Refer to main project documentation
- **AT32 MCU Documentation**: Visit [Artery Technology](https://www.arterytek.com/)
- **FreeRTOS Support**: See [FreeRTOS.org](https://www.freertos.org/)
- **CANopenNode**: See [CANopenNode GitHub](https://github.com/CANopenNode/CANopenNode)

---

**Last Updated**: November 25, 2025  
**Version**: Legacy/Customized for TafcoPigstore  
**Status**: Production-ready and actively maintained

