#include "CanApi.h"
#include "can_driver.h"


void CanApi::Init(void){
    can_gpio_config();
    can_filter_mask_configuration();
    can_filter_list_configuration();
    can_communication_configuration();
}

void CanApi::TransmitMessage(uint32_t id, uint8_t *data, uint8_t length){
    can_transmit_std_message(id, data, length);
}

void CanApi::ReceiveMessage(uint32_t *id, uint8_t *data, uint8_t *length){
    can_rx_message_type rx_message;
    can_receive_message(&rx_message);
    *id = rx_message.standard_id;
    memcpy(data, rx_message.data, rx_message.dlc);
    *length = rx_message.dlc;
}

void CanApi::DiagnoseErrors(void){
    can_diagnose_errors();
}
