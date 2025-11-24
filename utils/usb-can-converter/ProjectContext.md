# USB-CAN Converter - Project Context

## Project Overview

**Project Name**: USB-CAN Converter  
**Target MCU**: AT32F403A/407 (ARM Cortex-M4F)  
**Purpose**: Bidirectional USB Virtual COM Port to CAN bus bridge  
**Created**: November 24, 2025  
**Language**: C++17 / C11

## Architecture

### System Block Diagram

```
┌─────────────┐        ┌──────────────┐        ┌─────────────┐
│             │  USB   │              │  CAN    │             │
│  Host PC    │◄──────►│  AT32F407    │◄───────►│  CAN Bus    │
│  (COM Port) │        │  Converter   │         │  Devices    │
│             │        │              │         │             │
└─────────────┘        └──────────────┘         └─────────────┘
```

### Software Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                         Application Layer                    │
│  ┌──────────────┐    ┌─────────────┐    ┌────────────────┐ │
│  │ Main Loop    │◄──►│ USB-CAN     │◄──►│ Statistics &   │ │
│  │ Processing   │    │ Bridge Logic│    │ Error Handling │ │
│  └──────────────┘    └─────────────┘    └────────────────┘ │
└─────────────────────────────────────────────────────────────┘
                              ▲
                              │
┌─────────────────────────────┼─────────────────────────────┐
│                         HAL Layer                           │
│  ┌──────────────┐                    ┌─────────────────┐  │
│  │  USB CDC     │                    │  CAN Controller │  │
│  │  Driver      │                    │  Driver         │  │
│  └──────────────┘                    └─────────────────┘  │
└─────────────────────────────────────────────────────────────┘
                              ▲
                              │
┌─────────────────────────────┼─────────────────────────────┐
│                    Hardware Abstraction                     │
│  ┌────────────┐  ┌─────────┐  ┌────────────┐  ┌────────┐ │
│  │ USB        │  │ CAN     │  │ GPIO       │  │ Clock  │ │
│  │ Peripheral │  │ Periph. │  │ (Status)   │  │ Config │ │
│  └────────────┘  └─────────┘  └────────────┘  └────────┘ │
└─────────────────────────────────────────────────────────────┘
```

## Technical Details

### Clock Configuration

- **System Clock (SCLK)**: 144 MHz
- **Source**: HEXT (8MHz) × PLL (36) = 144MHz
- **AHB Clock**: 144 MHz (AHB divider = 1)
- **APB1 Clock**: 72 MHz (APB1 divider = 2) - for CAN
- **APB2 Clock**: 72 MHz (APB2 divider = 2)
- **USB Clock**: 48 MHz (derived from system clock)

### Memory Configuration

- **Flash**: 1024 KB
- **SRAM**: 96 KB
- **Stack Size**: 0x400 (1KB)
- **Heap Size**: 0x200 (512B)

### USB CDC Configuration

- **Device Class**: Communications Device Class (CDC)
- **Subclass**: Abstract Control Model (ACM)
- **Protocol**: Virtual COM Port
- **Endpoint Configuration**:
  - EP0: Control endpoint (64 bytes)
  - EP1 (0x81): Bulk IN (64 bytes) - Data to host
  - EP2 (0x02): Bulk OUT (64 bytes) - Data from host
  - EP3 (0x83): Interrupt IN (64 bytes) - Status notifications

### CAN Configuration

- **Controller**: CAN1
- **Pins**:
  - RX: PB8 (remapped via CAN1_GMUX_0010)
  - TX: PB9 (remapped via CAN1_GMUX_0010)
- **Baudrate**: 500 kbps
  - Sample Point: ~75% (8 TQ / 3 TQ)
  - Bit time quanta: 12 (1 + 8 + 3)
  - Prescaler: 6
  - Formula: 72MHz / (6 × 12) = 1 Mbps
- **Filters**: Accept all messages (ID mask 0x00000000)
- **FIFOs**: FIFO0 for reception

### Interrupt Configuration

**Priority Group**: NVIC_PRIORITY_GROUP_4 (4 bits for preemption, 0 bits for sub-priority)

| Interrupt | Priority | Handler | Description |
|-----------|----------|---------|-------------|
| CAN1_SE | 0 (Highest) | CAN error handler | Critical CAN error recovery |
| USBFS_L_CAN1_RX0 | 1 | USB/CAN RX handler | Shared interrupt for USB and CAN RX |

**Important Note**: The USBFS_L_CAN1_RX0_IRQn interrupt is shared between USB Low Priority and CAN1 RX0. The handler checks both sources and dispatches accordingly.

### Data Flow

#### USB to CAN Direction

1. USB CDC receives data from host via bulk OUT endpoint
2. Data is stored in circular buffer (`usb_rx_buffer`)
3. Main loop parses the data format: [ID][ID][DLC][DATA...]
4. CAN message is constructed and transmitted via CAN controller
5. Transmission status is checked with timeout

#### CAN to USB Direction

1. CAN RX interrupt triggers on message reception
2. CAN message is stored in receive queue (`can_rx_queue`)
3. Main loop retrieves message from queue
4. Message is formatted as ASCII string
5. String is transmitted via USB CDC bulk IN endpoint

### Buffer Management

- **USB RX Buffer**: 512 bytes circular buffer
- **CAN RX Queue**: 32 CAN messages (deep queue for burst traffic)
- **CAN TX Buffer**: 512 bytes temporary buffer (currently unused)

### Error Handling

#### CAN Bit Stuffing Error Workaround

The AT32F403A/407 has a known issue with CAN bit stuffing errors. The workaround implemented:

1. Enable high-priority CAN error interrupt (CAN1_SE)
2. Monitor error type register (ESTS)
3. Detect stuff error (error code 0x10)
4. Reset and reinitialize CAN peripheral immediately
5. Resume normal operation

Reference: [AT32F403A/407 Errata Sheet](https://github.com/xyphrodite/at32f403a-407-docs)

#### USB Error Handling

- Timeout on transmission (5000 iterations)
- Check USB connection status before sending
- Queue overflow detection for CAN RX messages

## File Structure

```
usb-can-converter/
├── CMakeLists.txt              # Build configuration
├── CMakePresets.json           # Build presets (Debug/Release)
├── README.md                   # User documentation
├── ProjectContext.md           # This file (technical documentation)
├── inc/                        # Header files
│   ├── main.h                  # Main application header
│   ├── usb_can_bridge.h        # Bridge logic header
│   ├── at32f403a_407_clock.h   # Clock configuration
│   ├── at32f403a_407_int.h     # Interrupt handlers
│   ├── at32f403a_407_conf.h    # HAL configuration
│   ├── at32f403a_407_board.h   # Board support
│   └── usb_conf.h              # USB configuration
└── src/                        # Source files
    ├── main.cpp                # Main application
    ├── usb_can_bridge.cpp      # Bridge implementation
    ├── at32f403a_407_clock.c   # Clock setup
    ├── at32f403a_407_int.c     # Interrupt handlers
    └── at32f403a_407_board.c   # Board support functions
```

## Dependencies

### External Libraries

- **AT32F403A HAL**: Located in `../../AT32F403A/`
  - CMSIS core and device support
  - Peripheral drivers (CRM, GPIO, CAN, USB, etc.)
  - Startup code and linker script

- **USB Middleware**: Located in `../../Middlewares/USB/`
  - USB Device Core (`usbd_core`, `usbd_int`, `usbd_sdr`)
  - CDC Class driver (`cdc_class`, `cdc_desc`)

### Build Tools

- **CMake**: 3.22+
- **Ninja**: Build system
- **ARM GNU Toolchain**: arm-none-eabi-gcc 10.3+

## Build Configuration

### Compiler Flags

**Common**:
- `-mthumb`: Thumb instruction set
- `-mcpu=cortex-m4`: Cortex-M4 target
- `-mfloat-abi=hard`: Hardware floating point
- `-mfpu=fpv4-sp-d16`: FPv4 single precision FPU
- `-fdata-sections`: Separate data sections
- `-ffunction-sections`: Separate function sections

**C Flags**:
- Standard: C11
- Warnings: `-Wall -Wextra -Wpedantic`

**C++ Flags**:
- Standard: C++17
- `-fno-rtti`: Disable RTTI
- `-fno-exceptions`: Disable exceptions
- `-fno-threadsafe-statics`: No thread-safe static initialization

### Linker Flags

- `-Wl,--gc-sections`: Remove unused sections
- `--specs=nosys.specs`: No system calls
- `--specs=nano.specs`: Newlib-nano (smaller C library)
- `-u _printf_float`: Enable float printf support

## Testing Strategy

### Unit Testing

Not implemented in current version. Consider adding:
- Buffer management tests
- Protocol parsing tests
- Error handling tests

### Integration Testing

1. **USB Enumeration Test**: Verify device appears as COM port
2. **Loopback Test**: CAN TX/RX connected via external loopback
3. **Real CAN Network Test**: Communication with actual CAN devices
4. **Stress Test**: High-frequency message transmission

### Debug Tools

- **LED Status**: PC13 toggles to indicate activity
- **SEGGER RTT**: Can be integrated for printf debugging
- **USB Analyzer**: Monitor USB traffic
- **CAN Analyzer**: Monitor CAN bus traffic

## Performance Considerations

### Throughput

- **USB CDC**: ~12 Mbps theoretical, ~8 Mbps practical
- **CAN Bus**: 500 kbps configured (adjustable)
- **Bottleneck**: CAN bus speed is the limiting factor

### Latency

- **USB to CAN**: ~1-2 ms (includes USB polling and CAN transmission)
- **CAN to USB**: <1 ms (interrupt-driven)

### CPU Usage

- **Idle**: Low (polling loop with minimal operations)
- **Active**: Moderate (string formatting is main overhead)

## Future Enhancements

### Planned Features

1. **Extended CAN ID Support**: 29-bit identifiers
2. **Configuration via USB**: Change CAN baudrate dynamically
3. **Binary Protocol**: More efficient than ASCII
4. **CAN Error Frame Detection**: Report error frames
5. **Timestamp Support**: Add timestamps to received messages
6. **Multiple CAN Controllers**: Support CAN1 and CAN2
7. **DMA Support**: Reduce CPU usage for data transfers
8. **RTOS Integration**: Better real-time characteristics

### Optimization Opportunities

1. Use DMA for USB data transfers
2. Optimize string formatting (sprintf is slow)
3. Implement zero-copy buffers where possible
4. Add compile-time protocol selection (binary vs ASCII)

## Compliance and Standards

- **USB 2.0**: Full-speed device (12 Mbps)
- **USB CDC 1.1**: Communications Device Class specification
- **CAN 2.0**: Parts A (11-bit ID) and B (29-bit ID - not yet implemented)
- **ISO 11898**: CAN protocol standard

## Known Issues

1. **Extended CAN ID**: Not implemented
2. **CAN Error Frames**: Not reported to USB
3. **USB Suspend**: Not properly handled
4. **Flow Control**: No XON/XOFF or RTS/CTS
5. **Statistics**: Counters not accessible from USB

## Debugging Notes

### Common Issues

1. **CAN Bus Silent**: Check termination resistors (120Ω)
2. **USB Not Enumerating**: Check USB clock (48MHz) and pull-up resistor
3. **Data Loss**: Increase buffer sizes or reduce CAN traffic
4. **Bit Stuffing Errors**: Workaround is implemented, but check bus integrity

### Debug Outputs

Add debug prints via UART or SEGGER RTT:
- USB enumeration events
- CAN error counters
- Buffer fill levels
- Message statistics

## References

### Documentation

- [TafcoPigstore Project Root](../../)
- [AT32F403A/407 Docs](../../at32f403a-407-docs/)
- [AT32 HAL Library](../../AT32F403A/)
- [USB Middleware](../../Middlewares/USB/)

### External Resources

- [AT32 Official Website](https://www.arterytek.com)
- [CAN Bus Tutorial](https://www.can-cia.org)
- [USB CDC Specification](https://www.usb.org/document-library/class-definitions-communication-devices-12)

## Change Log

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0.0 | 2025-11-24 | AI Assistant | Initial implementation |

## Maintenance

### Regular Tasks

- Monitor CAN error counters
- Check buffer utilization under load
- Verify USB enumeration stability
- Test with different CAN devices

### Update Procedures

1. Backup current firmware
2. Test new firmware in development environment
3. Verify backward compatibility
4. Update documentation
5. Release with version number

---

**Last Updated**: November 24, 2025  
**Document Version**: 1.0  
**Maintainer**: Project Team

