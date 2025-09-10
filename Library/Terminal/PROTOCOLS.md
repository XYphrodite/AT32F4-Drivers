# Packet Protocols Documentation

This document describes the packet protocols used in `PacketWrapper.cpp` for LoRaWAN and related communication. The code supports multiple packet formats, selectable via preprocessor defines:
- `CayenneLPP_PACKET`
- `TINY_PACKET`
- `EXTRA_SMALL_PACKET`

## 1. CayenneLPP_PACKET

**Format:** [Cayenne Low Power Payload (LPP)](https://community.mydevices.com/t/cayenne-lpp-cayenne-low-power-payload/7510)

- **Prefix:** `0x10` (code)
- **Version:** `0x01`
- **Serial Number:** 4 bytes (uint32_t)
- **Payload:** CayenneLPP buffer (variable size, typically 16 bytes in this implementation)

**Fields:**
| Field         | Size (bytes) | Description                       |
|---------------|--------------|-----------------------------------|
| Code          | 1            | Packet type (0x10)                |
| Version       | 1            | Protocol version (1)              |
| Serial Number | 4            | Device serial number              |
| Payload       | variable     | CayenneLPP-encoded sensor data    |


**Construction:**
- Uses the `CayenneLPP` class to encode sensor data.
- Serial number is obtained from `ModemConfig::GetSerialNum()`.

---

## 2. TINY_PACKET

**Format:** Custom, compact binary protocol (TafcoTinyProtocol, version 2)

- **Prefix:** `0x10` (code)
- **Version:** `0x02`
- **Serial Number:** 4 bytes (uint32_t)
- **Payload:** `TafcoTinyProtocol` struct (size defined by implementation)

**Fields:**
| Field         | Size (bytes) | Description                       |
|---------------|--------------|-----------------------------------|
| Code          | 1            | Packet type (0x10)                |
| Version       | 1            | Protocol version (2)              |
| Serial Number | 4            | Device serial number              |
| Payload       | 5            | Sensor data struct                |

**Construction:**
- Uses the `TafcoTinyProtocol_Create(metrics)` function to fill the struct.
- Serial number is obtained from `ModemConfig::GetSerialNum()`.

**Error Value Encoding:**
- If a voltage value is invalid (NaN in code), it is encoded as `0xFF` in the protocol struct.
- When decoding, a value of `0xFF` in a voltage field is interpreted as NaN (error/invalid value).
- If a temperature value is invalid (set to -100.0 in code), it is encoded as -100.0 in the protocol struct.
- When decoding, a value of -100.0 in a temperature field should be interpreted as an error/invalid value.

---

## 3. EXTRA_SMALL_PACKET

**Format:** Ultra-compact binary protocol (TafcoTinyProtocol, smaller serial number)

- **Prefix:** `0x11` (code)
- **Serial Number:** 2 bytes (uint16_t)
- **Payload:** `TafcoTinyProtocol` struct (size defined by implementation)

**Fields:**
| Field         | Size (bytes) | Description                       |
|---------------|--------------|-----------------------------------|
| Code          | 1            | Packet type (0x11)                |
| Serial Number | 2            | Device serial number (short)      |
| Payload       | 5            | Sensor data struct                |

**Construction:**
- Uses the `TafcoTinyProtocol_Create(metrics)` function to fill the struct.
- Serial number is obtained from `ModemConfig::GetSerialNum()` and cast to 2 bytes.

**Error Value Encoding:**
- If a voltage value is invalid (NaN in code), it is encoded as `0xFF` in the protocol struct.
- When decoding, a value of `0xFF` in a voltage field is interpreted as NaN (error/invalid value).
- If a temperature value is invalid (set to -100.0 in code), it is encoded as -100.0 in the protocol struct.
- When decoding, a value of -100.0 in a temperature field should be interpreted as an error/invalid value.

---

## 5. Notes
- The actual structure of `TafcoTinyProtocol` is defined elsewhere and may change.
- The protocol used is selected at compile time by defining one of the macros at the top of `PacketWrapper.cpp`.
- The code is designed for embedded systems with strict size constraints. 
