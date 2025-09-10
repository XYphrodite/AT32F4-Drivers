/**
 **************************************************************************
 * @file     rtc.h
 * @brief    this file provides template for calendar api.
 **************************************************************************
 *                       Copyright notice & Disclaimer
 *
 * The software Board Support Package (BSP) that is made available to
 * download from Artery official website is the copyrighted work of Artery.
 * Artery authorizes customers to use, copy, and distribute the BSP
 * software and its related documentation for the purpose of design and
 * development in conjunction with Artery microcontrollers. Use of the
 * software is governed by this copyright notice and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
 * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
 * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
 * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
 * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
 *
 **************************************************************************
 */

#ifndef __RTC_H
#define __RTC_H

#include "at32f403a_407.h"
#include "BprService.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /** @addtogroup AT32F403A_F407_periph_examples
   * @{
   */

  /** @addtogroup F403A_F407_RTC_calendar
   * @{
   */

  /** @defgroup RTC_calendar_type
   * @{
   */

  typedef struct
  {
    __IO uint16_t year; /*!< specifies the years of calendar.   */
    __IO uint8_t month; /*!< specifies the months of calendar.  */
    __IO uint8_t date;  /*!< specifies the date of calendar.    */
    __IO uint8_t hour;  /*!< specifies the hours of calendar.   */
    __IO uint8_t min;   /*!< specifies the minutes of calendar. */
    __IO uint8_t sec;   /*!< specifies the second of calendar.  */
    __IO uint8_t week;  /*!< specifies the weeks of calendar.   */
  } calendar_type;

  /**
   * @}
   */

  extern calendar_type calendar;

  /** @defgroup RTC_calendar_exported_functions
   * @{
   */

  uint8_t rtc_init(calendar_type *calendar);
  uint8_t rtc_time_set(calendar_type *calendar);
  uint8_t rtc_alarm_clock_set(calendar_type *calendar);
  void rtc_time_get(void);
  uint32_t get_seccount(calendar_type *calendar);
  uint32_t rtc_time_diff(calendar_type *date1, calendar_type *date2);

#ifdef __cplusplus
}
#endif

#endif
