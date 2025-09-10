#include "Shared.h"

void AddNanToFloat(float *pFloat)
{
    // *pFloat = NAN;
}

void wait_and_reset(void)
{
    delay_ms(POST_COMMAND_DELAY);
    nvic_system_reset();
}

#ifdef END_DEVICE_TEMPERATURE_SENSOR
void blink_after_reset(void)
{
    if (crm_flag_get(CRM_NRST_RESET_FLAG))
    {
        crm_flag_clear(CRM_NRST_RESET_FLAG);
        pinInit(DEBUG_LED_PORT, DEBUG_LED_PIN, OUTPUT, GPIO_PULL_NONE);
        gpio_bits_set(DEBUG_LED_PORT, DEBUG_LED_PIN);
        simple_wdt_reload();
        for (size_t i = 0; i < 500; i++)
        {
            delay_ms(1);
            simple_wdt_reload();
        }
        gpio_bits_reset(DEBUG_LED_PORT, DEBUG_LED_PIN);
    }
}
#endif
