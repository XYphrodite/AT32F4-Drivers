#include "Tracing.h"

void __attribute__((optimize("O0"))) hal_print_trace(const char *fmt, ...)
{
    uint8_t out_buff[255] = {0};
    va_list args;
    va_start(args, fmt);
    int out_len = vsprintf((char *)out_buff, fmt, args);
    va_end(args);
    if (out_len <= 0 || out_len >= 255)
        return;
    
#ifdef DEBUG_PRINT_UART
    drv_uart_transmit(USART1, out_buff, (uint16_t)out_len);
#elif defined(SEGGER_RTT_ENABLED)
    SEGGER_RTT_Write(0, out_buff, out_len);
#elif defined(DEBUG_PRINT_USB)
    usb_vcp_send_data(&usb_core_dev, out_buff, out_len);
#else
    /* Default: use UART (USART1) */
    drv_uart_transmit(USART1, out_buff, (uint16_t)out_len);
#endif
}