/**
  **************************************************************************
  * @file     at32f403a_407_board.c
  * @brief    board support package
  **************************************************************************
  */

#include "at32f403a_407_board.h"

/**
  * @brief  delay for some time
  * @param  count: delay counter
  * @retval none
  */
void delay(uint32_t count)
{
  while(count--)
  {
    __NOP();
  }
}

/**
  * @brief  delay microseconds
  * @param  us: delay microseconds count
  * @retval none
  */
void delay_us(uint32_t us)
{
  uint32_t ticks = us * (system_core_clock / 1000000);
  delay(ticks / 5);
}

/**
  * @brief  delay milliseconds
  * @param  ms: delay milliseconds count
  * @retval none
  */
void delay_ms(uint16_t ms)
{
  while(ms--)
  {
    delay_us(1000);
  }
}

/**
  * @brief  delay seconds
  * @param  sec: delay seconds count
  * @retval none
  */
void delay_sec(uint16_t sec)
{
  while(sec--)
  {
    delay_ms(1000);
  }
}

