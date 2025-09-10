// #include "UartStrBuf.h"

// #if DEBUG

// uint8_t strBuf[UART_STR_MAX][UART_TXBUF_SIZE];
// uint8_t strBuflLen[UART_STR_MAX];
// uint8_t ringStrB_Head;
// uint8_t ringStrB_Tail;

// void InitStrBuf(void) {
//     ringStrB_Head = 0;
//     ringStrB_Tail = 0;
//     memset(strBuflLen, 0, UART_STR_MAX);
// }

// // void AddStr(uint8_t* str, uint8_t len) {
// //     memcpy(strBuf[ringStrB_Head], str, CUT_OFF(len, UART_TXBUF_SIZE));
// //     strBuflLen[ringStrB_Head] = CUT_OFF(len, UART_TXBUF_SIZE);
// //     ringStrB_Head = CUT_OFF((ringStrB_Head + 1), UART_STR_MAX);
// // }

// // bool TrySendStr(void) {
// //     if (ringStrB_Head == ringStrB_Tail) return FALSE;
// //     drv_uart_transmit(strBuf[ringStrB_Tail], strBuflLen[ringStrB_Tail]);
// //     ringStrB_Tail = CUT_OFF((ringStrB_Tail + 1), UART_STR_MAX);
// //     return TRUE;
// // }

// #endif