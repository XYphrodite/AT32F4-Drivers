#include "UartApi.h"
#include "Command.h"
#include "UartDrv.h"
#include <fastmath.h>
#include <stdlib.h>
#include "Consts.h"

UartService::UartService(/* args */)
{
}

UartService::~UartService()
{
}

void UartService::Init(void)
{
    drv_uart_init();
}

bool UartService::Send(const uint8_t *buffer, uint16_t len)
{
    uint16_t newLen{0};
    PacketWrapper::Wrap(buffer, len, txbuf, &newLen, rxBuf[1]);
    drv_uart_transmit(txbuf, newLen);
    return TRUE;
}

uint16_t UartService::Read(void)
{
    return drv_uart_read();
}

bool UartService::TryGetCommand(void)
{
    if (myAbs((int)uartDrv.rx_head_index - (int)uartDrv.rx_tail_index) < 3)
        return false;
    for (; uartDrv.rx_tail_index != uartDrv.rx_head_index; uartDrv.rx_tail_index = CUT_OFF((uartDrv.rx_tail_index + 1), RX_RING_BUF_SIZE))
    {
        if (uartDrv.rxRingBuf[uartDrv.rx_tail_index] == REQUEST_COM_PREFIX)
        {
            uint8_t code = uartDrv.rxRingBuf[CUT_OFF(uartDrv.rx_tail_index + 1, RX_RING_BUF_SIZE)];
            uint8_t comSize = CmdHandler.GetCommandSize(code);
            if (comSize == 0)
            {
                uartDrv.rx_tail_index = CUT_OFF((uartDrv.rx_tail_index + 1), RX_RING_BUF_SIZE);
                if (myAbs((int)uartDrv.rx_head_index - (int)uartDrv.rx_tail_index) < 3)
                    return false;
                continue;
            } // command wasn't found
            if (myAbs((int)uartDrv.rx_head_index - (int)uartDrv.rx_tail_index) < comSize)
            {
                return false; // command isn't full
            }
            // put to RX
            uint8_t requestBytes[UART_RXBUF_SIZE] = {0};
            for (uint16_t i = 0; i < comSize; i++, uartDrv.rx_tail_index = CUT_OFF((uartDrv.rx_tail_index + 1), RX_RING_BUF_SIZE))
            {
                requestBytes[i] = uartDrv.rxRingBuf[uartDrv.rx_tail_index];
                uartDrv.rxRingBuf[uartDrv.rx_tail_index] = 0x00U;
            }
            // check CRC
            if (count_CRC(requestBytes, comSize - 1) != requestBytes[comSize - 1])
                return false;
            memcpy(rxBuf, requestBytes, comSize);
            return true;
        }
        uartDrv.rxRingBuf[uartDrv.rx_tail_index] = 0x0U;
    }
    return false;
}

void UartService::HandleCurrentRxBuff(void)
{
    CmdHandler.Do(rxBuf);
}
