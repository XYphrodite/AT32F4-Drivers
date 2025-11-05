#include "i2c_hal.h"

void i2c1_hal_init(void)
{
    /* reset i2c peripheral */
    i2c_reset(I2C1);
    i2c_enable(I2C1, FALSE);

    crm_periph_clock_enable(CRM_I2C1_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

    gpio_init_type gpio_init_struct;

    /* configure the SCL pin */
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_pins = GPIO_PINS_6;
    gpio_init(GPIOB, &gpio_init_struct);

    /* configure the SDA pin */
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_pins = GPIO_PINS_7;
    gpio_init(GPIOB, &gpio_init_struct);
    /* i2c peripheral initialization */
    i2c_init(I2C1, I2C_FSMODE_DUTY_2_1, 100000);

    i2c_own_address1_set(I2C1, I2C_ADDRESS_MODE_7BIT, 0);
    i2c_ack_enable(I2C1, TRUE);
    i2c_master_receive_ack_set(I2C1, I2C_MASTER_ACK_CURRENT);
    i2c_clock_stretch_enable(I2C1, TRUE);
    // i2c_general_call_enable(I2C1, FALSE);
    /* i2c peripheral enable */
    i2c_interrupt_enable(I2C1, I2C_ERR_INT, TRUE);

    i2c_enable(I2C1, TRUE);
}

void i2c_hal_pullUp_init(void)
{
    pinInit(GPIOB, GPIO_PINS_4, OUTPUT, GPIO_PULL_UP);
}

void i2c_hal_pullUp_set(bool new_state)
{
    if (new_state)
        gpio_bits_set(GPIOB, GPIO_PINS_4);
    else
        gpio_bits_reset(GPIOB, GPIO_PINS_4);
}

void i2c_ClearADDRFlag(void *context)
{
    __attribute__((unused)) __IO uint32_t tmpreg;
    i2c_context_t *ctx = reinterpret_cast<i2c_context_t *>(context);

    tmpreg = I2C1->sts1;

    tmpreg = I2C1->sts2;
}

bool i2c_hal_read(void *context, uint8_t regAddr, uint8_t *buf, uint32_t length)
{
    i2c_context_t *ctx = reinterpret_cast<i2c_context_t *>(context);
    uint32_t timeout = NST118_HAL_TIMEOUT;
    uint8_t recBytes[length];

    /* enable ack */
    i2c_ack_enable(I2C1, TRUE);

    /* generate start condtion */
    i2c_start_generate(I2C1);

    timeout = NST118_HAL_TIMEOUT;
    while (!i2c_flag_get(I2C1, I2C_STARTF_FLAG) && (timeout > 0))
    {
        timeout--;
    };
    if (!timeout)
    {
        return false;
    }

    timeout = NST118_HAL_TIMEOUT;
    while (I2C1->ctrl1 & I2C_CTRL1_STARTGEN && (timeout > 0))
    {
        timeout--;
    };
    if (!timeout)
    {
        return false;
    }

    i2c_7bit_address_send(I2C1, (ctx->slaveAddr << 1), I2C_DIRECTION_TRANSMIT);
    timeout = NST118_HAL_TIMEOUT;
    while (!i2c_flag_get(I2C1, I2C_ADDR7F_FLAG) && (timeout > 0))
    {
        timeout--;
    };
    if (!timeout)
    {
        return false;
    }
    i2c_ClearADDRFlag(context);

    i2c_data_send(I2C1, regAddr);
    timeout = NST118_HAL_TIMEOUT;
    while (!i2c_flag_get(I2C1, I2C_TDC_FLAG) && (timeout > 0))
    {
        timeout--;
    };
    if (!timeout)
    {
        return false;
    }

    i2c_flag_clear(I2C1, I2C_TDC_FLAG);

    i2c_start_generate(I2C1);
    timeout = NST118_HAL_TIMEOUT;
    while (!i2c_flag_get(I2C1, I2C_STARTF_FLAG) && (timeout > 0))
    {
        timeout--;
    };
    if (!timeout)
    {
        return false;
    }

    i2c_7bit_address_send(I2C1, (ctx->slaveAddr << 1), I2C_DIRECTION_RECEIVE);
    timeout = NST118_HAL_TIMEOUT;
    while (!i2c_flag_get(I2C1, I2C_ADDR7F_FLAG) && (timeout > 0))
    {
        timeout--;
    };
    if (!timeout)
    {
        return false;
    }

    i2c_ClearADDRFlag(I2C1);

    for (uint8_t i = 0; i <= length; ++i)
    {
        if (i == length)
        {
            i2c_ack_enable(I2C1, FALSE);
            i2c_stop_generate(I2C1);
        }
        timeout = NST118_HAL_TIMEOUT;
        while (!i2c_flag_get(I2C1, I2C_RDBF_FLAG) && (timeout > 0))
        {
            timeout--;
        };
        if (!timeout)
        {
            return false;
        }
        if (i == length)
            break;

        recBytes[i] = i2c_data_receive(I2C1);
    }
    memcpy(buf, recBytes, length);
    timeout = NST118_HAL_TIMEOUT;
    while ((I2C1->ctrl1 & I2C_CTRL1_STOPGEN) && (timeout > 0))
    {
        timeout--;
    };
    if (!timeout)
    {
        return false;
    }
    return true;
}

bool i2c_hal_write(void *context, uint8_t regAddr, uint8_t *buf, uint32_t length)
{
    i2c_context_t *ctx = reinterpret_cast<i2c_context_t *>(context);

    uint8_t *dat = (uint8_t *)buf;
    uint32_t timeout = NST118_HAL_TIMEOUT;

    i2c_start_generate(I2C1);
    timeout = NST118_HAL_TIMEOUT;
    while (!i2c_flag_get(I2C1, I2C_STARTF_FLAG) && (timeout > 0))
    {
        timeout--;
    };
    if (!timeout)
    {
        // gpio_bits_reset(GPIOB, GPIO_PINS_4);
        return false;
    }

    i2c_7bit_address_send(I2C1, (ctx->slaveAddr << 1), I2C_DIRECTION_TRANSMIT);
    timeout = NST118_HAL_TIMEOUT;
    while (!i2c_flag_get(I2C1, I2C_ADDR7F_FLAG) && (timeout > 0))
    {
        timeout--;
    };
    if (!timeout)
    {
        // gpio_bits_reset(GPIOB, GPIO_PINS_4);
        return false;
    }
    i2c_ClearADDRFlag(I2C1);

    i2c_data_send(I2C1, regAddr);
    timeout = NST118_HAL_TIMEOUT;
    while (!i2c_flag_get(I2C1, I2C_TDC_FLAG) && (timeout > 0))
    {
        timeout--;
    };
    if (!timeout)
    {
        // gpio_bits_reset(GPIOB, GPIO_PINS_4);
        return false;
    }
    i2c_flag_clear(I2C1, I2C_TDC_FLAG);

    while (length--)
    {
        i2c_data_send(I2C1, *dat++);
        timeout = NST118_HAL_TIMEOUT;
        while (!i2c_flag_get(I2C1, I2C_TDC_FLAG) && (timeout > 0))
        {
            timeout--;
        };
        if (!timeout)
        {
            // gpio_bits_reset(GPIOB, GPIO_PINS_4);
            return false;
        }
        i2c_flag_clear(I2C1, I2C_TDC_FLAG);
    }

    i2c_stop_generate(I2C1);

    timeout = NST118_HAL_TIMEOUT;
    while ((I2C1->ctrl1 & I2C_CTRL1_STOPGEN) && (timeout > 0))
    {
        timeout--;
    };
    if (!timeout)
    {
        // gpio_bits_reset(GPIOB, GPIO_PINS_4);
        return false;
    }
    // gpio_bits_reset(GPIOB, GPIO_PINS_4);
    return true;
}

/**
 * @brief  this function handles I2C1 Error handler.
 * @param  none
 * @retval none
 */
void I2C1_ERR_IRQHandler(void)
{
    i2c_enable(I2C1, FALSE);
    i2c_enable(I2C1, TRUE);
}
