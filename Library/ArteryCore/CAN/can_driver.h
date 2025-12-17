#ifndef _CAN_DRIVER_H_
#define _CAN_DRIVER_H_

#include "at32f403a_407.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Callback function pointer type for CAN interrupt handling */
typedef void (*can_interrupt_callback_t)(void *can_module);

error_status can_communication_configuration(void);
void can_gpio_config(void);
void can_filter_mask_configuration(void);
void can_filter_list_configuration(void);
void can_transmit_std_message(uint32_t std_id, uint8_t *data, uint8_t length);
void can_transmit_data(void);
void can_receive_message(can_rx_message_type *rx_message);
void can_diagnose_errors(void);
void can_interrupts_enable(void);
void can_interrupts_disable(void);
void can_cancel_pending_tx(can_type *can_x);

/* Register CANopen interrupt callback */
void can_register_interrupt_callback(can_interrupt_callback_t callback, void *can_module);

#ifdef __cplusplus
}
#endif

#endif