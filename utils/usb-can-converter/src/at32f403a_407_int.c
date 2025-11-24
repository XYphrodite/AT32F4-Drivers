/**
  **************************************************************************
  * @file     at32f403a_407_int.c
  * @brief    interrupt handlers
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#include "at32f403a_407_int.h"
#include "main.h"
#include "usb_can_bridge.h"

/** @addtogroup AT32F403A_periph_template
  * @{
  */

/** @addtogroup 403A_LED_toggle
  * @{
  */

/**
  * @brief  this function handles nmi exception.
  * @param  none
  * @retval none
  */
void NMI_Handler(void)
{
}

/**
  * @brief  this function handles hard fault exception.
  * @param  none
  * @retval none
  */
void HardFault_Handler(void)
{
  /* go to infinite loop when hard fault exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  this function handles memory manage exception.
  * @param  none
  * @retval none
  */
void MemManage_Handler(void)
{
  /* go to infinite loop when memory manage exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  this function handles bus fault exception.
  * @param  none
  * @retval none
  */
void BusFault_Handler(void)
{
  /* go to infinite loop when bus fault exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  this function handles usage fault exception.
  * @param  none
  * @retval none
  */
void UsageFault_Handler(void)
{
  /* go to infinite loop when usage fault exception occurs */
  while(1)
  {
  }
}

/**
  * @brief  this function handles svcall exception.
  * @param  none
  * @retval none
  */
void SVC_Handler(void)
{
}

/**
  * @brief  this function handles debug monitor exception.
  * @param  none
  * @retval none
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  this function handles pendsv_handler exception.
  * @param  none
  * @retval none
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  this function handles systick handler.
  * @param  none
  * @retval none
  */
void SysTick_Handler(void)
{
}

/**
  * @brief  this function handles USB and CAN1 RX0 interrupt.
  * @note   This interrupt is shared between USB Low Priority and CAN1 RX0
  * @param  none
  * @retval none
  */
void USBFS_L_CAN1_RX0_IRQHandler(void)
{
  /* Check if it's a USB interrupt */
  if(USB->intsts)
  {
    usbd_irq_handler(&usb_core_dev);
  }
  
  /* Check if it's a CAN interrupt */
  if(can_interrupt_flag_get(CAN1, CAN_RF0MN_FLAG) != RESET)
  {
    can_rx_callback();
  }
}

/**
  * @brief  this function handles CAN1 SE interrupt.
  * @param  none
  * @retval none
  */
void CAN1_SE_IRQHandler(void)
{
  can_error_callback();
}

/**
  * @}
  */

/**
  * @}
  */

