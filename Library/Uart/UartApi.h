#ifndef _UART_SPI_H_
#define _UART_SPI_H_

#include "Consts.h"
#include "string.h"
#include "at32f403a_407.h"
#include "CRC.h"
#include "Math.h"
#include "UartDrv.h"
#include "PacketWrapper.h"

class UartService {
private:
    uint8_t txbuf[UART_TXBUF_SIZE];
    uint8_t rxBuf[UART_RXBUF_SIZE];


public:
    UartService(/* args */);
    ~UartService();
    static void Init(const usart_init_type* init_config);
    bool Send(const uint8_t* buffer, uint16_t len);
    uint16_t Read(void);
    bool TryGetCommand(void);
    void HandleCurrentRxBuff(void);
    bool TrySendStr(void);
};

extern UartDrv_t uartDrv;

#endif

