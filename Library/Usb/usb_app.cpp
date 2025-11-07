/* add user code begin Header */
/**
 **************************************************************************
 * @file     usbd_app.c
 * @brief    usb device app
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
/* add user code end Header */

#include "usb_conf.h"
#include "usbd_int.h"
#include "cdc_class.h"
#include "cdc_desc.h"
#include "usb_app.h"

usbd_core_type usb_core_dev;

/**
 * @brief  usb application initialization
 * @param  none
 * @retval none
 */
void wk_usb_app_init(void)
{
  /*fs1 device cdc*/
  usbd_core_init(&usb_core_dev, USB, &cdc_class_handler, &cdc_desc_handler, 0);

  /* enable usb pull-up */
  usbd_connect(&usb_core_dev);
}

/**
 * @brief  usb delay function
 * @param  ms: delay number of milliseconds.
 * @retval none
 */
void usb_delay_ms(uint32_t ms)
{
  delay_ms(ms);
}

extern "C"
{
#include "usbd_int.h"
  void USBFS_L_CAN1_RX0_IRQHandler(void)
  {
    usbd_irq_handler(&usb_core_dev);
  }
}
