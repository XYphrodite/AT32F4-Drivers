#include "rtc_and_pwr.h"

#ifdef ADD_DEEPSLEEP_MODE

__IO uint32_t index = 0;
__IO uint32_t systick_index = 0;
#endif

/**
  * @brief  rtc configuration.
  * @param  none
  * @retval none
  */
void rtc_config(void) {
#ifdef ADD_DEEPSLEEP_MODE
  exint_init_type exint_init_struct;

  /* config the exint line of the rtc alarm */
  exint_init_struct.line_select = EXINT_LINE_17;
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);
#endif
  // #ifdef ADD_STANDBY_MODE
  /* enable the battery-powered domain write operations */
  pwc_battery_powered_domain_access(TRUE);

  /* enable the lick */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_LICK, TRUE);

  /* wait lick is ready */
  while (crm_flag_get(CRM_LICK_STABLE_FLAG) == RESET);

  /* select the rtc clock source */
  crm_rtc_clock_select(CRM_RTC_CLOCK_LICK);

  /* enable rtc clock */
  crm_rtc_clock_enable(TRUE);

  /* wait for rtc registers update */
  rtc_wait_update_finish();

  /* set rtc divider: set rtc period to 1sec */
  rtc_divider_set(32767);

  /* wait for the register write to complete */
  rtc_wait_config_finish();

#ifdef ADD_DEEPSLEEP_MODE
  /* enable alarm interrupt */
  rtc_interrupt_enable(RTC_TA_INT, TRUE);

  /* wait for the register write to complete */
  rtc_wait_config_finish();

  /* configure and enable rtc alarm interrupt */
  nvic_irq_enable(RTCAlarm_IRQn, 0, 0);
#endif
}

/**
  * @brief  rtc alarm set.
  * @param  none
  * @retval none
  */
void rtc_alarm_config(uint32_t alarm_time) {
  //comment two line under to decrease time
  /* clear second flag */
  // rtc_flag_clear(RTC_TS_FLAG);

  // /* wait for the second flag to be set */
  // while (rtc_flag_get(RTC_TS_FLAG) == RESET);

  /* config the wakeup time */
  rtc_alarm_set(rtc_counter_get() + alarm_time);

  /* wait for the register write to complete */
  rtc_wait_config_finish();
#ifdef ADD_DEEPSLEEP_MODE

  /* save systick register configuration */
  systick_index = SysTick->CTRL;
  systick_index &= ~((uint32_t)0xFFFFFFFE);

  /* disable systick */
  SysTick->CTRL &= (uint32_t)0xFFFFFFFE;

  /* congfig the voltage regulator mode */
  pwc_voltage_regulate_set(PWC_REGULATOR_LOW_POWER);
#endif
}

void rtc_init(void) {
  /* enable pwc and bpr clock */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_BPR_PERIPH_CLOCK, TRUE);
#ifdef ADD_STANDBY_MODE
  if (pwc_flag_get(PWC_STANDBY_FLAG) != RESET) {
    /* wakeup from standby */
    pwc_flag_clear(PWC_STANDBY_FLAG);
  }

  if (pwc_flag_get(PWC_WAKEUP_FLAG) != RESET) {
    /* wakeup event occurs */
    pwc_flag_clear(PWC_WAKEUP_FLAG);
  }
#endif
}

#ifdef ADD_DEEPSLEEP_MODE

void go_deepsleep(uint8_t s) {
  rtc_alarm_config(s);
  // pwc_voltage_regulate_set()
  /* enter deep sleep mode */
  pwc_deep_sleep_mode_enter(PWC_DEEP_SLEEP_ENTER_WFI);

  /* restore systick register configuration */
  SysTick->CTRL |= systick_index;
  /* wait clock stable */
  TimerService::DelayUS(120);

  /* wake up from deep sleep mode, congfig the system clock */
  system_clock_recover();
}

/**
  * @brief  systemclock recover.
  * @param  none
  * @retval none
  */
void system_clock_recover(void) {
  /* enable external high-speed crystal oscillator - hext */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);

  /* wait till hext is ready */
  while (crm_hext_stable_wait() == ERROR);

  /* enable pll */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

  /* wait till pll is ready */
  while (crm_flag_get(CRM_PLL_STABLE_FLAG) == RESET);

  /* enable auto step mode */
  crm_auto_step_mode_enable(TRUE);

  /* select pll as system clock source */
  crm_sysclk_switch(CRM_SCLK_PLL);

  /* wait till pll is used as system clock source */
  while (crm_sysclk_switch_status_get() != CRM_SCLK_PLL);
}

#endif

#ifdef ADD_STANDBY_MODE

void go_standby(uint32_t rtc_step) {
  /* set the wakeup */
  rtc_alarm_config(rtc_step);
  /* enter standby mode */
  pwc_standby_mode_enter();
}

#endif


void RTCAlarm_IRQHandler(void) {
  if (rtc_interrupt_flag_get(RTC_TA_FLAG) != RESET) {
    /* clear exint line flag */
    exint_flag_clear(EXINT_LINE_17);

    /* wait for the register write to complete */
    rtc_wait_config_finish();

    /* clear alarm flag */
    rtc_flag_clear(RTC_TA_FLAG);

    /* wait for the register write to complete */
    rtc_wait_config_finish();
  }
}
