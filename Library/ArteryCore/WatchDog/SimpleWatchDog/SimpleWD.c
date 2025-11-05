#include "SimpleWD.h"

/**
 * @brief  init wdt function.
 * @param  none
 * @retval none
 */
void simple_wdt_init(void)
{
#ifdef USE_WDT
#ifndef PREBOOT
    // ErrorJournal_CheckWDTRst();
#endif
#ifdef END_DEVICE_TEMPERATURE_SENSOR
    wwdt_reset();
    wwdt_divider_set(WWDT_PCLK1_DIV_32768);
    wwdt_window_counter_set(0x7F);
    wwdt_counter_set(0x7F);

    wwdt_enable(0x7F);
#elif LITE_GATEWAY
    wdt_register_write_enable(TRUE);
    wdt_divider_set(WDT_CLK_DIV_32);
    wdt_reload_value_set(0xB00);
    wdt_enable();
    // wdt_register_write_enable(FALSE);
    // while (wdt_flag_get(WDT_DIVF_UPDATE_FLAG) == RESET || wdt_flag_get(WDT_RLDF_UPDATE_FLAG) == RESET);
    wdt_counter_reload();

#else

#endif
#endif
}

void simple_wdt_reload(void)
{
#ifdef USE_WDT
#ifdef END_DEVICE_TEMPERATURE_SENSOR
    wwdt_counter_set(0x7F);
#elif LITE_GATEWAY
    wdt_counter_reload();
#else

#endif
#endif
}
