#ifndef _I2C_HAL_H_
#define _I2C_HAL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <at32f403a_407.h>
#include "gpio.h"
#include "i2c.h"
#include <string.h>
#include "DelayService.h"

#define NST118_HAL_TIMEOUT (10000U)
#define NST118_I2C_BAUDRATE (10000U)

    typedef struct i2c_context
    {
        i2c_type *i2c;

        gpio_type *scl_port;
        uint32_t scl_pin;
        gpio_type *sda_port;
        uint32_t sda_pin;

        uint8_t slaveAddr;
    } i2c_context_t;

    void i2c_ClearADDRFlag(void *context);
    void i2c1_hal_init(void);
    void i2c_hal_pullUp_init(void);
    void i2c_hal_pullUp_set(bool new_state);
    bool i2c_hal_read(void *context, uint8_t regAddr, uint8_t *buf, uint32_t length);
    bool i2c_hal_write(void *context, uint8_t regAddr, uint8_t *buf, uint32_t length);

    void I2C1_ERR_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif