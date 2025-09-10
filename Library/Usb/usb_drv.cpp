#include "usb_drv.h"

void init_usb_drv(void)
{
  crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_HICK);
  /* enable usb periph clock */
  crm_periph_clock_enable(CRM_USB_PERIPH_CLOCK, TRUE);
  /* enable the acc calibration ready interrupt */
  crm_periph_clock_enable(CRM_ACC_PERIPH_CLOCK, TRUE);
  /* update the c1\c2\c3 value */
  acc_write_c1(7980);
  acc_write_c2(8000);
  acc_write_c3(8020);
  /* open acc calibration */
  acc_calibration_mode_enable(ACC_CAL_HICKTRIM, TRUE);

  nvic_irq_enable(USBFS_L_CAN1_RX0_IRQn, 0, 0);
  nvic_irq_enable(USBFS_H_CAN1_TX_IRQn, 0, 0);
  
}

void usb_delay_ms(uint32_t ms)
{
  delay_ms(ms);
}
