#ifndef _LORA_SPI_H_
#define _LORA_SPI_H_

#include "at32f403a_407.h"
#include "gpio.h"
#include "LoraIRQ.h"
#include "LoraStructs.h"

#ifdef USE_LLCC68
#include "llcc68_hal.h"
#else
#include "sx126x_hal.h"
#endif

#include "unitControl.h"
#include "Enums.h"
#include "mcu_hal.h"

#ifdef LITE_GATEWAY

#define LORA_SPI_CLOCK CRM_SPI1_PERIPH_CLOCK

#else

#define LORA_SPI SPI2
#define LORA_SPI_CLOCK CRM_SPI2_PERIPH_CLOCK

#define LORA_MOSI_PORT GPIOB
#define LORA_MISO_PORT GPIOB
#define LORA_NSS_PORT GPIOB
#define LORA_SCK_PORT GPIOB
#define LORA_BUSY_PORT GPIOB
#define LORA_DIO1_PORT GPIOB
#define LORA_RST_PORT GPIOB

#define LORA_MOSI_PIN GPIO_PINS_15
#define LORA_MISO_PIN GPIO_PINS_14
#define LORA_NSS_PIN GPIO_PINS_12
#define LORA_SCK_PIN GPIO_PINS_13
#define LORA_BUSY_PIN GPIO_PINS_10
#define LORA_DIO1_PIN GPIO_PINS_11
#define LORA_RST_PIN GPIO_PINS_3

#define LORA_RXEN_PORT GPIOB
#define LORA_TXEN_PORT GPIOB

#define LORA_RXEN_PIN GPIO_PINS_9
#define LORA_TXEN_PIN GPIO_PINS_8

#endif

void spi_lora_init(void);
// void spi_lora_init(LbsFlags lbs_flags);
void lora_pins_init(void);
void lora_reset(void);
void set_LoraModule_Cfg(void);

#endif
