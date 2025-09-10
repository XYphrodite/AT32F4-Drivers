#ifndef _BOOTLOAD_H_
#define _BOOTLOAD_H_

// #ifdef __cplusplus
// extern "C"
// {
// #endif

#include <stdint.h>
#include <at32f403a_407.h>

#ifndef PREBOOT

#include "Macros.h"
#include "BootloadConfig.h"
#include "usb_drv.h"
#include "usb_app.h"
#include "PinDefines.h"
#include "Consts.h"
#include "gpio.h"
#include "SimpleWD.h"

#define DEFAULT_REST_BOOTLOAD_CNT (25000000U)

extern uint32_t restBootloadCnt;

void preload_task(void);
void load_app(void);
void delay_and_load_app(void);
void first_load_blink(void);
void app_wasnt_found_blink(void);

#else

#define BOOTLOADER_ADDRESS (0x08001000U)
#define MAIN_APP_ADDRESS (0x08009000U)

void load_app(uint32_t app_addr);

#endif

#endif //_BOOTLOAD_H_
