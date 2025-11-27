#include "MS5614-Api.h"
#include "general_spi.h"
#include "gpio.h"

MS5614::MS5614(uint8_t cs_pin, uint8_t sclk_pin, uint8_t din_pin, uint8_t fs_pin)
    : spi(), cs_pin_(cs_pin), sclk_pin_(sclk_pin), din_pin_(din_pin), fs_pin_(fs_pin) {
        spi = GENERAL_SPI();

        spi.gsc->cs_pin = PinConfig()
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
    uint8_t command_bytes[2];

    command_bytes[0] = (command >> 8) & 0xFF;
    command_bytes[1] = command & 0xFF;

    spi.set_cs(SET);
    if (spi.hal_write(&command_bytes[0]) != SUCCESS) {
        spi.set_cs(RESET);
        return;
    }
    if (spi.hal_write(&command_bytes[1]) != SUCCESS) {
        spi.set_cs(RESET);
        return;
    }
    if (spi.waitUntilLastByteSend() != SUCCESS) {
        spi.set_cs(RESET);
        return;
    }
    spi.set_cs(RESET);
}
