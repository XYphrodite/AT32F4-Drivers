#ifndef _RTC_AND_PWC_H_
#define _RTC_AND_PWC_H_

#include "at32f403a_407.h"

void rtc_alarm_config(uint32_t alarm_time);
void rtc_sleep_init(void);
void hal_mcu_set_sleep_for_ms(uint32_t ms);
extern "C"  void RTCAlarm_IRQHandler(void);
void rtc_config(void);
void rtc_init(void);
void go_deepsleep(uint8_t s);
void go_standby(uint32_t rtc_step);
void system_clock_recover(void);

#endif