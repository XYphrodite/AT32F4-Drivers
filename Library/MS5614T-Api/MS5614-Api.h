#include "general_spi.h"
#include "gpio.h"
#include <cstdint>


class MS5614 {
public:
  enum class Channel { A = 0, B = 1, C = 2, D = 3 };
  enum class PowerMode { Normal = 0, PowerDown = 1 };
  enum class RateMode { Slow = 0, Fast = 1 };

  // Constructor
  MS5614(PinConfig cs_pin, PinConfig sclk_pin, PinConfig din_pin, PinConfig fs_pin);

  // Set output voltage for a channel (0-4095 for 12-bit DAC)
  void setOutput(Channel channel, uint16_t value,
                 PowerMode power = PowerMode::Normal,
                 RateMode rate = RateMode::Slow);

  // Power down all channels
  void powerDown();

private:
  GENERAL_SPI spi;
  PinConfig fs_pin;

  // Helper to send 16-bit command over SPI-like interface
  void sendCommand(uint16_t command);
};