/**
  **************************************************************************
  * @file     main.cpp
  * @brief    USB-CAN Converter main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * This application provides a bridge between USB Virtual COM Port
  * and CAN bus for the AT32F403A/407 microcontroller.
  *
  * USB data received -> CAN transmission
  * CAN data received -> USB transmission
  *
  **************************************************************************
  */

#include "main.h"
#include "at32f403a_407_clock.h"
#include "usb_can_bridge.h"
#include <string.h>

/** @addtogroup USB_CAN_CONVERTER
  * @{
  */

usbd_core_type usb_core_dev;

/**
  * @brief  USB 48MHz clock select
  * @param  clk_s: USB_CLK_HICK or USB_CLK_HEXT
  * @retval none
  */
void usb_clock48m_select(usb_clk48_s clk_s)
{
  if(clk_s == USB_CLK_HICK)
  {
    crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_HICK);

    /* enable the acc calibration ready interrupt */
    crm_periph_clock_enable(CRM_ACC_PERIPH_CLOCK, TRUE);

    /* update the c1\c2\c3 value */
    acc_write_c1(7980);
    acc_write_c2(8000);
    acc_write_c3(8020);

    /* open acc calibration */
    acc_calibration_mode_enable(ACC_CAL_HICKTRIM, TRUE);
  }
  else
  {
    switch(system_core_clock)
    {
      /* 48MHz */
      case 48000000:
        crm_usb_clock_div_set(CRM_USB_DIV_1);
        break;

      /* 72MHz */
      case 72000000:
        crm_usb_clock_div_set(CRM_USB_DIV_1_5);
        break;

      /* 96MHz */
      case 96000000:
        crm_usb_clock_div_set(CRM_USB_DIV_2);
        break;

      /* 120MHz */
      case 120000000:
        crm_usb_clock_div_set(CRM_USB_DIV_2_5);
        break;

      /* 144MHz */
      case 144000000:
        crm_usb_clock_div_set(CRM_USB_DIV_3);
        break;

      /* 168MHz */
      case 168000000:
        crm_usb_clock_div_set(CRM_USB_DIV_3_5);
        break;

      /* 192MHz */
      case 192000000:
        crm_usb_clock_div_set(CRM_USB_DIV_4);
        break;

      default:
        break;
    }
  }
}

/**
  * @brief  main function
  * @param  none
  * @retval none
  */
int main(void)
{
  /* config nvic priority group */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  /* system clock config */
  system_clock_config();

  /* initialize gpio for status LED */
  gpio_init_type gpio_init_struct;
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
  
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_13;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOC, &gpio_init_struct);

  /* initialize USB-CAN bridge */
  usb_can_bridge_init();

  /* select usb 48MHz clock source */
  usb_clock48m_select(USB_CLK_HEXT);

  /* enable usb clock */
  crm_periph_clock_enable(CRM_USB_PERIPH_CLOCK, TRUE);

  /* Note: USBFS_L_CAN1_RX0_IRQn is shared between USB and CAN RX0 */
  /* Priority must be set carefully */
  nvic_irq_enable(USBFS_L_CAN1_RX0_IRQn, 1, 0);
  nvic_irq_enable(CAN1_SE_IRQn, 0, 0);  /* High priority for CAN errors */

  /* usb core init */
  usbd_core_init(&usb_core_dev, USB, &cdc_class_handler, &cdc_desc_handler, 0);

  /* enable usb pull-up */
  usbd_connect(&usb_core_dev);

  /* initialize CAN */
  if(can_configuration() != SUCCESS)
  {
    /* CAN initialization failed - blink LED rapidly */
    while(1)
    {
      gpio_bits_write(GPIOC, GPIO_PINS_13, !gpio_output_data_bit_read(GPIOC, GPIO_PINS_13));
      delay_ms(100);
    }
  }

  /* main loop - bridge data between USB and CAN */
  while(1)
  {
    /* process USB to CAN data transfer */
    usb_can_bridge_process();

    /* toggle status LED slowly to indicate running */
    static uint32_t led_counter = 0;
    if(++led_counter >= 100000)
    {
      led_counter = 0;
      gpio_bits_write(GPIOC, GPIO_PINS_13, !gpio_output_data_bit_read(GPIOC, GPIO_PINS_13));
    }
  }
}

/**
  * @brief  USB delay millisecond function
  * @param  ms: number of millisecond delay
  * @retval none
  */
void usb_delay_ms(uint32_t ms)
{
  delay_ms(ms);
}

/**
  * @brief  USB delay microsecond function
  * @param  us: number of microsecond delay
  * @retval none
  */
void usb_delay_us(uint32_t us)
{
  delay_us(us);
}

/**
  * @}
  */

