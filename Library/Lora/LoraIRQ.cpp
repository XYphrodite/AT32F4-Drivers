#include "LoraIRQ.h"

void (*dio1_irq_callback)(void* context);\
void* dio1_context;

void init_dio1_irq_handler(void) {
    exint_init_type exint_init_struct;

    /* configure the EXINT11 */
    gpio_exint_line_config(LORA_DIO1_SOURCE_PORT, LORA_DIO1_SOURCE_PIN);

    exint_default_para_init(&exint_init_struct);
    exint_init_struct.line_enable = TRUE;
    exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
    exint_init_struct.line_select = LORA_DIO1_EXINT;
    exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
    exint_init(&exint_init_struct);

    /**
     * Users need to configure EXINT11 interrupt functions according to the actual application.
     * 1. Call the below function to enable the corresponding EXINT11 interrupt.
     *     --exint_interrupt_enable(LORA_DIO1_EXINT, TRUE);
     * 2. Add the user's interrupt handler code into the below function in the at32f403a_407_int.c file.
     *     --void EXINT15_10_IRQHandler(void)
     */

    nvic_irq_enable(EXINT15_10_IRQn, 4, 0);
}

void enable_dio1_irq(void) {
    exint_interrupt_enable(LORA_DIO1_EXINT, TRUE);
}

void disable_dio1_irq(void) {
    exint_interrupt_enable(LORA_DIO1_EXINT, FALSE);
}

void set_dio1_irq_callback(void (*cb)(void* context), void* context) {
    dio1_irq_callback = cb;
    dio1_context = context;
}

void EXINT15_10_IRQHandler(void) {
    if (exint_flag_get(LORA_DIO1_EXINT) != RESET) {
        exint_flag_clear(LORA_DIO1_EXINT);
        if (dio1_irq_callback != NULL) {
            dio1_irq_callback(dio1_context);
        }
    }
}

