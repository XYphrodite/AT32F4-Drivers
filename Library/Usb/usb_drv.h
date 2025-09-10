#ifndef _USB_DRV_H_
#define _USB_DRV_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "at32f403a_407.h"
#include "usbd_core.h"
#include "cdc_class.h"
#include "cdc_desc.h"
#include "usbd_int.h"
#include "DelayService.h"

    void init_usb_drv(void);
    void usb_delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif