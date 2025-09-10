#ifndef _USB_SERVICE_H_
#define _USB_SERVICE_H_

#include <at32f403a_407.h>
#include "PinDefines.h"
#include "gpio.h"
#include "usb_drv.h"
#include "usb_app.h"
#include "Consts.h"
#include "CommandHandler.h"
#include <cstdlib>
#include "PacketWrapper.h"

#define USB_TXBUF_SIZE (256U)
#define USB_RXBUF_SIZE (512U)

class UsbService
{
private:
    bool isVBUS2_pinInit = false;

    uint8_t txbuf[USB_TXBUF_SIZE];
    uint8_t rx_ring_Buf[USB_RXBUF_SIZE];

    uint16_t rx_buff_tail_index = 0;
    uint16_t rx_buff_head_index = 0;

    uint8_t usb_buffer[USB_RXBUF_SIZE] = {0};
    uint16_t data_len{0};

    uint32_t usb_limit_s = 0;

    uint8_t com_buffer[256] = {0};
    uint16_t comSize{0};

public:
    bool initUSB = false;

    UsbService(/* args */);
    ~UsbService();
    void Init(void);
    bool IsConnect(void);
    bool Send(const uint8_t *buffer, uint8_t len);
    uint16_t Read(void);
    bool TryGetCommand(void);
    void HandleCurrentRxBuff(void);

    uint32_t GetUsbLimitS(void);
    void AddUsbLimitS(uint32_t add);
};

#endif