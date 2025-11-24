/**
  **************************************************************************
  * @file     at32f403a_407_board.h
  * @brief    header file for board support package
  **************************************************************************
  */

#ifndef __AT32F403A_407_BOARD_H
#define __AT32F403A_407_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f403a_407.h"

void delay(uint32_t count);
void delay_us(uint32_t us);
void delay_ms(uint16_t ms);
void delay_sec(uint16_t sec);

#ifdef __cplusplus
}
#endif

#endif /* __AT32F403A_407_BOARD_H */

