#include "general_spi.h"

GENERAL_SPI::GENERAL_SPI(/* args */) {
}

GENERAL_SPI::GENERAL_SPI(general_spi_cfg_t* gsc) : gsc(gsc) {}

GENERAL_SPI::~GENERAL_SPI() {
}

void GENERAL_SPI::set_cs(flag_status new_status) {
    if (new_status == SET) {
        gpio_bits_set(gsc->cs_pin.port, static_cast<uint32_t>(gsc->cs_pin.pin));
    }
    else {
        gpio_bits_reset(gsc->cs_pin.port, static_cast<uint32_t>(gsc->cs_pin.pin));
    }
}

error_status GENERAL_SPI::hal_write(const uint8_t* buff) {
    uint32_t count = 0;
    while ((RESET == spi_i2s_flag_get(gsc->spi, SPI_I2S_TDBE_FLAG)) && (count++ < HAL_SPI_MAX_DELAY));
    if (count >= HAL_SPI_MAX_DELAY) {
        add_error_to_unit_control((UNIT_NAME)gsc->unit_name);
        set_cs(SET);
        return ERROR;
    }
    else {
        spi_i2s_data_transmit(gsc->spi, *buff);
    }
    count = 0;
    while ((count++ < HAL_SPI_MAX_DELAY) && spi_i2s_flag_get(gsc->spi, SPI_I2S_RDBF_FLAG) == RESET);
    count = 0;
    spi_i2s_flag_clear(gsc->spi, SPI_I2S_RDBF_FLAG);
    return SUCCESS;
}

error_status GENERAL_SPI::hal_read(uint8_t* buff) {
    uint32_t count = 0;
    while ((RESET == spi_i2s_flag_get(gsc->spi, SPI_I2S_TDBE_FLAG)) && (count++ < HAL_SPI_MAX_DELAY));
    if (count >= HAL_SPI_MAX_DELAY) {
        add_error_to_unit_control((UNIT_NAME)gsc->unit_name);
        set_cs(SET);
        return ERROR;
    }
    else {
        spi_i2s_data_transmit(gsc->spi, 0x00);
    }
    count = 0;
    spi_i2s_flag_clear(gsc->spi, SPI_I2S_RDBF_FLAG);
    while ((count++ < HAL_SPI_MAX_DELAY) && spi_i2s_flag_get(gsc->spi, SPI_I2S_RDBF_FLAG) == RESET);
    if (count >= HAL_SPI_MAX_DELAY) {
        add_error_to_unit_control((UNIT_NAME)gsc->unit_name);
        set_cs(SET);
        return ERROR;
    }
    else {
        *buff = spi_i2s_data_receive(gsc->spi);
    }
    return SUCCESS;
}

error_status GENERAL_SPI::waitUntilLastByteSend(void) {
    uint32_t count = 0;
    while ((RESET == spi_i2s_flag_get(gsc->spi, SPI_I2S_TDBE_FLAG)) && (count++ < HAL_SPI_MAX_DELAY)) {
        add_error_to_unit_control((UNIT_NAME)gsc->unit_name);
        set_cs(SET);
        return ERROR;
    }
    return SUCCESS;
}