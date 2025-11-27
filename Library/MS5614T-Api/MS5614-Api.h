#include <cstdint>
#include "general_spi.h"

class MS5614 {
public:
    GENERAL_SPI* spi;

    enum class Channel { A = 0, B = 1, C = 2, D = 3 };
    enum class PowerMode { Normal = 0, PowerDown = 1 };
    enum class RateMode { Slow = 0, Fast = 1 };

    // Constructor
    MS5614(uint8_t cs_pin, uint8_t sclk_pin, uint8_t din_pin, uint8_t fs_pin);

    // Set output voltage for a channel (0-4095 for 12-bit DAC)
    void setOutput(Channel channel, uint16_t value, PowerMode power = PowerMode::Normal, RateMode rate = RateMode::Slow);

    // Power down all channels
    void powerDown();

private:
    uint8_t cs_pin_;
    uint8_t sclk_pin_;
    uint8_t din_pin_;
    uint8_t fs_pin_;

    // Helper to send 16-bit command over SPI-like interface
    void sendCommand(uint16_t command);
};