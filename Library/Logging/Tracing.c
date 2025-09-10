#include "Tracing.h"

void __attribute__((optimize("O0"))) hal_print_trace(const char *fmt, ...)
{
    uint8_t out_buff[255] = {0};
    va_list args;
    va_start(args, fmt);
    uint8_t out_len = vsprintf((char *)out_buff, fmt, args);
    va_end(args);
    if (out_len == 0)
        return;
    // SEGGER_RTT_Write(0, out_buff, out_len);
    return;

// #ifdef DEBUG_PRINT_UART
//     else drv_uart_transmit((uint8_t *)out_buff, out_len);
// #endif
// #ifdef DEBUG_PRINT_USB
//     else usb_vcp_send_data(&usb_core_dev, out_buff, out_len);
// #endif
}