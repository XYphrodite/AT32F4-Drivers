#ifndef _GENERAL_SPI_STRUCT_H_
#define _GENERAL_SPI_STRUCT_H_

#include "gpio.h"
#include "unitControl.h"

// #ifdef __cplusplus
// extern "C" {
// #endif

    typedef struct general_spi_cfg {
        spi_type* spi;

        PinConfig mosi_pin;
        PinConfig miso_pin;
        PinConfig sck_pin;
        PinConfig cs_pin;

        uint8_t unit_name; //UNIT_NAME
    } general_spi_cfg_t;

// #ifdef __cplusplus
// }
// #endif

#endif