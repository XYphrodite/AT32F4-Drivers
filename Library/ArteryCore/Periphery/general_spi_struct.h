#ifndef _GENERAL_SPI_STRUCT_H_
#define _GENERAL_SPI_STRUCT_H_

#include "gpio.h"
#include "unitControl.h"

// #ifdef __cplusplus
// extern "C" {
// #endif

    /// @brief General SPI configuration structure
    /// @details Contains SPI peripheral and pin configuration for SPI communication
    typedef struct general_spi_cfg {
        spi_type* spi;              ///< Pointer to SPI peripheral (SPI1, SPI2, etc.)

        PinConfig mosi_pin;          ///< Master Out Slave In (MOSI) pin configuration
        PinConfig miso_pin;          ///< Master In Slave Out (MISO) pin configuration
        PinConfig sck_pin;           ///< Serial Clock (SCK) pin configuration
        PinConfig cs_pin;            ///< Chip Select (CS) pin configuration

        uint8_t unit_name;           ///< Unit identifier (UNIT_NAME)
    } general_spi_cfg_t;

// #ifdef __cplusplus
// }
// #endif

#endif