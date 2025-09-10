#pragma once

template<typename T>
void WriteToBuffer(uint8_t*& cursor, const T& value) {
    memcpy(cursor, &value, sizeof(T));
    cursor += sizeof(T);
}

template<typename T>
void ReadFromBuffer(const uint8_t*& buffer, T& value) {
    memcpy(&value, buffer, sizeof(T));
    buffer += sizeof(T);
}

template<typename T>
void ReadFromFlash(const uint32_t& address, T& value) {
    FlashService::Read(address, reinterpret_cast<uint8_t*>(&value), sizeof(T));
}

template<typename T>
void SerialReadFromFlash(uint32_t& address, T& value) {
    FlashService::Read(address, reinterpret_cast<uint8_t*>(&value), sizeof(T));
    address += sizeof(T);
}