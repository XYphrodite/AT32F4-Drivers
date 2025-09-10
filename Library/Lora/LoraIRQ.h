#ifndef _LORA_IRQ_H_
#define _LORA_IRQ_H_

#ifdef LITE_GATEWAY
#define LORA_DIO1_EXINT (EXINT_LINE_10)
#define LORA_DIO1_SOURCE_PORT (GPIO_PORT_SOURCE_GPIOB)
#define LORA_DIO1_SOURCE_PIN (GPIO_PINS_SOURCE10)
#endif

#ifdef END_DEVICE_TEMPERATURE_SENSOR
#define LORA_DIO1_EXINT (EXINT_LINE_11)
#define LORA_DIO1_SOURCE_PORT (GPIO_PORT_SOURCE_GPIOB)
#define LORA_DIO1_SOURCE_PIN (GPIO_PINS_SOURCE11)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include "at32f403a_407.h"
#include <stddef.h>

    void init_dio1_irq_handler(void);
    void enable_dio1_irq(void);
    void disable_dio1_irq(void);
    void set_dio1_irq_callback(void (*cb)(void *context), void *context);
    void EXINT15_10_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif