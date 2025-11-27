#include "MS5614-Api.h"

MS5614::MS5614(uint8_t cs_pin, uint8_t sclk_pin, uint8_t din_pin, uint8_t fs_pin)
    : cs_pin_(cs_pin), sclk_pin_(sclk_pin), din_pin_(din_pin), fs_pin_(fs_pin) {
    spi =  GENERAL_SPI();
    spi.gsc->cs_pin = cs_pin;
    spi.gsc->sclk_pin = sclk_pin;
    spi.gsc->din_pin = din_pin;
    spi.gsc->fs_pin = fs_pin;
    gpioInit(cs_pin_, OUTPUT);

    gpioInit(fs_pin_, OUTPUT);
}

void MS5614::setOutput(Channel channel, uint16_t value, PowerMode power, RateMode rate) {
    uint16_t command = 0;
    command |= (static_cast<uint8_t>(channel) & 0x03) << 12; // A1, A0 bits
    command |= (static_cast<uint8_t>(power) & 0x01) << 11;  // PWR bit
    command |= (static_cast<uint8_t>(rate) & 0x01) << 10;   // SPD bit
    command |= (value & 0x0FFF);                           // 12-bit data

    sendCommand(command);
}

void MS5614::powerDown() {
    // Set all channels to power down mode
    for (int i = 0; i < 4; ++i) {
        setOutput(static_cast<Channel>(i), 0, PowerMode::PowerDown);
    }
}

void MS5614::sendCommand(uint16_t command) {
    // Assert CS (low)
    digitalWrite(cs_pin_, LOW);

    // Send 16 bits MSB first
    for (int i = 15; i >= 0; --i) {
        digitalWrite(sclk_pin_, LOW);
        digitalWrite(din_pin_, (command >> i) & 1);
        digitalWrite(sclk_pin_, HIGH);
    }

    // Assert FS (low, then high) to update DAC output
    digitalWrite(fs_pin_, LOW);
    digitalWrite(fs_pin_, HIGH);

    // Deassert CS (high)
    digitalWrite(cs_pin_, HIGH);
}
