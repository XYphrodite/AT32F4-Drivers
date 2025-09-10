#ifndef _UNIT_CONTROL_H_
#define _UNIT_CONTROL_H_

#include "at32f403a_407.h"
#include <stdbool.h>
#include "uartDrv.h"
#include "CRC.h"
#include "lora_spi.h"
#if (END_DEVICE_TEMPERATURE_SENSOR) && (!BOOTLOADER)
#include "nst118_hal.h"
#endif
// #include "lis2dh12_spi_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define RST_UNITS_NUM (5U)

#define MAX_ERR (1U)

    typedef enum
    {
        SPI_LORA_UNIT = 0x1,  // Lora
        SPI_ACCEL_UNIT = 0x2, // accelerometer
        LORA_UNIT = 0x3,
        UART_UNIT = 0x4,
        MCU_UNIT = 0x5,
        I2C1_UNIT = 0x6
    } UNIT_NAME;

    typedef struct UnitRST
    {
        UNIT_NAME name;
        uint32_t allErr;
        uint8_t errCnt;
        uint8_t maxErr;
        uint32_t rstCnt;
        void (*init_func)();
        // uint32_t last_error;
    } UnitRST_t;
#pragma pack(push, 1)
    typedef struct UnitRST_View
    {
        UNIT_NAME name;
        uint32_t allErr;
        uint8_t errCnt;
        uint32_t rstCnt;
    } UnitRST_View_t;
#pragma pack(pop)

    // extern UnitInitControl_t initControl;

    void init_rst_units(void);
    void add_error_to_unit_control(UNIT_NAME name);
    void unit_control_wave(void);
    void get_unit_rst_views(UnitRST_View_t *views);

    extern UnitRST_t rst_units[RST_UNITS_NUM];

#ifdef __cplusplus
}
#endif

#endif
