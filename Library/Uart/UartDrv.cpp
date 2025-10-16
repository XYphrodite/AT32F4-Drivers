/// @file UartDrv.cpp
/// @ingroup Uart
/// @brief Заголовочный файл драйвера UartDrv
/// @details Заголовочный файл драйвера UartDrv

#include "UartDrv.h"
#include <string.h>

/// @brief main variable of uart driver */
UartDrv_t uartDrv;


void drv_uart_init(const usart_init_type* init)
{
    if (init == NULL)
    {
        return;
    }

    /* Extract parameters with defaults */
    usart_type* us = (init->usart != NULL) ? init->usart : USART1;
    uint32_t baud = init->baudrate;
    usart_data_bit_num_type data_bits = init->data_bit;
    usart_stop_bit_num_type stop_bits = init->stop_bit;
    usart_parity_selection_type parity = init->parity;
    usart_hardware_flow_control_type flow = init->hardware_flow_control;
    uart_rx_mode_type rx_mode = init->rx_mode;

    /* Disable DMA if switching to INTERRUPT mode - determine DMA channel by USART */
    // if (rx_mode == UART_RX_MODE_INTERRUPT)
    // {
    //     if (us == USART1)
    //     {
    //         dma_channel_enable(DMA1_CHANNEL1, FALSE);
    //         dma_reset(DMA1_CHANNEL1);
    //     }
    //     else if (us == USART2)
    //     {
    //         dma_channel_enable(DMA1_CHANNEL6, FALSE);
    //         dma_reset(DMA1_CHANNEL6);
    //     }
    //     else if (us == USART3)
    //     {
    //         dma_channel_enable(DMA1_CHANNEL3, FALSE);
    //         dma_reset(DMA1_CHANNEL3);
    //     }
    //     /* Add other USARTs as needed */
    // }
    
    /* GPIO configuration with defaults */
    gpio_type* tx_port = (init->tx_gpio_port != NULL) ? init->tx_gpio_port : DEBUG_USART_TX_GPIO_PORT;
    uint16_t tx_pin = (init->tx_gpio_pin != 0) ? init->tx_gpio_pin : DEBUG_USART_TX_PIN;
    gpio_type* rx_port = (init->rx_gpio_port != NULL) ? init->rx_gpio_port : DEBUG_USART_RX_GPIO_PORT;
    uint16_t rx_pin = (init->rx_gpio_pin != 0) ? init->rx_gpio_pin : DEBUG_USART_RX_PIN;
    
    /* Enable clocks */
    if (init->tx_gpio_clk != 0)
    {
        crm_periph_clock_enable(init->tx_gpio_clk, TRUE);
    }
    else
    {
        crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
    }
    
    if (init->rx_gpio_clk != 0 && init->rx_gpio_clk != init->tx_gpio_clk)
    {
        crm_periph_clock_enable(init->rx_gpio_clk, TRUE);
    }
    
    if (init->usart_clk != 0)
    {
        crm_periph_clock_enable(init->usart_clk, TRUE);
    }
    else
    {
        crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);
    }

    /* Configure GPIO pins */
    gpio_init_type gpio_init_struct;
    gpio_default_para_init(&gpio_init_struct);

    /* Configure TX pin as alternate function push-pull */
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pins = tx_pin;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(tx_port, &gpio_init_struct);

    /* Configure RX pin as input */
    gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP; // Pull-up for idle high state
    gpio_init_struct.gpio_pins = rx_pin;
    gpio_init(rx_port, &gpio_init_struct);

    /* Initialize USART peripheral */
    usart_init(us, baud, data_bits, stop_bits);
    usart_transmitter_enable(us, TRUE);
    usart_receiver_enable(us, TRUE);
    usart_parity_selection_config(us, parity);
    usart_hardware_flow_control_set(us, flow);

    /* Configure reception mode: DMA or Interrupt */
    if (rx_mode == UART_RX_MODE_DMA)
    {
        /* DMA mode setup */
        crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
        
        /* init dma1 channel1 (USART1_RX) */
        wk_dma1_channel1_init();
        wk_dma_channel_config(DMA1_CHANNEL1,
            (uint32_t)&us->dt,
            (uint32_t)uartDrv.rxDmaBuf,
            DMA1_CHANNEL1_BUFFER_SIZE);
        dma_channel_enable(DMA1_CHANNEL1, TRUE);
        
        /* Enable DMA mode on USART */
        usart_dma_receiver_enable(us, TRUE);
        usart_dma_transmitter_enable(us, FALSE);
        
        /* Initialize DMA buffers */
        uartDrv.txCnt = 0;
        uartDrv.rx_tail_index = 0;
        uartDrv.rx_head_index = 0;
        uartDrv.dma_tail_index = 0;
        uartDrv.dma_head_index = 0;
        memset(uartDrv.rxDmaBuf, 0, DMA1_CHANNEL1_BUFFER_SIZE);
        memset(uartDrv.rxRingBuf, 0, RX_RING_BUF_SIZE);
        memset(uartDrv.txBuf, 0, UART_TXBUF_SIZE);
    }
    else if (rx_mode == UART_RX_MODE_INTERRUPT)
    {
        /* Interrupt mode setup */
        /* Disable DMA mode */
        usart_dma_receiver_enable(us, FALSE);
        usart_dma_transmitter_enable(us, FALSE);
        
        /* Configure NVIC for UART interrupt */
        if (init->usart_irq != 0)
        {
            nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
            nvic_irq_enable(init->usart_irq, init->irq_priority, init->irq_subpriority);
        }
        
        /* Interrupts will be enabled by application as needed */
        /* Initially disabled */
        usart_interrupt_enable(us, USART_RDBF_INT, FALSE);
        usart_interrupt_enable(us, USART_TDBE_INT, FALSE);
    }

    /* Enable USART */
    usart_enable(us, TRUE);
    
    /* Dummy read to clear any pending data */
    usart_data_receive(us);
}

void drv_uart_init_adapter(void* cfg)
{
    const usart_init_type* init = (const usart_init_type*)cfg;
    drv_uart_init(init);
}


void drv_uart_transmit(usart_type* usart, const uint8_t* pTxBuf, uint16_t cnt)
{
    // Validate parameters
    if (usart == NULL || pTxBuf == NULL || cnt == 0)
    {
        return;
    }
    
    // Validate buffer size
    if (cnt > UART_TXBUF_SIZE)
    {
        cnt = UART_TXBUF_SIZE;
    }
    
    memcpy(uartDrv.txBuf, pTxBuf, cnt);
    uartDrv.txCnt = cnt;

    uint32_t count = 0;
    
    for (uint16_t i = 0; i < uartDrv.txCnt; i++)
    {
        /* Wait for transmit data buffer empty before writing */
        count = 0;
        while (usart_flag_get(usart, USART_TDBE_FLAG) == RESET && count < HAL_UART_MAX_DELAY)
        {
            count++;
        }
        if (count >= HAL_UART_MAX_DELAY)
        {
            add_error_to_unit_control(UART_UNIT);
            return;
        }
        
        /* Write byte to transmit register */
        usart_data_transmit(usart, uartDrv.txBuf[i]);
        
        /* Wait for transmission complete before next byte - ensures byte is fully shifted out */
        count = 0;
        while (usart_flag_get(usart, USART_TDC_FLAG) == RESET && count < HAL_UART_MAX_DELAY)
        {
            count++;
        }
        if (count >= HAL_UART_MAX_DELAY)
        {
            add_error_to_unit_control(UART_UNIT);
            return;
        }
    }
}


void drv_uart_init_apply(const usart_init_type* init)
{
    uint32_t baud = (init != 0) ? init->baudrate : USART1_BAUD_RATE;
    usart_data_bit_num_type data_bits = (init != 0) ? (usart_data_bit_num_type)init->data_bit : USART_DATA_8BITS;
    usart_stop_bit_num_type stop_bits = (init != 0) ? (usart_stop_bit_num_type)init->stop_bit : USART_STOP_1_BIT;
    usart_parity_selection_type parity = (init != 0) ? (usart_parity_selection_type)init->parity : USART_PARITY_NONE;
    usart_hardware_flow_control_type flow = (init != 0) ? (usart_hardware_flow_control_type)init->hardware_flow_control : USART_HARDWARE_FLOW_NONE;

    usart_init(USART1, baud, data_bits, stop_bits);
    usart_transmitter_enable(USART1, TRUE);
    usart_receiver_enable(USART1, TRUE);
    usart_parity_selection_config(USART1, parity);
    usart_hardware_flow_control_set(USART1, flow);
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
