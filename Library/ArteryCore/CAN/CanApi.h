#ifndef _CAN_API_H_
#define _CAN_API_H_

#include "at32f403a_407.h"
#include "can_driver.h"

void can_init(void);
void can_transmit_message(uint32_t id, uint8_t *data, uint8_t length);
void can_receive_message(uint32_t *id, uint8_t *data, uint8_t *length);
void can_diagnose_errors(void);

#endif