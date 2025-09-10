#include "LoraModule.h"

LoraModule_Cfg lm_cfg;
LbsFlags lbs_flags;

LoraModule::LoraModule()
{
    lora_spi = Lora_SPI::GetInstance();
    lora_spi->gsc = &lm_cfg.gsc;
}

LoraModule::~LoraModule()
{
}

void LoraModule::SetPower(bool new_state)
{

    pinInit(lm_cfg.pwren_port, lm_cfg.pwren_pin, OUTPUT, GPIO_PULL_DOWN);
    if (new_state)
        gpio_bits_set(lm_cfg.pwren_port, lm_cfg.pwren_pin);
    else
        gpio_bits_reset(lm_cfg.pwren_port, lm_cfg.pwren_pin);
}

bool LoraModule::InitHard(device_type device_type)
{
    if (device_type == wireless_temperature_sensor)
    {
        spi_lora_init();
        lora_pins_init();
        lora_spi->SetLoraCfg(&lm_cfg);
        Reset();
#ifdef USE_LLCC68
        llcc68_hal_wakeup(&lora_spi);
#else
        sx126x_hal_wakeup(&lora_spi);
#endif
        if (SanityCheck() != true)
            return false;
        return true;
    }
    else if (device_type == lite_gateway_lbs)
    {
        lbs_flags = BootloadConfig::GetLbsFlags();
        if ((lbs_flags & lbs_new_board) || (lbs_flags & lbs_old_board))
        {
            spi_lora_init();
            SetPower(false);
            delay_ms(1000);
            SetPower(true);
            lora_pins_init();
            lora_spi->SetLoraCfg(&lm_cfg);
            Reset();
#ifdef USE_LLCC68
            llcc68_hal_wakeup(&lora_spi);
#else
            sx126x_hal_wakeup(&lora_spi);
#endif
            if (SanityCheck() != true)
                return false;
            return true;
        }
        else if (lbs_flags & lbs_unknown_board)
        {
            return DetectBoardType();
        }
        else
            return false;
    }
    else
        return false;
}

bool LoraModule::DetectBoardType(void)
{
    bool isNew = InitHard(lbs_new_board);
    if (isNew)
    {
        lbs_flags = static_cast<LbsFlags>(lbs_flags | lbs_new_board);
        BootloadConfig::SetLbsFlags(lbs_flags);
        return true;
    }
    SetPower(false);
    bool isOld = InitHard(lbs_old_board);
    if (isOld)
    {
        lbs_flags = static_cast<LbsFlags>(lbs_flags | lbs_old_board);
        BootloadConfig::SetLbsFlags(lbs_flags);
        return true;
    }
    SetPower(false);
    return false;
}

bool LoraModule::InitHard(LbsFlags _lbs_flags)
{
    lbs_flags = _lbs_flags;
    spi_lora_init();
    SetPower(false);
    delay_ms(1000);
    SetPower(true);
    lora_pins_init();
    lora_spi->SetLoraCfg(&lm_cfg);
    Reset();
#ifdef USE_LLCC68
    llcc68_hal_wakeup(&lora_spi);
#else
    sx126x_hal_wakeup(&lora_spi);
#endif
    if (SanityCheck() != true)
        return false;
    return true;
}

/// @brief init lora basic modem
/// @param
/// @return
bool LoraModule::InitSoftLBM(void)
{
    init_modem_tmr();
    return true;
}

bool LoraModule::IsBusy(void)
{
    return gpio_input_data_bit_read(lm_cfg.busy_type, lm_cfg.busy_pin) == SET;
}

bool LoraModule::WakeUp(void)
{
#ifdef USE_LLCC68
    return (llcc68_wakeup(&lora_spi) == (llcc68_status_t)LLCC68_STATUS_OK);
#else
    return (sx126x_wakeup(&lora_spi) == (sx126x_status_t)SX126X_HAL_STATUS_OK);
#endif
}

bool LoraModule::Reset(void)
{
#ifdef USE_LLCC68
    return (llcc68_reset(&lora_spi) == (llcc68_status_t)LLCC68_STATUS_OK);
#else
    return (sx126x_reset(&lora_spi) == (sx126x_status_t)SX126X_HAL_STATUS_OK);
#endif
}

bool LoraModule::Config(uint32_t freq)
{
    // Tell DIO2 to control the RF switch so we don't have to do it manually

#ifdef USE_LLCC68
    if (llcc68_set_dio2_as_rf_sw_ctrl(&lora_spi, TRUE) != (llcc68_status_t)LLCC68_STATUS_OK)
        return false;
#else
    if (sx126x_set_dio2_as_rf_sw_ctrl(lora_spi, TRUE) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return false;
#endif
    // Just a single SPI command to set the frequency, but it's broken out
    // into its own function so we can call it on-the-fly when the config changes
    ConfigSetFrequency(freq); // Set default frequency to 868mhz
// Set modem to LoRa (described in datasheet section 13.4.2)
#ifdef USE_LLCC68
    if (llcc68_set_pkt_type(&lora_spi, LLCC68_PKT_TYPE_LORA) != (llcc68_status_t)LLCC68_STATUS_OK)
        return false;
#else
    if (sx126x_set_pkt_type(lora_spi, SX126X_PKT_TYPE_LORA) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return false;
#endif
    // Set Rx Timeout to reset on SyncWord or Header detection
#ifdef USE_LLCC68
    if (llcc68_stop_timer_on_preamble(&lora_spi, FALSE) != (llcc68_status_t)LLCC68_STATUS_OK)
        return false;
#else
    if (sx126x_stop_timer_on_preamble(&lora_spi, FALSE) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return false;
#endif

    // Set modulation parameters is just one more SPI command, but since it
    // is often called frequently when changing the radio config, it's broken up into its own function
    ConfigSetPreset(PRESET_7_125); // Sets default modulation parameters

// Set PA Config
// See datasheet 13.1.4 for descriptions and optimal settings recommendations
#ifdef USE_LLCC68
    llcc68_pa_cfg_params_t pa_cfg;
#else
    sx126x_pa_cfg_params_t pa_cfg;
#endif
    pa_cfg.pa_duty_cycle = 0x04;
    pa_cfg.hp_max = 0x07;
    pa_cfg.device_sel = 0x00;
    pa_cfg.pa_lut = 0x01;
#ifdef USE_LLCC68
    if (llcc68_set_pa_cfg(&lora_spi, &pa_cfg) != (llcc68_status_t)LLCC68_STATUS_OK)
        return false;
#else
    if (sx126x_set_pa_cfg(&lora_spi, &pa_cfg) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return false;
#endif

    uint8_t ocp = 12;
    sx126x_set_ocp_value(lora_spi, ocp);
// Set TX Params
// See datasheet 13.4.4 for details
#ifdef END_DEVICE_TEMPERATURE_SENSOR
    int8_t lora_power = ModemConfig::GetLoraPower();
#endif

#ifdef LITE_GATEWAY
    int8_t lora_power = 22;
#endif
#ifdef USE_LLCC68
    if (llcc68_set_tx_params(&lora_spi, lora_power, LLCC68_RAMP_200_US) != (llcc68_status_t)LLCC68_STATUS_OK)
        return false;
#else
    // TODO low power mode
    if (sx126x_set_tx_params(&lora_spi, lora_power, SX126X_RAMP_40_US) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return false;
#endif

// Set LoRa Symbol Number timeout
// How many symbols are needed for a good receive.
// Symbols are preamble symbols
#ifdef USE_LLCC68
    if (llcc68_set_lora_symb_nb_timeout(&lora_spi, 0x00) != (llcc68_status_t)LLCC68_STATUS_OK)
        return false;
#else
    if (sx126x_set_lora_symb_nb_timeout(&lora_spi, 0x00) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return false;
#endif

// Enable interrupts
#ifdef USE_LLCC68
    if (llcc68_set_dio_irq_params(&lora_spi, LLCC68_IRQ_RX_DONE, 0xFFFF, 0x0000, 0x0000) != (llcc68_status_t)LLCC68_STATUS_OK)
        return false;
#else
    if (sx126x_set_dio_irq_params(&lora_spi, SX126X_IRQ_RX_DONE, 0xFFFF, 0x0000, 0x0000) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return false;
#endif

    // HAL_TRACE_INFO("Modem config = SUCCESS.\n");

    return true;
}

/* Tests that SPI is communicating correctly with the radio.
 * If this fails, check your SPI wiring.  This does not require any setup to run.
 * We test the radio by reading a register that should have a known value.
 *
 * Returns: True if radio is communicating over SPI. False if no connection.
 */
bool LoraModule::SanityCheck(void)
{
    uint8_t buffer[2] = {0x00};
#ifdef USE_LLCC68
    llcc68_status_t status = llcc68_read_register(&lora_spi, LLCC68_REG_LR_SYNCWORD, buffer, 2);
    if (status == LLCC68_STATUS_OK)
        return buffer[0] == 0x14;
#else
    sx126x_status_t status = sx126x_read_register(&lora_spi, SX126X_REG_LR_SYNCWORD, buffer, 2);
    if (status == SX126X_STATUS_OK)
        return buffer[0] == 0x14;
#endif
    return false;
}

bool LoraModule::IsWorking(void)
{
    uint8_t buffer[2] = {0x00};
#ifdef USE_LLCC68
    llcc68_status_t status = llcc68_read_register(&lora_spi, LLCC68_REG_LR_SYNCWORD, buffer, 2);
    if (status == LLCC68_STATUS_OK)
        return true;
#else
    sx126x_status_t status = sx126x_read_register(&lora_spi, SX126X_REG_LR_SYNCWORD, buffer, 2);
    if (status == SX126X_STATUS_OK)
        return true;
#endif
    return false;
}

bool LoraModule::SetPublic(void)
{
    sx126x_status_t status = sx126x_set_lora_sync_word(&lora_spi, 0x34);
    if (status == SX126X_STATUS_OK)
        return true;
    return false;
}

bool LoraModule::Transmit(uint8_t *payload, uint16_t PAYLOAD_SIZE)
{
    // Max lora packet size is 255 bytes
    if (PAYLOAD_SIZE > 255)
    {
        PAYLOAD_SIZE = 255;
    }

    // Switching directly from rx to tx mode is slow. Go to standby first
    if (inReceiveMode)
    {
        SetModeStandby();
    }
#ifdef USE_LLCC68
    llcc68_pkt_params_lora_t pkt_params;
    pkt_params.header_type = LLCC68_LORA_PKT_EXPLICIT; // Header Type. 0x00 = Variable Len, 0x01 = Fixed Length
#else
    sx126x_pkt_params_lora_t pkt_params;
    pkt_params.header_type = SX126X_LORA_PKT_EXPLICIT; // Header Type. 0x00 = Variable Len, 0x01 = Fixed Length
#endif
#ifdef END_DEVICE_TEMPERATURE_SENSOR
    pkt_params.preamble_len_in_symb = 0x0004;
#else
    pkt_params.preamble_len_in_symb = 0x000C;
#endif
    pkt_params.pld_len_in_bytes = PAYLOAD_SIZE; // Payload Length (Max is 255 bytes)
    pkt_params.crc_is_on = true;                // CRC Type. 0x00 = Off, 0x01 = on
    pkt_params.invert_iq_is_on = false;         // Invert IQ.  0x00 = Standard, 0x01 = Inverted
#ifdef USE_LLCC68
    if (llcc68_set_lora_pkt_params(&lora_spi, &pkt_params) != (llcc68_status_t)LLCC68_STATUS_OK)
        return false;
#else
    if (sx126x_set_lora_pkt_params(&lora_spi, &pkt_params) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return false;
#endif

    WaitForRadioCommandCompletion(1000); // Give time for radio to process the command

// Write the payload to the buffer
//   Reminder: PayloadLength is defined in setPacketParams
#ifdef USE_LLCC68
    if (llcc68_write_buffer(&lora_spi, 0, payload, PAYLOAD_SIZE) != (llcc68_status_t)LLCC68_STATUS_OK)
        return false;
#else
    if (sx126x_write_buffer(&lora_spi, 0, payload, PAYLOAD_SIZE) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return false;
#endif

    // SPI.transfer overwrites original buffer.  This could probably be confusing to the user
    // If they tried writing the same buffer twice and got different results
    // Eg "radio.transmit(buff,10); radio.transmit(buff,10);" would transmit two different packets
    // We'll make a performance+memory compromise and write in 32 byte chunks to avoid changing the contents
    // of the original data array
    // Copy contents to SPI buff until it's full, and then write that
    // TEST: I tested this method, which uses about 0.1ms (100 microseconds) more time, but it saves us about 10% of ram.
    // I think this is a fair trade

    WaitForRadioCommandCompletion(1000); // Give time for radio to process the command

// Transmit!
//  An interrupt will be triggered if we surpass our timeout
#ifdef USE_LLCC68
    if (llcc68_set_tx(&lora_spi, 0xFFFF) != (llcc68_status_t)LLCC68_STATUS_OK)
        return false;
#else
    if (sx126x_set_tx(&lora_spi, 0xFFFF) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return false;
#endif

    WaitForRadioCommandCompletion(this->transmitTimeout); // Wait for tx to complete, with a timeout so we don't wait forever
    WaitForRadioCommandCompletion(1000);                  // Wait for tx to complete, with a timeout so we don't wait forever

    // Remember that we are in Tx mode.  If we want to receive a packet, we need to switch into receiving mode
    inReceiveMode = false;

    return true;
}

/**This command will wait until the radio reports that it is no longer busy.
This is useful when waiting for commands to finish that take a while such as transmitting packets.
Specify a timeout (in milliseconds) to avoid an infinite loop if something happens to the radio

Returns TRUE on success, FALSE if timeout hit
*/
bool LoraModule::WaitForRadioCommandCompletion(uint32_t timeout)
{
    uint32_t startTime = 0;
    bool dataTransmitted = false;

    // Keep checking radio status until it has completed
    while (!dataTransmitted)
    {
        // Wait some time between spamming SPI status commands, asking if the chip is ready yet
        // Some commands take a bit before the radio even changes into a busy state,
        // so if we check too fast we might pre-maturely think we're done processing the command
        // 3ms delay gives inconsistent results.  4ms seems stable.  Using 5ms to be safe
        delay_ms(1);
        startTime += 1;

// Ask the radio for a status update
#ifdef USE_LLCC68
        llcc68_chip_status_t status;
        memset(&status, 0, sizeof(llcc68_chip_status_t));
        if (llcc68_get_status(&lora_spi, &status) != (llcc68_status_t)LLCC68_STATUS_OK)
            return false;
#else
        sx126x_chip_status_t status;
        memset(&status, 0, sizeof(sx126x_chip_status_t));
        if (sx126x_get_status(&lora_spi, &status) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
            return false;
#endif

        // Status 0, 1, 2 mean we're still busy.  Anything else means we're done.
        // Commands 3-6 = command timeout, command processing error, failure to execute command, and Tx Done (respoectively)
        if (status.cmd_status != 0 && status.cmd_status != 1 && status.cmd_status != 2)
        {
            dataTransmitted = true;
        }

        // If we're in standby mode, we don't need to wait at all
        // 0x03 = STBY_XOSC, 0x02= STBY_RC
        if (status.chip_mode == 0x03 || status.chip_mode == 0x02)
        {
            dataTransmitted = true;
        }

        // Avoid infinite loop by implementing a timeout
        if (startTime >= timeout)
        {
            return false;
        }
    }

    // We did it!
    return true;
}

bool LoraModule::WaitForRadioRx(uint32_t timeout)
{
    uint32_t startTime = 0;
    bool dataTransmitted = false;

    // Keep checking radio status until it has completed
    while (!dataTransmitted)
    {
        // Wait some time between spamming SPI status commands, asking if the chip is ready yet
        // Some commands take a bit before the radio even changes into a busy state,
        // so if we check too fast we might pre-maturely think we're done processing the command
        // 3ms delay gives inconsistent results.  4ms seems stable.  Using 5ms to be safe
        delay_ms(1);
        startTime += 1;

// Ask the radio for a status update
#ifdef USE_LLCC68
        llcc68_chip_status_t status;
        memset(&status, 0, sizeof(llcc68_chip_status_t));
        if (llcc68_get_status(&lora_spi, &status) != (llcc68_status_t)LLCC68_STATUS_OK)
            return false;
#else
        sx126x_chip_status_t status;
        memset(&status, 0, sizeof(sx126x_chip_status_t));
        if (sx126x_get_status(&lora_spi, &status) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
            return false;
#endif

        // Status 0, 1, 2 mean we're still busy.  Anything else means we're done.
        // Commands 3-6 = command timeout, command processing error, failure to execute command, and Tx Done (respoectively)
        if (status.cmd_status != 0 && status.cmd_status != 1 && status.cmd_status != 2)
        {
            dataTransmitted = true;
        }

        // If we're in standby mode, we don't need to wait at all
        // 0x03 = STBY_XOSC, 0x02= STBY_RC
        if (status.chip_mode == 0x03 || status.chip_mode == 0x02 || status.chip_mode == 0x05)
        {
            dataTransmitted = true;
        }

        // Avoid infinite loop by implementing a timeout
        if (startTime >= timeout)
        {
            return false;
        }
    }

    // We did it!
    return true;
}

// //Sets the radio into receive mode, allowing it to listen for incoming packets.
// //If radio is already in receive mode, this does nothing.
// //There's no such thing as "setModeTransmit" because it is set automatically when transmit() is called
void LoraModule::SetModeReceive(void)
{
    if (inReceiveMode)
        return; // We're already in receive mode, this would do nothing

// Set packet parameters
#ifdef USE_LLCC68
    llcc68_pkt_params_lora_t pkt_params;
    pkt_params.header_type = LLCC68_LORA_PKT_EXPLICIT; // Header Type. 0x00 = Variable Len, 0x01 = Fixed Length
#else
    sx126x_pkt_params_lora_t pkt_params;
    pkt_params.header_type = SX126X_LORA_PKT_EXPLICIT; // Header Type. 0x00 = Variable Len, 0x01 = Fixed Length
#endif
    pkt_params.preamble_len_in_symb = 0x000C;
    pkt_params.pld_len_in_bytes = 0xFF; // Payload Length (Max is 255 bytes)
    pkt_params.crc_is_on = FALSE;       // CRC Type. 0x00 = Off, 0x01 = on
    pkt_params.invert_iq_is_on = FALSE; // Invert IQ.  0x00 = Standard, 0x01 = Inverted
#ifdef USE_LLCC68
    if (llcc68_set_lora_pkt_params(&lora_spi, &pkt_params) != (llcc68_status_t)LLCC68_STATUS_OK)
        return;
#else
    if (sx126x_set_lora_pkt_params(&lora_spi, &pkt_params) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return;
#endif

// Tell the chip to wait for it to receive a packet.
// Based on our previous config, this should throw an interrupt when we get a packet
#ifdef USE_LLCC68
    if (llcc68_set_rx_with_timeout_in_rtc_step(&lora_spi, LLCC68_RX_CONTINUOUS) != (llcc68_status_t)LLCC68_STATUS_OK)
        return;
#else
    if (sx126x_set_rx_with_timeout_in_rtc_step(&lora_spi, SX126X_RX_CONTINUOUS) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return;
#endif

    // Remember that we're in receive mode so we don't need to run this code again unnecessarily
    inReceiveMode = true;
}

/*Set radio into standby mode.
Switching directly from Rx to Tx mode can be slow, so we first want to go into standby*/
bool LoraModule::SetModeStandby(void)
{
// Tell the chip to wait for it to receive a packet.
// Based on our previous config, this should throw an interrupt when we get a packet
#ifdef USE_LLCC68
    llcc68_standby_cfg_t cfg;
#else
    sx126x_standby_cfg_t cfg;
#endif
    // cfg = SX126X_STANDBY_CFG_XOSC;
    cfg = SX126X_STANDBY_CFG_RC;
#ifdef USE_LLCC68
    if (llcc68_set_standby(&lora_spi, cfg) != (llcc68_status_t)LLCC68_STATUS_OK)
        return false;
#else
    if (sx126x_set_standby(&lora_spi, cfg) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return false;
#endif

    WaitForRadioCommandCompletion(100);

    inReceiveMode = false; // No longer in receive mode
#ifdef USE_LLCC68

#else
    // if (!SetDCDC())
    //     return false;
#endif

    WaitForRadioCommandCompletion(100);

    return true;
}

bool LoraModule::SetDCDC(void)
{
    sx126x_reg_mod_t mode = SX126X_REG_MODE_DCDC;
    if (sx126x_set_reg_mode(&lora_spi, mode) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return false;
    return true;
}

bool LoraModule::SetSleepModeWithColdStart(void)
{
    lora_spi->NoWaitAfterCmd = true;
#ifdef USE_LLCC68
    llcc68_sleep_cfgs_t cfg = LLCC68_SLEEP_CFG_COLD_START;
    if (llcc68_set_sleep(&lora_spi, cfg) != (llcc68_status_t)LLCC68_STATUS_OK)
        return false;
#else
    sx126x_sleep_cfgs_t cfg = SX126X_SLEEP_CFG_COLD_START;
    if (sx126x_set_sleep(lora_spi, cfg) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return false;
#endif
    // WaitForRadioCommandCompletion(100);
    inReceiveMode = false; // No longer in receive mode
    return true;
}

// /*Receive a packet if available
// If available, this will return the size of the packet and store the packet contents into the user-provided buffer.
// A max length of the buffer can be provided to avoid buffer overflow.  If buffer is not large enough for entire payload, overflow is thrown out.
// Recommended to pass in a buffer that is 255 bytes long to make sure you can received any lora packet that comes in.

// Returns -1 when no packet is available.
// Returns 0 when an empty packet is received (packet with no payload)
// Returns payload size (1-255) when a packet with a non-zero payload is received. If packet received is larger than the buffer provided, this will return buffMaxLen
// */
int32_t LoraModule::Receive(uint8_t *buff, uint32_t buffMaxLen)
{
    SetModeReceive(); // Sets the mode to receive (if not already in receive mode)

// Radio pin DIO1 (interrupt) goes high when we have a packet ready.  If it's low, there's no packet yet
// if (gpio_output_data_bit_read(lm_cfg.dio1_type, lm_cfg.dio1_pin) == RESET) { return -1; } //Return -1, meanining no packet ready

// // Tell the radio to clear the interrupt, and set the pin back inactive.
// if (gpio_output_data_bit_read(lm_cfg.dio1_type, lm_cfg.dio1_pin) == SET) {
//     //Clear all interrupt flags.  This should result in the interrupt pin going low
//     if (sx126x_clear_irq_status(&g_spi, 0xFFFF) != (sx126x_status_t)SX126X_HAL_STATUS_OK) // (0xFFFF means clear all interrupts)
//         return -2;
// }
#ifdef USE_LLCC68
    if (llcc68_clear_irq_status(&lora_spi, 0xFFFF) != (llcc68_status_t)LLCC68_STATUS_OK) // (0xFFFF means clear all interrupts)
        return -2;
#else
    if (sx126x_clear_irq_status(&lora_spi, 0xFFFF) != (sx126x_status_t)SX126X_HAL_STATUS_OK) // (0xFFFF means clear all interrupts)
        return -2;
#endif

// (Optional) Read the packet status info from the radio.
// This is things like radio strength, noise, etc.
// See datasheet 13.5.3 for more info
// This provides debug info about the packet we received
#ifdef USE_LLCC68
    llcc68_pkt_status_lora_t p_st;
    if (llcc68_get_lora_pkt_status(&lora_spi, &p_st) != (llcc68_status_t)LLCC68_STATUS_OK)
        return -2;
#else
    sx126x_pkt_status_lora_t p_st;
    if (sx126x_get_lora_pkt_status(&lora_spi, &p_st) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return -2;
#endif

    // Store these values as class variables so they can be accessed if needed
    // Documentation for what these variables mean can be found in the .h file
    rssi = p_st.rssi_pkt_in_dbm;
    snr = p_st.snr_pkt_in_db;
    signalRssi = p_st.signal_rssi_pkt_in_dbm;

// We're almost ready to read the packet from the radio
// But first we have to know how big the packet is, and where in the radio memory it is stored
#ifdef USE_LLCC68
    llcc68_rx_buffer_status_t rx_buf_st;
    if (llcc68_get_rx_buffer_status(&lora_spi, &rx_buf_st) != (llcc68_status_t)LLCC68_STATUS_OK)
        return -2;
#else
    sx126x_rx_buffer_status_t rx_buf_st;
    if (sx126x_get_rx_buffer_status(&lora_spi, &rx_buf_st) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return -2;
#endif

    uint8_t payloadLen = rx_buf_st.pld_len_in_bytes;       // How long the lora packet is
    uint8_t startAddress = rx_buf_st.buffer_start_pointer; // Where in 1262 memory is the packet stored

    // //Make sure we don't overflow the buffer if the packet is larger than our buffer
    if (buffMaxLen < payloadLen)
    {
        payloadLen = buffMaxLen;
    }

// //Read the radio buffer from the SX1262 into the user-supplied buffer
#ifdef USE_LLCC68
    if (llcc68_read_buffer(&lora_spi, startAddress, buff, payloadLen) != (llcc68_status_t)LLCC68_HAL_STATUS_OK)
        return -2;

    if (llcc68_set_dio_irq_params(&lora_spi, LLCC68_IRQ_RX_DONE, 0xFFFF, 0x0000, 0x0000) != (llcc68_status_t)LLCC68_HAL_STATUS_OK)
        return -2;
#else
    if (sx126x_read_buffer(&lora_spi, startAddress, buff, payloadLen) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return -2;

    if (sx126x_set_dio_irq_params(&lora_spi, SX126X_IRQ_RX_DONE, 0xFFFF, 0x0000, 0x0000) != (sx126x_status_t)SX126X_HAL_STATUS_OK)
        return -2;
#endif

    return payloadLen; // Return how many bytes we actually read
}

// /*Waits for a packet to come in.  This code will block until something is received, or the timeout is hit.
// Set timeout=0 for no timeout, or set to a positive integer to specify a timeout in milliseconds
// This will store the contents of the payload into the user-provided buffer.  Recommended to use a buffer with 255 bytes to always receive full packet.
// If a smaller buffer is used, maxBuffLen can be set to avoid buffer overflow. Any packets larger than this will have remaining bytes thrown out.

// Returns -1 when no packet is available and timeout was hit.
// Returns 0 when an empty packet is received (packet with no payload)
// Returns payload size (1-255) when a packet with a non-zero payload is received. If packet received is larger than the buffer provided, this will return buffMaxLen
// */
// int LoraSx1262::lora_receive_blocking(byte* buff, int buffMaxLen, uint32_t timeout) {
//     setModeReceive(); //Sets the mode to receive (if not already in receive mode)

//     uint32_t startTime = millis();
//     uint32_t elapsed = startTime;

//     //Wait for radio interrupt pin to go high, indicating a packet was received, or if we hit our timeout
//     while (digitalRead(SX1262_DIO1) == false) {
//         //If user specified a timeout, check if we hit it
//         if (timeout > 0) {
//             elapsed = millis() - startTime;
//             if (elapsed >= timeout) {
//                 return -1;    //Return error, saying that we hit our timeout
//             }
//         }
//     }

//     //If our pin went high, then we got a packet!  Return it
//     return lora_receive_async(buff, buffMaxLen);
// }

// Set the radio modulation parameters.
// This is things like bandwitdh, spreading factor, coding rate, etc.
// This is broken into its own function because this command might get called frequently
void LoraModule::UpdateModulationParameters(void)
{
/*Set modulation parameters
# Modulation parameters are:
#  - SpreadingFactor
#  - Bandwidth
#  - CodingRate
#  - LowDataRateOptimize
# None of these actually matter that much.  You can set them to anything, and data will still show up
# on a radio frequency monitor.
# You just MUST call "setModulationParameters", otherwise the radio won't work at all*/
#ifdef USE_LLCC68
    llcc68_mod_params_lora_t params;
#else
    sx126x_mod_params_lora_t params;
#endif
    params.sf = this->spreadingFactor;       // ModParam1 = Spreading Factor.  Can be SF5-SF12, written in hex (0x05-0x0C)
    params.bw = this->bandwidth;             // ModParam2 = Bandwidth.  See Datasheet 13.4.5.2 for details. 0x00=7.81khz (slowest)
    params.cr = this->codingRate;            // ModParam3 = CodingRate.  Semtech recommends CR_4_5 (which is 0x01).  Options are 0x01-0x04, which correspond to coding rate 5-8 respectively
    params.ldro = this->lowDataRateOptimize; // LowDataRateOptimize.  0x00 = 0ff, 0x01 = On.  Required to be on for SF11 + SF12
#ifdef USE_LLCC68
    if (llcc68_set_lora_mod_params(&lora_spi, &params) != (llcc68_status_t)LLCC68_STATUS_OK)
        ;
#else
    if (sx126x_set_lora_mod_params(&lora_spi, &params) != (sx126x_status_t)SX126X_STATUS_OK)
        ;
#endif

    // Come up with a reasonable timeout for transmissions
    // SF12 is painfully slow, so we want a nice long timeout for that,
    // but we really don't want someone using SF5 to have to wait MINUTES for a timeout
    // I came up with these timeouts by measuring how long it actually took to transmit a packet
    // at each spreading factor with a MAX 255-byte payload and 7khz Bandwitdh (the slowest one)
    switch (this->spreadingFactor)
    {
    case 12:
        this->transmitTimeout = 252000; // Actual tx time 126 seconds
        break;
    case 11:
        this->transmitTimeout = 160000; // Actual tx time 81 seconds
        break;
    case 10:
        this->transmitTimeout = 60000; // Actual tx time 36 seconds
        break;
    case 9:
        this->transmitTimeout = 40000; // Actual tx time 20 seconds
        break;
    case 8:
        this->transmitTimeout = 20000; // Actual tx time 11 seconds
        break;
    case 7:
        this->transmitTimeout = 12000; // Actual tx time 6.3 seconds
        break;
    case 6:
        this->transmitTimeout = 7000; // Actual tx time 3.7s seconds
        break;
    default:                          // SF5
        this->transmitTimeout = 5000; // Actual tx time 2.2 seconds
        break;
    }
}

// //--------------------------
// // ADVANCED FUNCTIONS
// //--------------------------
// //The functions below are intended for advanced users who are more familiar with LoRa Radios at a lower level

/**(Optional) Use one of the pre-made radio configurations
 * This is ideal for making simple changes to the radio config
 * without needing to understand how the underlying settings work
 *
 * Argument: pass in one of the following
 *     - PRESET_DEFAULT:   Default radio config.
 *                         Medium range, medium speed
 *     - PRESET_FAST:      Faster speeds, but less reliable at long ranges.
 *                         Use when you need fast data transfer and have radios close together
 *     - PRESET_LONGRANGE: Most reliable option, but slow. Suitable when you prioritize
 *                         reliability over speed, or when transmitting over long distances
 */
bool LoraModule::ConfigSetPreset(int preset)
{
#ifdef USE_LLCC68
    if (preset == PRESET_DEFAULT)
    {
        this->bandwidth = LLCC68_LORA_BW_250;
        this->codingRate = LLCC68_LORA_CR_4_5;
        this->spreadingFactor = LLCC68_LORA_SF7;
        this->lowDataRateOptimize = 0; // Don't optimize (used for SF12 only)
        this->UpdateModulationParameters();
        return true;
    }
    if (preset == PRESET_FAST)
    {
        this->bandwidth = LLCC68_LORA_BW_500;
        this->codingRate = LLCC68_LORA_CR_4_5;
        this->spreadingFactor = LLCC68_LORA_SF5;
        this->lowDataRateOptimize = 0; // Don't optimize (used for SF12 only)
        this->UpdateModulationParameters();
        return true;
    }
    if (preset == PRESET_7_125)
    {
        this->bandwidth = LLCC68_LORA_BW_125;
        this->codingRate = LLCC68_LORA_CR_4_5;
        this->spreadingFactor = LLCC68_LORA_SF7;
        this->lowDataRateOptimize = 0; // Don't optimize (used for SF12 only)
        this->UpdateModulationParameters();
        return true;
    }
#else
    if (preset == PRESET_DEFAULT)
    {
        this->bandwidth = SX126X_LORA_BW_250;
        this->codingRate = SX126X_LORA_CR_4_5;
        this->spreadingFactor = SX126X_LORA_SF7;
        this->lowDataRateOptimize = 0; // Don't optimize (used for SF12 only)
        this->UpdateModulationParameters();
        return true;
    }
    if (preset == PRESET_LONGRANGE)
    {
        this->bandwidth = SX126X_LORA_BW_125;
        this->codingRate = SX126X_LORA_CR_4_5;
        this->spreadingFactor = SX126X_LORA_SF12;
        this->lowDataRateOptimize = 1; // Optimize for low data rate (SF12 only)
        this->UpdateModulationParameters();
        return true;
    }
    if (preset == PRESET_FAST)
    {
        this->bandwidth = SX126X_LORA_BW_500;
        this->codingRate = SX126X_LORA_CR_4_5;
        this->spreadingFactor = SX126X_LORA_SF5;
        this->lowDataRateOptimize = 0; // Don't optimize (used for SF12 only)
        this->UpdateModulationParameters();
        return true;
    }
    if (preset == PRESET_7_125)
    {
        this->bandwidth = SX126X_LORA_BW_125;
        this->codingRate = SX126X_LORA_CR_4_5;
        this->spreadingFactor = SX126X_LORA_SF7;
        this->lowDataRateOptimize = 0; // Don't optimize (used for SF12 only)
        this->UpdateModulationParameters();
        return true;
    }
#endif
    // Invalid preset specified
    return false;
}

/** (Optional) Set the operating frequency of the radio.
 * The 1262 radio supports 150-960Mhz.  This library uses a default of 915Mhz.
 * MAKE SURE THAT YOU ARE OPERATING IN A FREQUENCY THAT IS ALLOWED IN YOUR COUNTRY!
 * For example, 915mhz (915000000 hz) is safe in the US.
 *
 * Specify the desired frequency in Hz (eg 915MHZ is 915000000).
 * Returns TRUE on success, FALSE on invalid frequency
 */
bool LoraModule::ConfigSetFrequency(long frequencyInHz)
{
    // Make sure the specified frequency is in the valid range.
    if (frequencyInHz < 803000000 || frequencyInHz > 930000000)
    {
        return false;
    }
#ifdef USE_LLCC68
    llcc68_set_rf_freq(&lora_spi, frequencyInHz);
#else
    sx126x_set_rf_freq(lora_spi, frequencyInHz);
#endif

    return true;
}

void LoraModule::EnableDIO1(bool new_state)
{
    if (new_state)
        enable_dio1_irq();
    else
        disable_dio1_irq();
}

void LoraModule::SetDIO1Handler(void (*cb)(void *context), void *context)
{
    set_dio1_irq_callback(cb, context);
}

// /*Set the bandwith (basically, this is how big the frequency span is that we occupy)
// * Bigger bandwidth allows us to transmit large amounts of data faster, but it occupies a larger span of frequencies.
// * Smaller bandwith takes longer to transmit large amounts of data, but its less likely to collide with other frequencies.
// *
// * Available bandwidth settings, pulled from datasheet 13.4.5.2
// *  SETTING.   | Bandwidth
// * ------------+-----------
// *    0x00     |    7.81khz
// *    0x08     |   10.42khz
// *    0x01     |   15.63khz
// *    0x09     |   20.83khz
// *    0x02     |   31.25khz
// *    0x0A     |   41.67khz
// *    0x03     |   62.50khz
// *    0x04     |  125.00khz
// *    0x05     |  250.00khz (default)
// *    0x06     |  500.00khz
// *
// * Returns TRUE on success, FALSE on failure (invalid bandwidth)
// */
// bool LoraSx1262::configSetBandwidth(int bandwidth) {
//     //Bandwidth setting must be 0-10 (excluding 7 for some reason)
//     if (bandwidth < 0 || bandwidth > 0x0A || bandwidth == 7) { return false; }
//     this->bandwidth = bandwidth;
//     this->updateModulationParameters();
//     return true;
// }

// /*I honestly don't really know what coding rate means.  It's something technical to have to do with radios
// * Set it here if you want.  See datasheet 13.4.5.2 for details
// *  SETTING. | Coding Rate
// * ----------+--------------------
// *    0x01   |   CR_4_5 (default)
// *    0x02   |   CR_4_6
// *    0x03   |   CR_4_7
// *    0x04   |   CR_4_8
// *
// * Returns TRUE on success, FALSE on failure (invalid coding rate)
// */
// bool LoraSx1262::configSetCodingRate(int codingRate) {
//     //Coding rate must be 1-4 (inclusive)
//     if (codingRate < 1 || codingRate > 4) { return false; }
//     this->codingRate = codingRate;
//     this->updateModulationParameters();
//     return true;
// }

// /*Change the spreading factor of a packet
// The higher the spreading factor, the slower and more reliable the transmission will be.
// Higher spreading factors are good for longer distances with slower transmit speeds.
// Lower spreading factors are good when the radios are close, which allows faster transmission speeds.

// * Setting | Spreading Factor
// * --------+---------------------------
// *    5    | SF5 (fastest, short range)
// *    6    | SF6
// *    7    | SF7 (default)
// *    8    | SF8
// *    9    | SF9
// *   10    | SF10
// *   11    | SF11
// *   12    | SF12 (Slowest, long range, most reliable)
// *
// * Returns TRUE on success, FALSE on failure (incorrect spreading factor)
// */
// bool LoraSx1262::configSetSpreadingFactor(int spreadingFactor) {
//     if (spreadingFactor < 5 || spreadingFactor > 12) { return false; }

//     //The datasheet highly recommends enabling "LowDataRateOptimize" for SF11 and SF12
//     this->lowDataRateOptimize = (spreadingFactor >= 11) ? 1 : 0;  //Turn on for SF11+SF12, turn off for anything else
//     this->spreadingFactor = spreadingFactor;
//     this->updateModulationParameters();
//     return true;
// }
