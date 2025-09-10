#ifndef _USB_PROCESS_H_
#define _USB_PROCESS_H_

// #ifdef __cplusplus
// extern "C"
// {
// #endif

#include "at32f403a_407.h"
#include "Command.h"
#include "CommandHandler.h"
#if (LITE_GATEWAY && BOOTLOADER) || BOOTLOADER || END_DEVICE_TEMPERATURE_SENSOR
#include "SimpleWD.h"
#endif
#if (LITE_GATEWAY || !BOOTLOADER)
// #include "wdt.h"
#endif
#include "PacketWrapper.h"
#include "usb_drv.h"
#include "usb_app.h"
#include "UpTime.h"
#include "PinDefines.h"
#include "gpio.h"
#include "usbService.h"

void usb_process(void);

extern uint32_t usb_limit_s;
extern UsbService usbService;

// #ifdef __cplusplus
// }
// #endif

#endif