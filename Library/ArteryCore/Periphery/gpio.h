#ifndef _GPIO_H_
#define _GPIO_H_

#include "at32f403a_407_conf.h"
#include "stdint.h"

typedef enum {
  INPUT,
  // INPUT_PULLUP,
  // INPUT_PULLDOWN,
  INPUT_ANALOG,
  INPUT_ANALOG_DMA,
  OUTPUT,
  OUTPUT_OPEN_DRAIN,
  OUTPUT_AF_OD,
  OUTPUT_AF_PP,
  // PWM
} PinMode_t;

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Инициализировать контакт
/// @param [in] gpio_x    :: Порт контакта (GPIOA...)
/// @param [in] pins      :: Номер контактов (GPIO_PINS_1 или GPIO_PINS_1 |
/// GPIO_PINS_2)
/// @param [in] mode      :: Режим работы: вход - выход
/// @param [in] pull      :: Подтяжка
void pinInit(gpio_type *gpio_x, uint32_t pins, PinMode_t mode,
             gpio_pull_type pull);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

struct PinConfig {
public:
  gpio_type *port;
  uint16_t pin;

  PinConfig();
  PinConfig(gpio_type *_port, uint16_t _pin);

  ~PinConfig();
};

struct PinControl {
private:
  PinConfig config;
  bool state;
  PinMode_t mode;
  gpio_pull_type pull;

public:
  PinControl(gpio_type *_port, uint16_t _pin, PinMode_t _mode, gpio_pull_type _pull);
  ~PinControl();

  void Set(bool state);
  bool Get();
  void Toggle();
  void Reinit(gpio_type *gpio_x, uint32_t pins, PinMode_t mode,
             gpio_pull_type pull);
};

void pinInit(PinConfig config, PinMode_t mode, gpio_pull_type pull);

#endif



#endif //_GPIO_H_