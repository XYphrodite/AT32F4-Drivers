#ifndef _GENERAL_SPI_STRUCT_H_
#define _GENERAL_SPI_STRUCT_H_

#include "unitControl.h"

// #ifdef __cplusplus
// extern "C" {
// #endif

    typedef struct general_spi_cfg {
        spi_type* spi;
        uint16_t mosi_pin;
        gpio_type* mosi_type;
        uint16_t miso_pin;
        gpio_type* miso_type;
        uint16_t sck_pin;
        gpio_type* sck_type;
        uint16_t cs_pin;
        gpio_type* cs_type;
        uint8_t unit_name; //UNIT_NAME
    } general_spi_cfg_t;

// #ifdef __cplusplus
// }
// #endif

#endif