#ifndef _LORA_STRUCTS_H_
#define _LORA_STRUCTS_H_

#include "at32f403a_407.h"
#include "general_spi_struct.h"

typedef struct
{
    general_spi_cfg_t gsc;
    uint16_t rst_pin;
    gpio_type *rst_type;
    uint16_t busy_pin;
    gpio_type *busy_type;
    uint16_t dio1_pin;
    gpio_type *dio1_type;

    uint16_t rxen_pin;
    gpio_type *rxen_type;
    uint16_t txen_pin;
    gpio_type *txen_type;

    uint16_t pwren_pin;
    gpio_type *pwren_port;
} LoraModule_Cfg;

#endif
