#include "lora_spi.h"

extern LoraModule_Cfg lm_cfg;

extern LbsFlags lbs_flags;

void spi_lora_init(void)
{

    set_LoraModule_Cfg();
    // reset
    spi_i2s_reset(lm_cfg.gsc.spi);
    spi_i2s_flag_clear(lm_cfg.gsc.spi, SPI_CCERR_FLAG);
    spi_i2s_flag_clear(lm_cfg.gsc.spi, SPI_I2S_RDBF_FLAG);
    spi_i2s_flag_clear(lm_cfg.gsc.spi, I2S_TUERR_FLAG);
    spi_i2s_flag_clear(lm_cfg.gsc.spi, SPI_MMERR_FLAG);
    spi_i2s_flag_clear(lm_cfg.gsc.spi, SPI_I2S_ROERR_FLAG);
    spi_i2s_flag_clear(lm_cfg.gsc.spi, SPI_I2S_TDBE_FLAG);
    spi_i2s_flag_clear(lm_cfg.gsc.spi, I2S_ACS_FLAG);
    spi_i2s_flag_clear(lm_cfg.gsc.spi, SPI_I2S_BF_FLAG);

    // /* init dma1 channel3 */
    // // wk_dma1_channel3_init();
    // // /* config dma channel transfer parameter */
    // // /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
    // // wk_dma_channel_config(DMA1_CHANNEL3,
    // //     (uint32_t)&SPI1->dt,
    // //     (uint32_t)spiRxBuf,
    // //     DMA1_CHANNEL3_BUFFER_SIZE);
    // // dma_channel_enable(DMA1_CHANNEL3, TRUE);

    // nss initialization
    pinInit(lm_cfg.gsc.cs_type, lm_cfg.gsc.cs_pin, OUTPUT, GPIO_PULL_UP);
    gpio_bits_set(lm_cfg.gsc.cs_type, lm_cfg.gsc.cs_pin);
    // txen and rxen
    pinInit(lm_cfg.rxen_type, lm_cfg.rxen_pin, OUTPUT, GPIO_PULL_NONE);
    gpio_bits_reset(lm_cfg.rxen_type, lm_cfg.rxen_pin);
    pinInit(lm_cfg.txen_type, lm_cfg.txen_pin, OUTPUT, GPIO_PULL_NONE);
    gpio_bits_reset(lm_cfg.txen_type, lm_cfg.txen_pin);

    gpio_init_type gpio_initstructure;
    crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
    gpio_default_para_init(&gpio_initstructure);

    /* spi1 sck pin */
    gpio_initstructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_initstructure.gpio_pull = GPIO_PULL_NONE;
    gpio_initstructure.gpio_mode = GPIO_MODE_MUX;
    gpio_initstructure.gpio_pins = lm_cfg.gsc.sck_pin;
    gpio_init(lm_cfg.gsc.sck_type, &gpio_initstructure);

    /* spi1 miso pin */
    gpio_initstructure.gpio_pull = GPIO_PULL_DOWN;
    gpio_initstructure.gpio_mode = GPIO_MODE_INPUT;
    gpio_initstructure.gpio_pins = lm_cfg.gsc.miso_pin;
    gpio_init(lm_cfg.gsc.miso_type, &gpio_initstructure);

    /* spi1 mosi pin */
    gpio_initstructure.gpio_pull = GPIO_PULL_UP;
    gpio_initstructure.gpio_mode = GPIO_MODE_MUX;
    gpio_initstructure.gpio_pins = lm_cfg.gsc.mosi_pin;
    gpio_init(lm_cfg.gsc.mosi_type, &gpio_initstructure);

    spi_init_type spi_init_struct;
    crm_periph_clock_enable(LORA_SPI_CLOCK, TRUE);

    // spi_default_para_init(&spi_init_struct);
    spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
    spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
#ifdef END_DEICE_TEMPERATURE_SENSOR
    spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_2;
#endif
#ifdef LITE_GATEWAY
    spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_32;
#endif
    spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;
    spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
    spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
    spi_init_struct.clock_phase = SPI_CLOCK_PHASE_1EDGE;
    spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
    spi_init(lm_cfg.gsc.spi, &spi_init_struct);

    spi_enable(lm_cfg.gsc.spi, TRUE);
}

void lora_pins_init(void)
{

    // rxen initialization
    pinInit(lm_cfg.rxen_type, lm_cfg.rxen_pin, INPUT, GPIO_PULL_DOWN);

    // txen initialization
    pinInit(lm_cfg.txen_type, lm_cfg.txen_pin, INPUT, GPIO_PULL_DOWN);

    // rst initialization
    pinInit(lm_cfg.rst_type, lm_cfg.rst_pin, OUTPUT, GPIO_PULL_UP);
    gpio_bits_set(lm_cfg.rst_type, lm_cfg.rst_pin);

    pinInit(lm_cfg.busy_type, lm_cfg.busy_pin, INPUT, GPIO_PULL_NONE);

    gpio_init_type gpio_initstructure;

    /* dio1 pin */
    // gpio_initstructure.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
    gpio_initstructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
    gpio_initstructure.gpio_pull = GPIO_PULL_UP;
    gpio_initstructure.gpio_mode = GPIO_MODE_INPUT;
    gpio_initstructure.gpio_pins = lm_cfg.dio1_pin;
    gpio_init(lm_cfg.dio1_type, &gpio_initstructure);

    init_dio1_irq_handler();
}

void lora_reset(void)
{
    sx126x_hal_reset(&lm_cfg);
}

void set_LoraModule_Cfg(void)
{
#ifdef LITE_GATEWAY
    // auto lbs_flags = BootLoadConfig::GetLbsFlags();
    // struct
    if (lbs_flags & lbs_new_board)
    {

        lm_cfg.gsc.mosi_pin = GPIO_PINS_7;
        lm_cfg.gsc.mosi_type = GPIOA;
        lm_cfg.gsc.miso_pin = GPIO_PINS_6;
        lm_cfg.gsc.miso_type = GPIOA;
        lm_cfg.gsc.sck_pin = GPIO_PINS_5;
        lm_cfg.gsc.sck_type = GPIOA;

        lm_cfg.gsc.cs_pin = GPIO_PINS_11;
        lm_cfg.gsc.cs_type = GPIOB;
        lm_cfg.gsc.unit_name = static_cast<uint8_t>(SPI_LORA_UNIT); // LORA_UNIT_NAME;
        lm_cfg.rst_pin = GPIO_PINS_15;
        lm_cfg.rst_type = GPIOB;
        lm_cfg.busy_pin = GPIO_PINS_12;
        lm_cfg.busy_type = GPIOB;
        lm_cfg.dio1_pin = GPIO_PINS_10;
        lm_cfg.dio1_type = GPIOB;
        lm_cfg.gsc.spi = SPI1;

        lm_cfg.txen_pin = GPIO_PINS_14;
        lm_cfg.txen_type = GPIOB;
        lm_cfg.rxen_pin = GPIO_PINS_13;
        lm_cfg.txen_type = GPIOB;

        lm_cfg.pwren_pin = GPIO_PINS_8;
        lm_cfg.pwren_port = GPIOA;
    }
    else if (lbs_flags & lbs_old_board)
    {
        lm_cfg.gsc.mosi_pin = GPIO_PINS_7;
        lm_cfg.gsc.mosi_type = GPIOA;
        lm_cfg.gsc.miso_pin = GPIO_PINS_6;
        lm_cfg.gsc.miso_type = GPIOA;
        lm_cfg.gsc.sck_pin = GPIO_PINS_5;
        lm_cfg.gsc.sck_type = GPIOA;

        lm_cfg.gsc.cs_pin = GPIO_PINS_0;
        lm_cfg.gsc.cs_type = GPIOB;
        lm_cfg.gsc.unit_name = static_cast<uint8_t>(SPI_LORA_UNIT); // LORA_UNIT_NAME;
        lm_cfg.rst_pin = GPIO_PINS_2;
        lm_cfg.rst_type = GPIOB;
        lm_cfg.busy_pin = GPIO_PINS_10;
        lm_cfg.busy_type = GPIOB;
        lm_cfg.dio1_pin = GPIO_PINS_12;
        lm_cfg.dio1_type = GPIOB;
        lm_cfg.gsc.spi = SPI1;

        lm_cfg.txen_pin = GPIO_PINS_11;
        lm_cfg.txen_type = GPIOB;
        lm_cfg.rxen_pin = GPIO_PINS_1;
        lm_cfg.txen_type = GPIOB;

        lm_cfg.pwren_pin = GPIO_PINS_13;
        lm_cfg.pwren_port = GPIOB;
    }
    else if (lbs_flags & lbs_unknown_board)
    {
        mcu_safe_reset();
    }

#elif END_DEVICE_TEMPERATURE_SENSOR

    lm_cfg.gsc.mosi_pin = LORA_MOSI_PIN;
    lm_cfg.gsc.mosi_type = LORA_MOSI_PORT;
    lm_cfg.gsc.miso_pin = LORA_MISO_PIN;
    lm_cfg.gsc.miso_type = LORA_MISO_PORT;
    lm_cfg.gsc.sck_pin = LORA_SCK_PIN;
    lm_cfg.gsc.sck_type = LORA_SCK_PORT;
    lm_cfg.gsc.cs_pin = LORA_NSS_PIN;
    lm_cfg.gsc.cs_type = LORA_NSS_PORT;
    lm_cfg.gsc.unit_name = static_cast<uint8_t>(SPI_LORA_UNIT); // LORA_UNIT_NAME;
    lm_cfg.rst_pin = LORA_RST_PIN;
    lm_cfg.rst_type = LORA_RST_PORT;
    lm_cfg.busy_pin = LORA_BUSY_PIN;
    lm_cfg.busy_type = LORA_BUSY_PORT;
    lm_cfg.dio1_pin = LORA_DIO1_PIN;
    lm_cfg.dio1_type = LORA_DIO1_PORT;
    lm_cfg.gsc.spi = LORA_SPI;

    lm_cfg.txen_pin = LORA_TXEN_PIN;
    lm_cfg.txen_type = LORA_TXEN_PORT;
    lm_cfg.rxen_pin = LORA_RXEN_PIN;
    lm_cfg.txen_type = LORA_RXEN_PORT;
#endif
}
