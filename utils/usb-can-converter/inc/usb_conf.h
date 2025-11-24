/**
  **************************************************************************
  * @file     usb_conf.h
  * @brief    usb config header file
  **************************************************************************
  */

#ifndef __USB_CONF_H
#define __USB_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f403a_407_usb.h"
#include "at32f403a_407_board.h"
#include "stdio.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */

/** @addtogroup USB_vcp_class
  * @{
  */

/** @defgroup USB_vcp_class_definition
  * @{
  */

/**
  * define usb device mode (USE_OTG_DEVICE_MODE or USE_OTG_HOST_MODE)
  */
#define USE_OTG_DEVICE_MODE

/**
  * usb device mode config: dma, sof output, vbus ignore
  */
#define USB_EPT_AUTO_MALLOC
// #define USB_LOW_POWER_WAKUP
// #define USB_SOF_OUTPUT_ENABLE
// #define USB_VBUS_IGNORE

/**
  * in the usb ip, the max endpoint number is 7 (include ept 0)
  * the max in/out endpoint number is 7 (exclude ept 0)
  * define the endpoint number used by the cdc class
  */
#define USB_EPT0_MAXPACKET_SIZE         0x40
#define USBD_CDC_MAXPACKET_SIZE         0x40

#define USBD_CDC_BULK_IN_EPT            0x81
#define USBD_CDC_BULK_OUT_EPT           0x02
#define USBD_CDC_INT_EPT                0x83

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USB_CONF_H */

