# USB-CAN Converter

A simple USB to CAN bus converter application for the AT32F403A/407 microcontroller.

## Overview

This application creates a bidirectional bridge between a USB Virtual COM Port (CDC) and a CAN bus interface. It allows you to:

- Send data from USB to CAN bus
- Receive data from CAN bus and transmit via USB
- Monitor CAN bus traffic through a virtual COM port

## Features

- **USB CDC (Virtual COM Port)**: Appears as a standard COM port on host PC
- **CAN Bus Support**: Full CAN 2.0A/B support with configurable baudrate
- **Bidirectional Communication**: Real-time data transfer in both directions
- **Error Handling**: Implements workarounds for AT32F403A CAN bit stuffing errors
- **Status LED**: Visual indication of operation (PC13)
- **Circular Buffers**: Efficient data handling for both directions

## Hardware Requirements

- AT32F403A or AT32F407 microcontroller board
- USB connection (typically micro-USB or USB-C)
- CAN transceiver connected to PB8 (CAN RX) and PB9 (CAN TX)
- 8MHz external crystal (HEXT)
- Status LED on PC13 (optional)

## Pin Configuration

| Function | Pin | Description |
|----------|-----|-------------|
| CAN RX | PB8 | CAN receiver input |
| CAN TX | PB9 | CAN transmitter output |
| USB D- | PA11 | USB data negative |
| USB D+ | PA12 | USB data positive |
| LED | PC13 | Status indicator |

## CAN Configuration

- **Baudrate**: 500 kbps (configurable)
- **Filter**: Accept all messages
- **Mode**: Normal communication mode
- **Error handling**: Automatic recovery from bit stuffing errors

## Data Protocol

### USB to CAN Format

Data sent from USB to CAN follows this format:

```
[ID_HIGH][ID_LOW][DLC][DATA0]...[DATA7]
```

- **ID_HIGH, ID_LOW**: 2 bytes for CAN standard ID (11-bit)
- **DLC**: 1 byte for data length (0-8)
- **DATA0-DATA7**: 0 to 8 bytes of CAN data

Example: Send CAN message with ID 0x123 and data [0x11, 0x22]:
```
01 23 02 11 22
```

### CAN to USB Format

CAN messages received are formatted as ASCII strings:

```
CAN ID:xxx DLC:x DATA:xx xx xx ...\r\n
```

Example output:
```
CAN ID:123 DLC:2 DATA:11 22
CAN ID:456 DLC:8 DATA:01 02 03 04 05 06 07 08
```

## Building the Project

### Prerequisites

- CMake 3.22 or higher
- Ninja build system
- ARM GNU Toolchain (arm-none-eabi-gcc)

### Build Steps

1. Configure the project:
```bash
cmake --preset Debug
```

2. Build:
```bash
cmake --build build/Debug
```

3. Output files will be in `build/Debug/`:
   - `USB-CAN-Converter.elf` - Executable with debug symbols
   - `USB-CAN-Converter.hex` - Intel HEX format for programming
   - `USB-CAN-Converter.bin` - Raw binary format
   - `USB-CAN-Converter.map` - Memory map file

### Flash Programming

Use your preferred programming tool to flash the `.hex` or `.bin` file:

- OpenOCD
- ST-Link
- J-Link
- CMSIS-DAP

## Usage

1. Flash the firmware to your AT32F403A/407 board
2. Connect the USB cable to your PC
3. The board will enumerate as a virtual COM port
4. Open the COM port with any terminal application:
   - Baudrate: Any (ignored by USB CDC)
   - Data bits: 8
   - Stop bits: 1
   - Parity: None

5. Send CAN messages using the protocol described above
6. Receive CAN messages as formatted ASCII text

## Testing

### Loopback Test

Connect CAN RX and CAN TX pins together through a CAN transceiver in loopback mode to test basic functionality.

### With Another CAN Device

1. Connect to a CAN bus with other devices
2. Monitor traffic in your terminal application
3. Send test messages from USB

## Troubleshooting

### Device Not Recognized

- Check USB connection
- Verify 48MHz USB clock configuration
- Ensure pull-up resistor on USB D+ (typically internal)

### CAN Bus Errors

- Verify CAN transceiver connections
- Check termination resistors (120Ω at both ends)
- Confirm baudrate matches other devices on bus
- Ensure HEXT (8MHz) is stable

### No Data Transfer

- Check circular buffer sizes if high traffic
- Verify CAN filter configuration
- Monitor error interrupts

## Known Limitations

- Standard CAN ID only (11-bit), extended ID support not implemented
- Maximum 32 CAN messages can be queued
- USB buffer size is 512 bytes
- Shared interrupt handler for USB and CAN RX0 (careful priority setting)

## License

See project root for license information.

## References

- [AT32F403A/407 Datasheet](https://www.arterytek.com)
- [AT32F403A/407 Reference Manual](https://www.arterytek.com)
- [AT32F403A/407 Errata Document](https://github.com/xyphrodite/at32f403a-407-docs)
- [USB CDC Class Specification](https://www.usb.org/document-library/class-definitions-communication-devices-12)
- [CAN Bus Specification](https://www.can-cia.org)

## Contributing

Contributions are welcome! Please follow the coding standards defined in the project root.

## Version History

- **v1.0.0** (2025-11-24): Initial release
  - USB CDC implementation
  - CAN bus support
  - Bidirectional data transfer
  - Error handling and workarounds

