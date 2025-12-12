#include "CanApi.h"

void can_init(void){
    can_gpio_config();
    can_filter_mask_configuration();
    can_filter_list_configuration();
    can_communication_configuration();
}

void can_transmit_message(uint32_t id, uint8_t *data, uint8_t length){
    can_transmit_std_message(id, data, length);
}

void can_receive_message(uint32_t *id, uint8_t *data, uint8_t *length){
    can_receive_message(id, data, length);
}

void can_diagnose_errors(void){
    can_diagnose_errors();
}