/// @file UartDrv.cpp
/// @ingroup Uart
/// @brief Заголовочный файл драйвера UartDrv
/// @details Заголовочный файл драйвера UartDrv

#include "UartDrv.h"

/// @brief main variable of uart driver */
UartDrv_t uartDrv;


void drv_uart_init(void) {
    crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
    /* init dma1 channel1 */
    wk_dma1_channel1_init();
    /* config dma channel transfer parameter */
    /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */

    wk_dma_channel_config(DMA1_CHANNEL1,
        (uint32_t)&USART1->dt,
        (uint32_t)uartDrv.rxDmaBuf,
        DMA1_CHANNEL1_BUFFER_SIZE);
    dma_channel_enable(DMA1_CHANNEL1, TRUE);

    gpio_init_type gpio_init_struct;
    gpio_default_para_init(&gpio_init_struct);

    /* configure the TX pin */
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pins = DEBUG_USART_TX_PIN;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(DEBUG_USART_TX_GPIO_PORT, &gpio_init_struct);

    /* configure the RX pin */
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
    gpio_init_struct.gpio_pins = DEBUG_USART_RX_PIN;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(DEBUG_USART_RX_GPIO_PORT, &gpio_init_struct);

    /* configure param */
    usart_init(USART1, USART1_BAUD_RATE, USART_DATA_8BITS, USART_STOP_1_BIT);
    usart_transmitter_enable(USART1, TRUE);
    usart_receiver_enable(USART1, TRUE);
    usart_parity_selection_config(USART1, USART_PARITY_NONE);

    usart_dma_receiver_enable(USART1, TRUE);

    usart_hardware_flow_control_set(USART1, USART_HARDWARE_FLOW_NONE);

    usart_enable(USART1, TRUE);

    usart_data_receive(USART1);

    uartDrv.txCnt = 0;
    uartDrv.rx_tail_index = 0;
    uartDrv.rx_head_index = 0;
    uartDrv.dma_tail_index = 0;
    uartDrv.dma_head_index = 0;
    memset(uartDrv.rxDmaBuf, 0, DMA1_CHANNEL1_BUFFER_SIZE);
    memset(uartDrv.rxRingBuf, 0, RX_RING_BUF_SIZE);
    memset(uartDrv.txBuf, 0, UART_TXBUF_SIZE);
}

void drv_uart_transmit(uint8_t* pTxBuf, uint16_t cnt) {
    // если указатель на передаваемые данные нулевой или число передаваемых байт равно нулю - вхыодим
    if ((pTxBuf == 0) || (cnt == 0)) {
        return;
    }
    memcpy(uartDrv.txBuf, pTxBuf, cnt);
    uartDrv.txCnt = cnt;

    uint32_t count = 0;
    for (uint8_t i = 0;i < uartDrv.txCnt;i++) {
        while (usart_flag_get(USART1, USART_TDBE_FLAG) == RESET && count < HAL_MAX_DELAY);
        if (count >= HAL_MAX_DELAY) {
            add_error_to_unit_control(UART_UNIT);
            return;
        }
        usart_data_transmit(USART1, uartDrv.txBuf[i]);
    }
}


uint16_t drv_uart_read() {
    uint16_t dmaNum = DMA1_CHANNEL1_BUFFER_SIZE - (dma_data_number_get(DMA1_CHANNEL1) + uartDrv.dma_head_index);
    if (dmaNum > 0) {
        uint16_t newBytes = dmaNum;
        uartDrv.dma_head_index = CUT_OFF((uartDrv.dma_head_index + newBytes), DMA1_CHANNEL1_BUFFER_SIZE);
        while (newBytes--) {
            uartDrv.rxRingBuf[uartDrv.rx_head_index] = uartDrv.rxDmaBuf[uartDrv.dma_tail_index];
            uartDrv.rx_head_index = CUT_OFF((uartDrv.rx_head_index + 1), RX_RING_BUF_SIZE);
            uartDrv.dma_tail_index = CUT_OFF((uartDrv.dma_tail_index + 1), DMA1_CHANNEL1_BUFFER_SIZE);
        }
    }
    return dmaNum;
}
