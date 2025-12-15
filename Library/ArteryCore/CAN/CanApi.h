#ifndef _CAN_API_H_
#define _CAN_API_H_

#include "at32f403a_407.h"
#include "can_driver.h"
#include <string.h>


class CanApi{
    public:
    void Init(void);
    void TransmitMessage(uint32_t id, uint8_t *data, uint8_t length);
    void ReceiveMessage(uint32_t *id, uint8_t *data, uint8_t *length);
    void DiagnoseErrors(void);
};

#endif