/**
  **************************************************************************
  * @file     usb_can_bridge.cpp
  * @brief    USB-CAN bridge implementation
  **************************************************************************
  *
  * This module implements the bidirectional data bridge between USB CDC
  * (Virtual COM Port) and CAN bus.
  *
  * USB -> CAN: Data received from USB is parsed and transmitted via CAN
  * CAN -> USB: CAN messages are formatted and transmitted via USB
  *
  **************************************************************************
  */

#include "usb_can_bridge.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

/** @addtogroup USB_CAN_BRIDGE
  * @{
  */

/* Circular buffers for USB->CAN and CAN->USB data */
#define BUFFER_SIZE 512

static uint8_t usb_rx_buffer[BUFFER_SIZE];
static uint16_t usb_rx_write_idx = 0;
static uint16_t usb_rx_read_idx = 0;

/* CAN receive message queue */
#define CAN_RX_QUEUE_SIZE 32
static can_rx_message_type can_rx_queue[CAN_RX_QUEUE_SIZE];
static uint8_t can_rx_queue_write = 0;
static uint8_t can_rx_queue_read = 0;

/* Statistics */
static volatile uint32_t usb_rx_count = 0;
static volatile uint32_t usb_tx_count = 0;
static volatile uint32_t can_rx_count = 0;
static volatile uint32_t can_tx_count = 0;

/**
  * @brief  Initialize USB-CAN bridge
  * @param  none
  * @retval none
  */
void usb_can_bridge_init(void)
{
  /* clear buffers */
  usb_rx_write_idx = 0;
  usb_rx_read_idx = 0;
  can_rx_queue_write = 0;
  can_rx_queue_read = 0;
  
  /* clear statistics */
  usb_rx_count = 0;
  usb_tx_count = 0;
  can_rx_count = 0;
  can_tx_count = 0;
}

/**
  * @brief  CAN configuration
  * @param  none
  * @retval SUCCESS or ERROR
  */
error_status can_configuration(void)
{
  can_base_type can_base_struct;
  can_baudrate_type can_baudrate_struct;
  can_filter_init_type can_filter_init_struct;
  
  /* Check if HEXT is ready - CAN requires stable external clock */
  if(crm_flag_get(CRM_HEXT_STABLE_FLAG) != SET)
  {
    return ERROR;
  }

  /* CAN GPIO configuration */
  gpio_init_type gpio_init_struct;
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
  gpio_pin_remap_config(CAN1_GMUX_0010, TRUE);

  /* CAN TX pin - PB9 */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOB, &gpio_init_struct);

  /* CAN RX pin - PB8 */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;
  gpio_init(GPIOB, &gpio_init_struct);

  /* Enable CAN1 clock */
  crm_periph_clock_enable(CRM_CAN1_PERIPH_CLOCK, TRUE);

  /* CAN base init */
  can_default_para_init(&can_base_struct);
  can_base_struct.mode_selection = CAN_MODE_COMMUNICATE;
  can_base_struct.ttc_enable = FALSE;
  can_base_struct.aebo_enable = TRUE;
  can_base_struct.aed_enable = TRUE;
  can_base_struct.prsf_enable = FALSE;
  can_base_struct.mdrsel_selection = CAN_DISCARDING_FIRST_RECEIVED;
  can_base_struct.mmssr_selection = CAN_SENDING_BY_ID;
  can_base_init(CAN1, &can_base_struct);

  /* CAN baudrate: 500 kbps @ 72MHz APB1
   * baudrate = pclk/(baudrate_div * (1 + bts1_size + bts2_size))
   * 500k = 36MHz / (6 * (1 + 8 + 3)) = 36MHz / 72 = 500kHz
   */
  can_baudrate_struct.baudrate_div = 6;
  can_baudrate_struct.rsaw_size = CAN_RSAW_3TQ;  /* Workaround for pulse disturbance */
  can_baudrate_struct.bts1_size = CAN_BTS1_8TQ;
  can_baudrate_struct.bts2_size = CAN_BTS2_3TQ;
  
  if(can_baudrate_set(CAN1, &can_baudrate_struct) != SUCCESS)
  {
    return ERROR;
  }

  /* CAN filter - accept all messages */
  can_filter_init_struct.filter_activate_enable = TRUE;
  can_filter_init_struct.filter_mode = CAN_FILTER_MODE_ID_MASK;
  can_filter_init_struct.filter_fifo = CAN_FILTER_FIFO0;
  can_filter_init_struct.filter_number = 0;
  can_filter_init_struct.filter_bit = CAN_FILTER_32BIT;
  can_filter_init_struct.filter_id_high = 0;
  can_filter_init_struct.filter_id_low = 0;
  can_filter_init_struct.filter_mask_high = 0;
  can_filter_init_struct.filter_mask_low = 0;
  can_filter_init(CAN1, &can_filter_init_struct);

  /* Enable CAN interrupts */
  can_interrupt_enable(CAN1, CAN_RF0MIEN_INT, TRUE);
  
  /* Enable CAN error interrupts - workaround for bit stuffing errors */
  can_interrupt_enable(CAN1, CAN_ETRIEN_INT, TRUE);
  can_interrupt_enable(CAN1, CAN_EOIEN_INT, TRUE);

  return SUCCESS;
}

/**
  * @brief  Format CAN message to ASCII string for USB transmission
  * @param  rx_msg: pointer to CAN RX message
  * @param  buffer: output buffer
  * @retval length of formatted string
  */
static uint16_t can_message_to_string(can_rx_message_type *rx_msg, uint8_t *buffer)
{
  uint16_t len = 0;
  
  /* Format: "CAN ID:xxx DLC:x DATA:xx xx xx...\r\n" */
  if(rx_msg->id_type == CAN_ID_STANDARD)
  {
    len = sprintf((char*)buffer, "CAN ID:%03lX DLC:%d DATA:", 
                  rx_msg->standard_id, rx_msg->dlc);
  }
  else
  {
    len = sprintf((char*)buffer, "CAN EID:%08lX DLC:%d DATA:", 
                  rx_msg->extended_id, rx_msg->dlc);
  }
  
  for(uint8_t i = 0; i < rx_msg->dlc && i < 8; i++)
  {
    len += sprintf((char*)&buffer[len], "%02X ", rx_msg->data[i]);
  }
  
  buffer[len++] = '\r';
  buffer[len++] = '\n';
  
  return len;
}

/**
  * @brief  Process USB to CAN data transfer
  * @param  none
  * @retval none
  */
static void process_usb_to_can(void)
{
  uint8_t data[64];
  uint16_t len;
  
  /* Get data from USB */
  len = usb_vcp_get_rxdata(&usb_core_dev, data);
  
  if(len > 0)
  {
    usb_rx_count += len;
    
    /* Copy to buffer */
    for(uint16_t i = 0; i < len; i++)
    {
      usb_rx_buffer[usb_rx_write_idx] = data[i];
      usb_rx_write_idx = (usb_rx_write_idx + 1) % BUFFER_SIZE;
    }
    
    /* Parse and transmit CAN messages */
    /* Simple protocol: send raw bytes as CAN data frames */
    /* Format: ID (4 bytes) + DLC (1 byte) + DATA (0-8 bytes) */
    
    while((usb_rx_write_idx != usb_rx_read_idx))
    {
      uint16_t available = 0;
      
      if(usb_rx_write_idx > usb_rx_read_idx)
        available = usb_rx_write_idx - usb_rx_read_idx;
      else
        available = BUFFER_SIZE - usb_rx_read_idx + usb_rx_write_idx;
      
      /* Need at least 5 bytes (ID + DLC) */
      if(available < 5)
        break;
      
      /* Extract CAN message */
      can_tx_message_type tx_msg;
      
      tx_msg.standard_id = (usb_rx_buffer[usb_rx_read_idx] << 8) | 
                           usb_rx_buffer[(usb_rx_read_idx + 1) % BUFFER_SIZE];
      usb_rx_read_idx = (usb_rx_read_idx + 2) % BUFFER_SIZE;
      
      tx_msg.extended_id = 0;
      tx_msg.id_type = CAN_ID_STANDARD;
      tx_msg.frame_type = CAN_TFT_DATA;
      tx_msg.dlc = usb_rx_buffer[usb_rx_read_idx];
      usb_rx_read_idx = (usb_rx_read_idx + 1) % BUFFER_SIZE;
      
      if(tx_msg.dlc > 8)
        tx_msg.dlc = 8;
      
      /* Check if we have enough data bytes */
      available = (usb_rx_write_idx >= usb_rx_read_idx) ?
                  (usb_rx_write_idx - usb_rx_read_idx) :
                  (BUFFER_SIZE - usb_rx_read_idx + usb_rx_write_idx);
      
      if(available < tx_msg.dlc)
        break;
      
      /* Extract data bytes */
      for(uint8_t i = 0; i < tx_msg.dlc; i++)
      {
        tx_msg.data[i] = usb_rx_buffer[usb_rx_read_idx];
        usb_rx_read_idx = (usb_rx_read_idx + 1) % BUFFER_SIZE;
      }
      
      /* Transmit CAN message */
      uint8_t mailbox = can_message_transmit(CAN1, &tx_msg);
      
      /* Wait for transmission (with timeout) */
      uint32_t timeout = 10000;
      while(can_transmit_status_get(CAN1, (can_tx_mailbox_num_type)mailbox) != CAN_TX_STATUS_SUCCESSFUL && timeout--)
      {
        /* Timeout or error */
      }
      
      if(timeout > 0)
      {
        can_tx_count++;
      }
    }
  }
}

/**
  * @brief  Process CAN to USB data transfer
  * @param  none
  * @retval none
  */
static void process_can_to_usb(void)
{
  /* Check if there are CAN messages in queue */
  while(can_rx_queue_read != can_rx_queue_write)
  {
    uint8_t buffer[128];
    uint16_t len;
    
    /* Format CAN message */
    len = can_message_to_string(&can_rx_queue[can_rx_queue_read], buffer);
    
    /* Send to USB */
    uint32_t timeout = 5000;
    while(usb_vcp_send_data(&usb_core_dev, buffer, len) != SUCCESS && timeout--)
    {
      /* Wait for USB ready */
    }
    
    if(timeout > 0)
    {
      usb_tx_count += len;
    }
    
    /* Move to next message */
    can_rx_queue_read = (can_rx_queue_read + 1) % CAN_RX_QUEUE_SIZE;
  }
}

/**
  * @brief  USB-CAN bridge main process
  * @param  none
  * @retval none
  */
void usb_can_bridge_process(void)
{
  /* Process USB to CAN */
  process_usb_to_can();
  
  /* Process CAN to USB */
  process_can_to_usb();
}

/**
  * @brief  CAN RX interrupt callback
  * @param  none
  * @retval none
  */
void can_rx_callback(void)
{
  if(can_interrupt_flag_get(CAN1, CAN_RF0MN_FLAG) != RESET)
  {
    /* Read CAN message */
    uint8_t next_write = (can_rx_queue_write + 1) % CAN_RX_QUEUE_SIZE;
    
    /* Check if queue is full */
    if(next_write != can_rx_queue_read)
    {
      can_message_receive(CAN1, CAN_RX_FIFO0, &can_rx_queue[can_rx_queue_write]);
      can_rx_queue_write = next_write;
      can_rx_count++;
    }
  }
}

/**
  * @brief  CAN error callback
  * @param  none
  * @retval none
  */
void can_error_callback(void)
{
  __IO uint32_t err_index = 0;
  
  if(can_interrupt_flag_get(CAN1, CAN_ETR_FLAG) != RESET)
  {
    err_index = CAN1->ests & 0x70;
    can_flag_clear(CAN1, CAN_ETR_FLAG);
    
    /* Bit stuffing error workaround */
    if(err_index == 0x00000010)
    {
      can_reset(CAN1);
      can_configuration();
    }
  }
}

/**
  * @brief  Get statistics
  * @param  stats: pointer to statistics structure
  * @retval none
  */
void usb_can_get_statistics(usb_can_statistics_type *stats)
{
  stats->usb_rx_count = usb_rx_count;
  stats->usb_tx_count = usb_tx_count;
  stats->can_rx_count = can_rx_count;
  stats->can_tx_count = can_tx_count;
}

/**
  * @}
  */

