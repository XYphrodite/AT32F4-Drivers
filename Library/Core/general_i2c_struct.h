#ifndef _GENERAl_I2C_STRUCT_
#define _GENERAl_I2C_STRUCT_

#include <at32f403a_407.h>

typedef struct general_i2c_cfg
{
    i2c_type* i2c;
    gpio_type* scl_port;
    uint16_t scl_pin;
    gpio_type* sda_port;
    uint16_t sda_pin;
    crm_periph_clock_type i2c_clock;
    crm_periph_clock_type port_clock;
    uint32_t baudrate;
} general_i2c_cfg_t;

#endif