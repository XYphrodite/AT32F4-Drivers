#include "llcc68_hal.h"
#include "LoraModule.h"
#include "unitControl.h"

extern LoraModule_Cfg lm_cfg;

/**
 * Radio data transfer - write
 *
 * @remark Shall be implemented by the user
 *
 * @param [in] context          Radio implementation parameters
 * @param [in] command          Pointer to the buffer to be transmitted
 * @param [in] command_length   Buffer size to be transmitted
 * @param [in] data             Pointer to the buffer to be transmitted
 * @param [in] data_length      Buffer size to be transmitted
 *
 * @returns Operation status
 */
llcc68_hal_status_t llcc68_hal_write(const void *context, const uint8_t *command, const uint16_t command_length,
                                     const uint8_t *data, const uint16_t data_length)
{
    Lora_SPI *lora_spi = Lora_SPI::GetInstance();
    LoraModule_Cfg *cfg = &lm_cfg;

    if (gpio_input_data_bit_read(cfg->busy_type, cfg->busy_pin) == SET)
    {
        sx126x_hal_wakeup(context);
    }
    lora_spi->set_cs(RESET); // Pull NSS low to start communication
    for (uint16_t i = 0; i < command_length; i++)
    {
        if (lora_spi->hal_write(&command[i]) == ERROR)
            return LLCC68_HAL_STATUS_ERROR;
    }
    for (uint16_t i = 0; i < data_length; i++)
    {
        if (lora_spi->hal_write(&data[i]) == ERROR)
            return LLCC68_HAL_STATUS_ERROR;
    }
    lora_spi->waitUntilLastByteSend();
    lora_spi->set_cs(SET); // Pull NSS high to end communication

    uint32_t count = 0;
    while (gpio_input_data_bit_read(cfg->busy_type, cfg->busy_pin) == SET && (count++ < HAL_SPI_MAX_DELAY * 100))
        ;

    return LLCC68_HAL_STATUS_OK;
}

/**
 * Radio data transfer - read
 *
 * @remark Shall be implemented by the user
 *
 * @param [in] context          Radio implementation parameters
 * @param [in] command          Pointer to the buffer to be transmitted
 * @param [in] command_length   Buffer size to be transmitted
 * @param [in] data             Pointer to the buffer to be received
 * @param [in] data_length      Buffer size to be received
 *
 * @returns Operation status
 */
llcc68_hal_status_t llcc68_hal_read(const void *context, const uint8_t *command, const uint16_t command_length,
                                    uint8_t *data, const uint16_t data_length)
{

    Lora_SPI *lora_spi = Lora_SPI::GetInstance();
    LoraModule_Cfg *cfg = &lm_cfg;

    uint32_t count = 0;
    if (gpio_input_data_bit_read(cfg->busy_type, cfg->busy_pin) == SET)
    {
        sx126x_hal_wakeup(context);
    }

    lora_spi->set_cs(RESET); // Pull NSS low to start communication

    for (uint16_t i = 0; i < command_length; i++)
    {
        if (lora_spi->hal_write(&command[i]) == ERROR)
            return LLCC68_HAL_STATUS_ERROR;
    }
    count = 0;
    for (uint16_t i = 0; i < data_length; i++)
    {
        lora_spi->hal_read(&data[i]);
    }
    lora_spi->waitUntilLastByteSend();
    lora_spi->set_cs(SET); // Pull NSS high to end communication

    while (gpio_input_data_bit_read(cfg->busy_type, cfg->busy_pin) == SET && (count++ < HAL_SPI_MAX_DELAY * 100))
        ;

    return LLCC68_HAL_STATUS_OK;
}

/**
 * Reset the radio
 *
 * @remark Shall be implemented by the user
 *
 * @param [in] context Radio implementation parameters
 *
 * @returns Operation status
 */
llcc68_hal_status_t llcc68_hal_reset(const void *context)
{
    disable_dio1_irq();
    LoraModule_Cfg *cfg = &lm_cfg;
    gpio_bits_reset(cfg->rst_type, cfg->rst_pin);
    delay_ms(100);
    gpio_bits_set(cfg->rst_type, cfg->rst_pin);
    uint32_t count = 0;
    while (gpio_input_data_bit_read(cfg->busy_type, cfg->busy_pin) == SET && (count++ < HAL_SPI_MAX_DELAY * 1000))
        ;
    return LLCC68_HAL_STATUS_OK;
}

/**
 * Wake the radio up.
 *
 * @remark Shall be implemented by the user
 *
 * @param [in] context Radio implementation parameters
 *
 * @returns Operation status
 */
llcc68_hal_status_t llcc68_hal_wakeup(const void *context)
{
    Lora_SPI *lora_spi = Lora_SPI::GetInstance();
    // LoraModule_Cfg* cfg = &lm_cfg;
    lora_spi->set_cs(RESET);
    delay_us(150);
    lora_spi->set_cs(SET);
    delay_us(400);
    return LLCC68_HAL_STATUS_OK;
}

/* --- EOF ------------------------------------------------------------------ */
