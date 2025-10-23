#include "unitControl.h"
#include "UartDrv.h"
#include <string.h>

typedef void (*UnitInitNoArgFunc)(void);
static void unit_noarg_adapter(void* fn)
{
    if (fn == 0) return;
    ((UnitInitNoArgFunc)fn)();
}

// UnitInitControl_t initControl;

// void CheckInitAndInit(UnitInitControl_t _unit) {
//     if (_unit.init_crc && !initControl.init_crc) {
//         init_CRC();
//     }
//     if (_unit.init_uart && !initControl.init_uart) {
//         drv_uart_init();
//     }
//     // if (_unit.init_spi1 && !initControl.init_spi1) {
//     //     // LoraModule lm = LoraModule().Get;
//     //     // lm.InitHard();
//     // }
//     if (_unit.wd_init && !initControl.wd_init) {

//     }
// }

UnitRST_t rst_units[RST_UNITS_NUM];

void init_rst_units(void) {
    memset(rst_units, 0, RST_UNITS_NUM * sizeof(UnitRST_t));
    #if (END_DEVICE_TEMPERATURE_SENSOR) || (LITE_GATEWAY)
    //SPI_UINT
    rst_units[0].name = SPI_LORA_UNIT;
    rst_units[0].init_func = unit_noarg_adapter;
    rst_units[0].init_arg = (void*)spi_lora_init;
    rst_units[0].maxErr = MAX_ERR;
    #endif
    //SPI2_UNIT
    rst_units[1].name = SPI_ACCEL_UNIT;
    rst_units[1].init_func = NULL; //TODO
    rst_units[1].maxErr = MAX_ERR;
    #if (END_DEVICE_TEMPERATURE_SENSOR) || (LITE_GATEWAY)
    //LORA_UNIT
    rst_units[2].name = LORA_UNIT;
    rst_units[2].init_func = unit_noarg_adapter;
    rst_units[2].init_arg = (void*)lora_reset;
    rst_units[2].maxErr = MAX_ERR;
    #endif
    //UART_UNIT
    rst_units[3].name = UART_UNIT;
    rst_units[3].init_func = drv_uart_init_adapter;
    static const usart_init_type uart_cfg = {
        .baudrate = USART1_BAUD_RATE,
        .data_bit = USART_DATA_8BITS,
        .stop_bit = USART_STOP_1_BIT,
        .parity = USART_PARITY_NONE,
        .mode = USART_MODE_TX_RX,
        .hardware_flow_control = USART_HARDWARE_FLOW_NONE
    };
    rst_units[3].init_arg = (void*)&uart_cfg;
    rst_units[3].maxErr = MAX_ERR;
    //MCU_UNIT
    rst_units[4].name = MCU_UNIT;
    rst_units[4].init_func = unit_noarg_adapter;
    rst_units[4].init_arg = (void*)nvic_system_reset;
    rst_units[4].maxErr = MAX_ERR;
    #if (END_DEVICE_TEMPERATURE_SENSOR) && (!BOOTLOADER)
    //I2C1_UNIT
    rst_units[5].name = I2C1_UNIT;
    rst_units[5].init_func = unit_noarg_adapter;
    rst_units[5].init_arg = (void*)nst118_hal_init;
    rst_units[5].maxErr = MAX_ERR;
    #endif
}

void add_error_to_unit_control(UNIT_NAME name) {
    for (uint8_t i = 0;i < RST_UNITS_NUM;i++) {
        if (rst_units[i].name == name) {
            rst_units[i].allErr++;
            rst_units[i].errCnt++;
            //last err time
        }
    }
}

void unit_control_wave(void) {
    for (uint8_t i = 0;i < RST_UNITS_NUM;i++) {
        if (rst_units[i].errCnt >= rst_units[i].maxErr) {
            if (rst_units[i].init_func != NULL) {
                rst_units[i].errCnt = 0;
                rst_units[i].rstCnt++;
                rst_units[i].init_func(rst_units[i].init_arg);
            }
        }
    }
}

void get_unit_rst_views(UnitRST_View_t* views) {
    for (size_t i = 0;i < RST_UNITS_NUM;++i) {
        views[i].name = rst_units[i].name;
        views[i].rstCnt = rst_units[i].rstCnt;
        views[i].errCnt = rst_units[i].errCnt;
        views[i].allErr = rst_units[i].allErr;
    }
}


