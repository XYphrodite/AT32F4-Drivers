#include "gpio.h"

void pinInit(gpio_type *gpio_x, uint32_t pins, PinMode_t mode, gpio_pull_type pull) {
  __IO crm_periph_clock_type clock_periph = {};
  if(gpio_x == GPIOA) {
    clock_periph = CRM_GPIOA_PERIPH_CLOCK;
  }
  else if(gpio_x == GPIOB) {
    clock_periph = CRM_GPIOB_PERIPH_CLOCK;
  }
  else if(gpio_x == GPIOC) {
    clock_periph = CRM_GPIOC_PERIPH_CLOCK;
  }
  else if(gpio_x == GPIOD) {
    clock_periph = CRM_GPIOD_PERIPH_CLOCK;
  }
  else if(gpio_x == GPIOE) {
    clock_periph = CRM_GPIOE_PERIPH_CLOCK;
  }

  gpio_init_type gpio_init_struct = {};
  /* set default parameter */
  gpio_default_para_init(&gpio_init_struct);
  /* enable the gpioa clock */
  crm_periph_clock_enable(clock_periph, TRUE);
  /* configure the gpio */
  gpio_init_struct.gpio_pins = pins;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_pull = pull;
  switch(mode) {
    case INPUT:
      gpio_init_struct.gpio_out_type = (gpio_output_type)0x00;
      gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
    break;
    case OUTPUT:
      gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
      gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    break;
    case OUTPUT_AF_PP:
      gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
      gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    break;
    case OUTPUT_AF_OD:
      gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
      // gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
      gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
    break;
    case OUTPUT_OPEN_DRAIN:
      gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
      // gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
      gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
    break;
    default:
      return;
    break;
  }
  gpio_init(gpio_x, &gpio_init_struct);
}

PinConfig::PinConfig(gpio_type *_port, uint16_t _pin) : port(_port), pin(_pin) {}

PinControl::PinControl(gpio_type *_port, uint16_t _pin, PinMode_t _mode, gpio_pull_type _pull) 
  : state(false), mode(_mode), pull(_pull) { 
  config.port = _port;
  config.pin = _pin;
  pinInit(config.port, static_cast<uint32_t>(config.pin), mode, pull);
}

PinControl::~PinControl() {}

void PinControl::Set(bool state) {
  this->state = state;
  if(state) {
    gpio_bits_set(config.port, static_cast<uint32_t>(config.pin));
  }
  else {
    gpio_bits_reset(config.port, static_cast<uint32_t>(config.pin));
  }
}

bool PinControl::Get() {
  return gpio_output_data_bit_read(config.port, static_cast<uint32_t>(config.pin));
}

void PinControl::Toggle() {
  Set(!state);
}

void PinControl::Reinit(gpio_type *gpio_x, uint32_t pins, PinMode_t mode, gpio_pull_type pull) {
  this->config.port = gpio_x;
  this->config.pin = pins;
  this->mode = mode;
  this->pull = pull;
  pinInit(this->config.port, static_cast<uint32_t>(this->config.pin), this->mode, this->pull);
}