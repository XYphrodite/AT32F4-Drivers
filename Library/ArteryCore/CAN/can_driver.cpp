#include "can_driver.h"
#include <stddef.h>

/**
 * @brief Configure CAN bitrate timing parameters
 * @param bitrate_kbps CAN bitrate in kbps (125, 250, 500, 1000)
 * @param baudrate_div Output: prescaler value
 * @param rsaw_size Output: resynchronization jump width
 * @param bts1_size Output: bit segment 1 size
 * @param bts2_size Output: bit segment 2 size
 * @return SUCCESS if bitrate is supported, ERROR otherwise
 */
static error_status can_get_bitrate_timing(uint16_t bitrate_kbps,
                                           uint16_t *baudrate_div,
                                           can_rsaw_size_type *rsaw_size,
                                           can_bts1_size_type *bts1_size,
                                           can_bts2_size_type *bts2_size)
{
  /* Timing parameters for 120 MHz PCLK (AT32F403A/407 @ 240MHz, APB1 = 120MHz)
   * Formula: Bitrate = PCLK / (baudrate_div * (1 + BTS1 + BTS2))
   * Note: RSAW is for resynchronization, not part of bit time calculation
   * Bit time = 1 + 8 + 3 = 12 TQ, Sample point at 75% (9/12)
   */
  switch (bitrate_kbps)
  {
    case 125:
      *baudrate_div = 80;
      *rsaw_size = CAN_RSAW_3TQ;
      *bts1_size = CAN_BTS1_8TQ;
      *bts2_size = CAN_BTS2_3TQ;
      /* 120MHz / (80 * 12) = 125kHz - SKOV default bitrate */
      break;
    case 250:
      *baudrate_div = 40;
      *rsaw_size = CAN_RSAW_3TQ;
      *bts1_size = CAN_BTS1_8TQ;
      *bts2_size = CAN_BTS2_3TQ;
      /* 120MHz / (40 * 12) = 250kHz */
      break;
    case 500:
      *baudrate_div = 20;
      *rsaw_size = CAN_RSAW_3TQ;
      *bts1_size = CAN_BTS1_8TQ;
      *bts2_size = CAN_BTS2_3TQ;
      /* 120MHz / (20 * 12) = 500kHz */
      break;
    case 1000:
      *baudrate_div = 10;
      *rsaw_size = CAN_RSAW_3TQ;
      *bts1_size = CAN_BTS1_8TQ;
      *bts2_size = CAN_BTS2_3TQ;
      /* 120MHz / (10 * 12) = 1000kHz */
      break;
    default:
      return ERROR; /* Unsupported bitrate */
  }
  return SUCCESS;
}

error_status can_communication_configuration(uint16_t bitrate_kbps)
{
  can_base_type can_base_struct;
  can_baudrate_type can_baudrate_struct;
  can_filter_init_type can_filter_init_struct;
  uint16_t baudrate_div;
  can_rsaw_size_type rsaw_size;
  can_bts1_size_type bts1_size;
  can_bts2_size_type bts2_size;

  /* Check HEXT clock stability */
  if (crm_flag_get(CRM_HEXT_STABLE_FLAG) != SET)
  {
    return ERROR;
  }

  /* Enable CAN1 peripheral clock */
  crm_periph_clock_enable(CRM_CAN1_PERIPH_CLOCK, TRUE);

  /* Initialize CAN base configuration */
  can_default_para_init(&can_base_struct);
  can_base_struct.mode_selection = CAN_MODE_COMMUNICATE;  /* Normal mode */
  can_base_struct.aebo_enable = TRUE;                      /* Auto error bus-off */
  can_base_struct.aed_enable = TRUE;                       /* Auto error disable */
  can_base_init(CAN1, &can_base_struct);

  /* Get bitrate timing parameters */
  if (can_get_bitrate_timing(bitrate_kbps, &baudrate_div, &rsaw_size, 
                             &bts1_size, &bts2_size) != SUCCESS)
  {
    return ERROR; /* Unsupported bitrate */
  }

  /* Configure baudrate @ 120 MHz PCLK */
  can_baudrate_struct.baudrate_div = baudrate_div;
  can_baudrate_struct.rsaw_size = rsaw_size;
  can_baudrate_struct.bts1_size = bts1_size;
  can_baudrate_struct.bts2_size = bts2_size;
  if (can_baudrate_set(CAN1, &can_baudrate_struct) != SUCCESS)
  {
    return ERROR;
  }

  /* Configure filter to accept all messages */
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
  nvic_irq_enable(CAN1_SE_IRQn, 0, 0);
  nvic_irq_enable(USBFS_L_CAN1_RX0_IRQn, 0, 0);
  can_interrupts_enable();

  return SUCCESS;
}

static void can_configure_interrupts(confirm_state state)
{
  can_interrupt_enable(CAN1, CAN_RF0MIEN_INT, state);  /* RX FIFO0 message */
  can_interrupt_enable(CAN1, CAN_TCIEN_INT, state);    /* TX mailbox completion */
  can_interrupt_enable(CAN1, CAN_ETRIEN_INT, state);   /* Error type record */
  can_interrupt_enable(CAN1, CAN_EOIEN_INT, state);    /* Error occurrence */
}

void can_interrupts_enable(void)
{
  can_configure_interrupts(TRUE);
}

void can_interrupts_disable(void)
{
  can_configure_interrupts(FALSE);
}

void can_gpio_config(void)
{
  gpio_init_type gpio_init_struct;

  /* Enable GPIO clocks */
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  /* Configure PA11 (CAN_RX) */
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;
  gpio_init_struct.gpio_pins = GPIO_PINS_11;
  gpio_init(GPIOA, &gpio_init_struct);

  /* Configure PA12 (CAN_TX) */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_pins = GPIO_PINS_12;
  gpio_init(GPIOA, &gpio_init_struct);
}

error_status can_filter_mask_configuration(void)
{
  can_filter_init_type can_filter_init_struct;

  /* Accept messages with IDs 0x100-0x1FF */
  can_filter_init_struct.filter_activate_enable = TRUE;
  can_filter_init_struct.filter_mode = CAN_FILTER_MODE_ID_MASK;
  can_filter_init_struct.filter_fifo = CAN_FILTER_FIFO0;
  can_filter_init_struct.filter_number = 0;
  can_filter_init_struct.filter_bit = CAN_FILTER_32BIT;
  
  /* Standard ID mask: shift left 5 bits */
  can_filter_init_struct.filter_id_high = (0x100 << 5);
  can_filter_init_struct.filter_id_low = 0;
  can_filter_init_struct.filter_mask_high = 0xFFFFFFF0;  /* Mask high bits */
  can_filter_init_struct.filter_mask_low = 0;
  
  can_filter_init(CAN1, &can_filter_init_struct);
  return SUCCESS;
}

#define FILTER_STD_ID1  0x123
#define FILTER_STD_ID2  0x456
#define FILTER_EXT_ID1  0x18F5F100
#define FILTER_EXT_ID2  0x18F5F200

error_status can_filter_list_configuration(void)
{
  can_filter_init_type can_filter_init_struct;

  /* Filter 0: Extended ID List (accepts EXT_ID1 and EXT_ID2) */
  can_filter_init_struct.filter_activate_enable = TRUE;
  can_filter_init_struct.filter_mode = CAN_FILTER_MODE_ID_LIST;
  can_filter_init_struct.filter_fifo = CAN_FILTER_FIFO0;
  can_filter_init_struct.filter_number = 0;
  can_filter_init_struct.filter_bit = CAN_FILTER_32BIT;
  
  /* Extended ID encoding: shift left 3 bits, set IDE bit (0x04) */
  can_filter_init_struct.filter_id_high = ((FILTER_EXT_ID1 << 3) >> 16) & 0xFFFF;
  can_filter_init_struct.filter_id_low = ((FILTER_EXT_ID1 << 3) & 0xFFFF) | 0x04;
  can_filter_init_struct.filter_mask_high = ((FILTER_EXT_ID2 << 3) >> 16) & 0xFFFF;
  can_filter_init_struct.filter_mask_low = ((FILTER_EXT_ID2 << 3) & 0xFFFF) | 0x04;
  
  can_filter_init(CAN1, &can_filter_init_struct);

  /* Filter 1: Standard ID List (accepts STD_ID1 and STD_ID2) */
  can_filter_init_struct.filter_number = 1;
  
  /* Standard ID encoding: shift left 5 bits */
  can_filter_init_struct.filter_id_high = FILTER_STD_ID1 << 5;
  can_filter_init_struct.filter_id_low = 0;
  can_filter_init_struct.filter_mask_high = FILTER_STD_ID2 << 5;
  can_filter_init_struct.filter_mask_low = 0;
  
  can_filter_init(CAN1, &can_filter_init_struct);
  return SUCCESS;
}


void can_transmit_std_message(uint32_t std_id, uint8_t *data, uint8_t length)
{
    can_tx_message_type tx_message;
    uint8_t i;

    tx_message.standard_id = std_id;
    tx_message.extended_id = 0;
    tx_message.id_type = CAN_ID_STANDARD;
    tx_message.frame_type = CAN_TFT_DATA;
    tx_message.dlc = (length > 8) ? 8 : length;

    for(i = 0; i < tx_message.dlc; i++)
    {
        tx_message.data[i] = data[i];
    }

    can_message_transmit(CAN1, &tx_message);
}

void can_transmit_data(void)
{
  uint8_t transmit_mailbox;
  can_tx_message_type tx_message_struct;

  tx_message_struct.standard_id = 0x400;
  tx_message_struct.extended_id = 0;
  tx_message_struct.id_type = CAN_ID_STANDARD;
  tx_message_struct.frame_type = CAN_TFT_DATA;
  tx_message_struct.dlc = 8;
  tx_message_struct.data[0] = 0x11;
  tx_message_struct.data[1] = 0x22;
  tx_message_struct.data[2] = 0x33;
  tx_message_struct.data[3] = 0x44;
  tx_message_struct.data[4] = 0x55;
  tx_message_struct.data[5] = 0x66;
  tx_message_struct.data[6] = 0x77;
  tx_message_struct.data[7] = 0x88;

  transmit_mailbox = can_message_transmit(CAN1, &tx_message_struct);
  
  /* Wait for transmission complete */
  while (can_transmit_status_get(CAN1, (can_tx_mailbox_num_type)transmit_mailbox) 
         != CAN_TX_STATUS_SUCCESSFUL);
}

error_status can_receive_message(can_rx_message_type *rx_message)
{
    uint32_t pending_count;

    /* Check if messages are pending in FIFO0 */
    pending_count = can_receive_message_num_get(CAN1, CAN_RX_FIFO0);
    
    if (pending_count == 0)
    {
        return ERROR;  /* No messages available */
    }

    /* Receive the message */
    can_message_receive(CAN1, CAN_RX_FIFO0, rx_message);
    
    /* Release the FIFO entry */
    can_receive_release_fifo(CAN1, CAN_RX_FIFO0);

    return SUCCESS;
}

volatile uint8_t rx_data[8];
volatile uint8_t rx_length = 0;
volatile uint32_t rx_id = 0;

/* CANopen interrupt callback registration */
static can_interrupt_callback_t g_can_interrupt_callback = NULL;
static void *g_can_module_ptr = NULL;

/**
 * @brief Register a callback function for CAN interrupt handling
 * @param callback Function pointer to the interrupt handler (e.g., CO_CANinterrupt)
 * @param can_module Pointer to the CAN module structure (e.g., CO_CANmodule_t *)
 * @note If callback is registered, the ISRs will call it instead of default handling
 */
void can_register_interrupt_callback(can_interrupt_callback_t callback, void *can_module)
{
    g_can_interrupt_callback = callback;
    g_can_module_ptr = can_module;
}

void CAN1_RX0_IRQHandler(void)
{
    /* If CANopen callback is registered, use it */
    if (g_can_interrupt_callback != NULL && g_can_module_ptr != NULL)
    {
        g_can_interrupt_callback(g_can_module_ptr);
    }
    else
    {
        /* Default handling: store received data in global variables */
        can_rx_message_type rx_message;

        /* Check if message is available in FIFO0 */
        if (can_receive_message_num_get(CAN1, CAN_RX_FIFO0) > 0)
        {
            /* Receive the message */
            can_message_receive(CAN1, CAN_RX_FIFO0, &rx_message);
            
            /* Store received data */
            rx_id = (rx_message.id_type == CAN_ID_STANDARD) ? 
                    rx_message.standard_id : rx_message.extended_id;
            rx_length = rx_message.dlc;
            
            for (uint8_t i = 0; i < rx_message.dlc; i++)
            {
                rx_data[i] = rx_message.data[i];
            }
            
            /* Release FIFO entry */
            can_receive_release_fifo(CAN1, CAN_RX_FIFO0);
        }
    }
}

void CAN1_SE_IRQHandler(void)
{
    /* If CANopen callback is registered, use it */
    if (g_can_interrupt_callback != NULL && g_can_module_ptr != NULL)
    {
        g_can_interrupt_callback(g_can_module_ptr);
    }
    else
    {
        /* Default handling: read error counters */
        can_error_record_type error_type;
        uint8_t tx_err_cnt, rx_err_cnt;

        /* Check for errors */
        if (can_interrupt_flag_get(CAN1, CAN_EOIEN_INT) == SET)
        {
            error_type = can_error_type_record_get(CAN1);
            tx_err_cnt = can_transmit_error_counter_get(CAN1);
            rx_err_cnt = can_receive_error_counter_get(CAN1);
        }
    }
}


void can_diagnose_errors(void)
{
    can_error_record_type last_error;
    uint8_t tx_error_cnt, rx_error_cnt;

    /* Get last error type */
    last_error = can_error_type_record_get(CAN1);
    
    /* Get error counters */
    tx_error_cnt = can_transmit_error_counter_get(CAN1);
    rx_error_cnt = can_receive_error_counter_get(CAN1);
}

void can_cancel_pending_tx(can_type *can_x)
{
  for (can_tx_mailbox_num_type mb = CAN_TX_MAILBOX0; mb <= CAN_TX_MAILBOX2; mb++)
  {
    if (can_transmit_status_get(can_x, mb) == CAN_TX_STATUS_PENDING)
    {
      can_transmit_cancel(can_x, mb);
    }
  }
}