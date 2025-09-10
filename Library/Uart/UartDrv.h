/// @file UartDrv.hpp
/// @ingroup RS485_Master
/// @brief Заголовочный файл драйвера RS485 Master *
/// @details Заголовочный файл драйвера RS485 Master


#ifndef _DRV_UART_H_
#define _DRV_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f403a_407_clock.h"
#include <stdint.h>
#include <stdio.h>
#include "at32f403a_407_wk_config.h"
#include <string.h>
#include "Macros.h"
#include "Consts.h"
#include "unitControl.h"

#define DEBUG_USART_TX_PIN    GPIO_PINS_9
#define DEBUG_USART_TX_GPIO_PORT    GPIOA
#define DEBUG_USART_RX_PIN    GPIO_PINS_10
#define DEBUG_USART_RX_GPIO_PORT    GPIOA

#define UART_TXBUF_SIZE (256)                  // Размер буфера передачи RS485 Master
#define UART_RXBUF_SIZE (256)                  // Размер приёмного буфера RS485 Master, 2^n !!!
    //#define RS485_RXBUF_MASK (RS485_RXBUF_SIZE - 1) // Маска приёмного буфера RS485 Master

#define USART1_BAUD_RATE (115200U)       // Скорость RS485 Master
#define RX_RING_BUF_SIZE (512U)


/// @brief инициализация периферии МК
    void drv_uart_init(void);

    /// @brief  Передаёт cnt байт данных из буфера по указателю pTxBuf в шину rs485.
    ///         Запускает процесс передачи: переключение линии в передачу, отправку данных,
    ///         переключение линии в приём
    /// @param [in] pTxBuf  :: указатель на буфер, данные из которого будут переданы в шину RS485
    /// @param [in] cnt     :: кол-во байт, которое необходимо передать
    void drv_uart_transmit(uint8_t* pTxBuf, uint16_t cnt); // передаёт cnt байт данных data по rs485
    // void drv_uart_transmit_str(const char* pTxBuf, uint16_t cnt);

    uint16_t drv_uart_read();

    /// @brief структура для работы драйвера RS485
    typedef struct {
        uint8_t txBuf[UART_TXBUF_SIZE];    /// Указатель на буфер с данными, которые будут отправлены в RS485
        uint16_t txCnt;      /// Кол-во байт, которые нужно отправить в RS485
        uint8_t rxDmaBuf[DMA1_CHANNEL1_BUFFER_SIZE];      /// buffer DMA
        uint8_t rxRingBuf[RX_RING_BUF_SIZE];
        /** Index of tail. */
        uint16_t rx_tail_index;
        /** Index of head. */
        uint16_t rx_head_index;
        /** Index of tail. */
        uint16_t dma_tail_index;
        /** Index of head. */
        uint16_t dma_head_index;
    } UartDrv_t;

    extern UartDrv_t uartDrv;

#ifdef __cplusplus
}
#endif
#endif // _DRV_RS485_H_
