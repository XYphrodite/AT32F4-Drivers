#ifndef _GPIO_H_
#define _GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "at32f403a_407_conf.h"

  // #ifdef __cplusplus
  // extern "C" {
  // #endif

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

  // class GPIO {
  //   public:
  //   //static void Init(gpio_type *gpio_x, uint32_t pin, PinMode_t mode, gpio_drive_type strength);
  // };

  /// @brief Инициализировать контакт
  /// @param [in] gpio_x    :: Порт контакта (GPIOA...)
  /// @param [in] pins      :: Номер контактов (GPIO_PINS_1 или GPIO_PINS_1 | GPIO_PINS_2)
  /// @param [in] mode      :: Режим работы: вход - выход
  /// @param [in] pull      :: Подтяжка
  void pinInit(gpio_type* gpio_x, uint32_t pins, PinMode_t mode, gpio_pull_type pull);

#ifdef __cplusplus
}
#endif

/// @brief Push-Pull pin
// class PinPP {
// private:
//   gpio_type* port;
//   uint32_t    pin;
// public:
//   PinPP() {};
//   PinPP(gpio_type* _port, uint32_t _pin) : port(_port), pin(_pin) {
//     pinInit(port, pin, OUTPUT, GPIO_PULL_NONE);
//   }
//   void Init(gpio_type* _port, uint32_t _pin) {
//     port = _port;
//     pin = _pin;
//     pinInit(port, pin, OUTPUT, GPIO_PULL_NONE);
//   }
//   void On() {
//     gpio_bits_set(this->port, this->pin);
//   }
//   void Off() {
//     gpio_bits_reset(this->port, this->pin);
//   }
//   bool GetState() {
//     return gpio_output_data_bit_read(port, pin);
//   }
// };

// #ifdef __cplusplus
//   }
// #endif

#endif //_GPIO_H_