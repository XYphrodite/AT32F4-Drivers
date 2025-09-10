#include "general_i2c.h"

general_i2c_cfg_t GENERAL_I2C::gi2cc;

GENERAL_I2C::~GENERAL_I2C(){

}

void GENERAL_I2C::SetI2C(general_i2c_cfg_t _gi2cc)
{
    gi2cc = _gi2cc;
}

general_i2c_cfg_t GENERAL_I2C::GetI2C1(void)
{
    general_i2c_cfg_t cfg;
    cfg.i2c = I2C1;
    cfg.sda_port = GPIOB;
    cfg.sda_pin = GPIO_PINS_7;
    cfg.scl_port = GPIOB;
    cfg.scl_pin = GPIO_PINS_6;
    cfg.i2c_clock = CRM_I2C1_PERIPH_CLOCK;
    cfg.port_clock = CRM_GPIOB_PERIPH_CLOCK;
    cfg.baudrate = 10000U;
    return cfg;
}

void GENERAL_I2C::InitI2C(void)
{
    /* reset i2c peripheral */
    i2c_reset(GENERAL_I2C::gi2cc.i2c);
    DisableI2C();

    crm_periph_clock_enable(GENERAL_I2C::gi2cc.i2c_clock, TRUE);
    crm_periph_clock_enable(GENERAL_I2C::gi2cc.port_clock, TRUE);

    gpio_init_type gpio_init_struct;

    gpio_default_para_init(&gpio_init_struct);

    /* configure the I2C_PUP pin */
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_pins = GPIO_PINS_4;
    gpio_init(GPIOB, &gpio_init_struct);
    // //gpio_bits_set(GPIOB, GPIO_PINS_4);

    /* configure the ALERT pin */
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_pins = GPIO_PINS_5;
    gpio_init(GPIOB, &gpio_init_struct);
    gpio_bits_set(GPIOB, GPIO_PINS_5);

    /* configure the SCL pin */
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_pins = GENERAL_I2C::gi2cc.scl_pin;
    gpio_init(GENERAL_I2C::gi2cc.scl_port, &gpio_init_struct);

    /* configure the SDA pin */
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_pins = GENERAL_I2C::gi2cc.sda_pin;
    gpio_init(GENERAL_I2C::gi2cc.sda_port, &gpio_init_struct);
    /* i2c peripheral initialization */
    i2c_init(GENERAL_I2C::gi2cc.i2c, I2C_FSMODE_DUTY_2_1, GENERAL_I2C::gi2cc.baudrate);

    i2c_own_address1_set(GENERAL_I2C::gi2cc.i2c, I2C_ADDRESS_MODE_7BIT, 0);
    i2c_ack_enable(GENERAL_I2C::gi2cc.i2c, TRUE);
    i2c_master_receive_ack_set(GENERAL_I2C::gi2cc.i2c, I2C_MASTER_ACK_CURRENT);
    i2c_clock_stretch_enable(GENERAL_I2C::gi2cc.i2c, TRUE);
    // i2c_general_call_enable(I2C1, FALSE);
    /* i2c peripheral enable */
    i2c_interrupt_enable(GENERAL_I2C::gi2cc.i2c, I2C_ERR_INT, TRUE);
}

void GENERAL_I2C::DisableI2C(void)
{
    i2c_enable(GENERAL_I2C::gi2cc.i2c, FALSE);
}

void GENERAL_I2C::EnableI2C(void)
{
    i2c_enable(GENERAL_I2C::gi2cc.i2c, TRUE);
}

general_i2c_cfg_t GENERAL_I2C::GetI2C(void) { return GENERAL_I2C::gi2cc; }
