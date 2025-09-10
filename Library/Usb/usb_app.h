/* add user code begin Header */
/**
  **************************************************************************
  * @file     usb_app.h
  * @brief    usb application config header file
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

  /* define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_APP_H
#define __USB_APP_H

#include "usb_conf.h"
#include "usbd_core.h"
#include "cdc_class.h"
#include "cdc_desc.h"
#include "usbd_int.h"
#include "DelayService.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {

#include "usbd_int.h"
#endif

  void wk_usb_app_init(void);

  extern usbd_core_type usb_core_dev;

  extern bool isUsbCon;


  void USBFS_L_CAN1_RX0_IRQHandler(void);


#ifdef __cplusplus
}
#endif

#endif
