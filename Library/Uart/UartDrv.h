/// @file UartDrv.hpp
/// @ingroup RS485_Master
/// @brief Заголовочный файл драйвера RS485 Master *
/// @details Заголовочный файл драйвера RS485 Master

#ifndef _DRV_UART_H_
#define _DRV_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Consts.h"
#include "Macros.h"
#include "at32f403a_407_clock.h"
#include "at32f403a_407_usart.h"
#include "at32f403a_407_wk_config.h"
#include "unitControl.h"
#include <stdint.h>
#include <stdio.h>


#define DEBUG_USART_TX_PIN GPIO_PINS_9
#define DEBUG_USART_TX_GPIO_PORT GPIOA
#define DEBUG_USART_RX_PIN GPIO_PINS_10
#define DEBUG_USART_RX_GPIO_PORT GPIOA

#define UART_TXBUF_SIZE (256) // Размер буфера передачи RS485 Master
#define UART_RXBUF_SIZE (256) // Размер приёмного буфера RS485 Master, 2^n !!!
// #define RS485_RXBUF_MASK (RS485_RXBUF_SIZE - 1) // Маска приёмного буфера
// RS485 Master

#define USART1_BAUD_RATE (115200U) // Скорость RS485 Master
#define RX_RING_BUF_SIZE (512U)

/* USART mode enumeration */
typedef enum {
  USART_MODE_RX = 0x01,
  USART_MODE_TX = 0x02,
  USART_MODE_TX_RX = 0x03
} usart_mode_type;

/* UART reception mode enumeration */
typedef enum {
  UART_RX_MODE_DMA = 0x00,      /*!< Use DMA for reception (polling-based) */
  UART_RX_MODE_INTERRUPT = 0x01 /*!< Use interrupts for reception (event-driven) */
} uart_rx_mode_type;

/* Define USART init structure if not provided by vendor headers */
#ifndef __USART_INIT_TYPE_DEFINED
#define __USART_INIT_TYPE_DEFINED
typedef struct {
  usart_type* usart;
  uint32_t baudrate;
  usart_data_bit_num_type data_bit;
  usart_stop_bit_num_type stop_bit;
  usart_parity_selection_type parity;
  usart_mode_type mode;
  usart_hardware_flow_control_type hardware_flow_control;
  uart_rx_mode_type rx_mode;              // DMA or Interrupt mode for RX
  gpio_type* tx_gpio_port;                // GPIO port for TX pin
  uint16_t tx_gpio_pin;                   // GPIO pin for TX
  gpio_type* rx_gpio_port;                // GPIO port for RX pin
  uint16_t rx_gpio_pin;                   // GPIO pin for RX
  crm_periph_clock_type tx_gpio_clk;      // Clock for TX GPIO
  crm_periph_clock_type rx_gpio_clk;      // Clock for RX GPIO
  crm_periph_clock_type usart_clk;        // Clock for USART peripheral
  IRQn_Type usart_irq;                    // IRQ number for USART
  uint8_t irq_priority;                   // IRQ priority
  uint8_t irq_subpriority;                // IRQ sub-priority
} usart_init_type;
#endif

/// @brief инициализация периферии МК (без параметров — использует значения по
/// умолчанию)
void drv_uart_init(void);
/// @brief инициализация периферии МК с параметрами
void drv_uart_init_ex(const usart_init_type *init);
void drv_uart_init_adapter(void *cfg);
void drv_uart_init_apply(const usart_init_type *init);

/// @brief  Передаёт cnt байт данных из буфера по указателю pTxBuf в шину rs485.
///         Запускает процесс передачи: переключение линии в передачу, отправку
///         данных, переключение линии в приём
/// @param [in] pTxBuf  :: указатель на буфер, данные из которого будут переданы
/// в шину RS485
/// @param [in] cnt     :: кол-во байт, которое необходимо передать
void drv_uart_transmit(uint8_t *pTxBuf,
                       uint16_t cnt); // передаёт cnt байт данных data по rs485
// void drv_uart_transmit_str(const char* pTxBuf, uint16_t cnt);

uint16_t drv_uart_read();

/// @brief структура для работы драйвера RS485
typedef struct {
  uint8_t txBuf[UART_TXBUF_SIZE]; /// Указатель на буфер с данными, которые
                                  /// будут отправлены в RS485
  uint16_t txCnt; /// Кол-во байт, которые нужно отправить в RS485
  uint8_t rxDmaBuf[DMA1_CHANNEL1_BUFFER_SIZE]; /// buffer DMA
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
