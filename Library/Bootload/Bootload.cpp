#include "Bootload.h"

#ifndef PREBOOT

uint32_t restBootloadCnt = DEFAULT_REST_BOOTLOAD_CNT;

void load_app(void)
{
    preload_task();

    crm_reset();

    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;

    // Disable all Interrupts
    __disable_irq();

    uint32_t appAddr = BootloadConfig::GetJumpAddress();
    volatile uint32_t const *app_vector = (volatile uint32_t const *)appAddr;

    /* switch exception handlers to the application */
    SCB->VTOR = (uint32_t)appAddr;
    __DSB();
    __set_CONTROL(0);
    __set_MSP(app_vector[0]);
    asm("bx %0" ::"r"(app_vector[1]));
}

void preload_task(void)
{
    // disconnect usb
#ifdef END_DEVICE_TEMPERATURE_SENSOR
    if (gpio_input_data_bit_read(V_BUS_2_PORT, V_BUS_2_PIN) == SET)
    {
        usbd_disconnect(&usb_core_dev);
        delay_ms(10);
    }
#endif

#ifdef LITE_GATEWAY
    state_machine.work_with_uart = false;
    save_state_machine();
#endif
    // disable interaptions
    nvic_irq_disable(TMR1_OVF_TMR10_IRQn);
    tmr_interrupt_enable(TMR1, TMR_OVF_INT, FALSE);

    nvic_irq_disable(USBFS_L_CAN1_RX0_IRQn);
}

void delay_and_load_app(void)
{
    delay_ms(POST_COMMAND_DELAY);
    load_app();
}

void app_wasnt_found_blink(void)
{
    uint32_t all_time = 8000, blink_time = 1;
    pinInit(DEBUG_LED_PORT, DEBUG_LED_PIN, OUTPUT, GPIO_PULL_NONE);
    for (size_t i = 0; i < all_time / blink_time; i++)
    {
        gpio_bits_set(DEBUG_LED_PORT, DEBUG_LED_PIN);
        delay_ms(blink_time);
        simple_wdt_reload();
        gpio_bits_reset(DEBUG_LED_PORT, DEBUG_LED_PIN);
        delay_ms(blink_time);
        simple_wdt_reload();
    }
}

void first_load_blink(void)
{
    if (!state_machine.was_blink)
    {
        pinInit(DEBUG_LED_PORT, DEBUG_LED_PIN, OUTPUT, GPIO_PULL_NONE);
        gpio_bits_set(DEBUG_LED_PORT, DEBUG_LED_PIN);
        simple_wdt_reload();
        for (size_t i = 0; i < 1500; i++)
        {
            delay_ms(1);
            simple_wdt_reload();
        }
        state_machine.was_blink = true;
        save_state_machine();
        gpio_bits_reset(DEBUG_LED_PORT, DEBUG_LED_PIN);
    }
#if END_DEVICE_TEMPERATURE_SENSOR
    else
        blink_after_reset();
#endif
}

#else

void load_app(uint32_t app_addr)
{
    crm_reset(); // Предположительно сброс тактирования

    // Остановить SysTick
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;

    // Отключить прерывания
    __disable_irq();

    // Очистить NVIC (отключить все IRQ и pending-флаги)
    for (uint8_t i = 0; i < 8; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    // Получить адрес вектора приложения
    volatile uint32_t const *app_vector = (volatile uint32_t const *)app_addr;

    // (опционально) Установить новый VTOR
    SCB->VTOR = app_addr;
    __DSB();

    // Установить указатель стека приложения
    __set_MSP(app_vector[0]);

    // Перейти в Reset_Handler приложения
    asm("bx %0" ::"r"(app_vector[1]));
}

#endif
