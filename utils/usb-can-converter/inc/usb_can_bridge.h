/**
  **************************************************************************
  * @file     usb_can_bridge.h
  * @brief    USB-CAN bridge header file
  **************************************************************************
  */

#ifndef __USB_CAN_BRIDGE_H
#define __USB_CAN_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f403a_407_conf.h"

/* Statistics structure */
typedef struct
{
  uint32_t usb_rx_count;
  uint32_t usb_tx_count;
  uint32_t can_rx_count;
  uint32_t can_tx_count;
} usb_can_statistics_type;

/* Function declarations */
void usb_can_bridge_init(void);
error_status can_configuration(void);
void usb_can_bridge_process(void);
void can_rx_callback(void);
void can_error_callback(void);
void usb_can_get_statistics(usb_can_statistics_type *stats);

#ifdef __cplusplus
}
#endif

#endif /* __USB_CAN_BRIDGE_H */

