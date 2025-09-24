#ifndef _LORA_MODULE_
#define _LORA_MODULE_

#include "at32f403a_407.h"
#include "at32f403a_407_wk_config.h"
#include "gpio.h"

#ifdef USE_LLCC68
#include "llcc68.h"
#include "llcc68_hal.h"
#include "llcc68_regs.h"
#else
#include "sx126x.h"
#include "sx126x_hal.h"
#include "sx126x_regs.h"
#endif
#include "string.h"
#include "LoraIRQ.h"
#include "ModemTmr.h"
#include "lora_spi.h"
#include "LoraSPI.h"
#include "Tracing.h"
#include "DelayService.h"
#ifdef END_DEVICE_TEMPERATURE_SENSOR
#include "ModemConfig.h"
#endif
#ifdef LITE_GATEWAY
#include "GatewayConfig.h"
#endif
#include "BootloadConfig.h"
#include "PinDefines.h"
#include "Enums.h"

#define PRESET_DEFAULT 0
#define PRESET_LONGRANGE 1
#define PRESET_FAST 2
#define PRESET_7_125 3

class LoraModule
{
private:
    uint8_t spiRxBuf[DMA1_CHANNEL3_BUFFER_SIZE];

    bool inReceiveMode = false;
    // Config variables (set to PRESET_DEFAULT on init)
    uint32_t pllFrequency;
#ifdef USE_LLCC68
    llcc68_lora_bw_t bandwidth;
    llcc68_lora_cr_t codingRate;
    llcc68_lora_sf_t spreadingFactor;
#else
    sx126x_lora_bw_t bandwidth;
    sx126x_lora_cr_t codingRate;
    sx126x_lora_sf_t spreadingFactor;
#endif

    uint8_t lowDataRateOptimize;
    uint32_t transmitTimeout; // Worst-case transmit time depends on some factors
    Lora_SPI *lora_spi;

public:
    LoraModule();
    ~LoraModule();
    void SetPower(bool new_state);
    bool InitHard(device_type device_type);
    bool InitHard(LbsFlags _lbs_flags);
    bool DetectBoardType(void);
    bool InitSoftLBM();
    bool WakeUp(void);
    bool Reset(void);
    bool SanityCheck(void);
    bool Config(uint32_t freq);
    bool Transmit(uint8_t *payload, uint16_t PAYLOAD_SIZE);
    bool ConfigSetFrequency(long frequencyInHz);
    bool UpdateRadioFrequency(void);
    bool ConfigSetPreset(int preset);
    void UpdateModulationParameters(void);
    bool SetModeStandby(void);
    bool SetSleepModeWithColdStart(void);
    bool WaitForRadioCommandCompletion(uint32_t timeout);
    bool WaitForRadioRx(uint32_t timeout);
    bool IsBusy(void);
    void SetModeReceive(void);
    int32_t Receive(uint8_t *buff, uint32_t buffMaxLen);
    void EnableDIO1(bool new_state);
    void SetDIO1Handler(void (*cb)(void *context), void *context);
    bool SetDCDC(void);
    bool SetPublic(void);
    bool IsWorking(void);

    // These variables show signal quality, and are updated automatically whenever a packet is received
    int rssi = 0;
    int snr = 0;
    int signalRssi = 0;
};

#endif
