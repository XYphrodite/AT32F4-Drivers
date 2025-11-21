#include "wdt.h"
#include <cstring>

WatchDogSubject_t wds;

/**
 * @brief  init wdt function.
 * @param  none
 * @retval none
 */
void wdt_init(void)
{
//   ErrorJournal_CheckWDTRst();

  wdt_register_write_enable(TRUE);
  wdt_divider_set(WDT_CLK_DIV_256);
  wdt_reload_value_set(0xB00);
  wdt_enable();
  // wdt_register_write_enable(FALSE);
  // while (wdt_flag_get(WDT_DIVF_UPDATE_FLAG) == RESET || wdt_flag_get(WDT_RLDF_UPDATE_FLAG) == RESET);
  wdt_counter_reload();
}

void wdt_try_reload(void)
{
  bool is_healthy =
      // (bool)wds.unit_control == true
      // && (bool)wds.gateway == true
      (bool)wds.gateway == true && (bool)wds.blinky == true && (bool)wds.uart == true && (bool)wds.usb == true;
  if (is_healthy == true)
  {
    wdt_counter_reload();
    memset(&wds, 0, sizeof(WatchDogSubject_t));
    // HAL_TRACE_INFO("Reset WDT.\n");
  }
}

void wdt_notify_to_subject(WatchDogSubject_t s)
{
  uint8_t temp = (*(uint8_t *)&wds) | *((uint8_t *)&s);
  wds = *(WatchDogSubject_t *)&temp;
}
