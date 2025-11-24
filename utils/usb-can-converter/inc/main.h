/**
  **************************************************************************
  * @file     main.h
  * @brief    main header file
  **************************************************************************
  */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f403a_407_board.h"
#include "at32f403a_407_conf.h"
#include "at32f403a_407_usb.h"
#include "usbd_core.h"
#include "cdc_class.h"
#include "cdc_desc.h"
#include "usbd_int.h"

/* Note: usb_clk48_s is defined in at32f403a_407_usb.h */

/* function declarations */
void usb_clock48m_select(usb_clk48_s clk_s);
void usb_delay_ms(uint32_t ms);
void usb_delay_us(uint32_t us);

/* external variables */
extern usbd_core_type usb_core_dev;

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

