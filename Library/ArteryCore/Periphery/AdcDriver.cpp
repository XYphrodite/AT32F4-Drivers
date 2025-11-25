#include "AdcDriver.h"
#include "gpio.h"

// Simple memory copy function (since cstring may not be available in bare-metal)
static void* adc_memcpy(void* dest, const void* src, size_t n) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

// Simple memory set function
static void* adc_memset(void* s, int c, size_t n) {
    uint8_t* p = (uint8_t*)s;
    while (n--) {
        *p++ = (uint8_t)c;
    }
    return s;
}

// Static variables to store configuration
static AdcConfig_t s_adc_configs[3]; // Support for ADC1, ADC2, ADC3
static AdcMultiChannel_t s_multi_configs[3];

// Helper function to get ADC index (0, 1, or 2)
static uint8_t AdcDriver_GetIndex(adc_type* adc_x) {
    if (adc_x == ADC1) return 0;
    if (adc_x == ADC2) return 1;
    if (adc_x == ADC3) return 2;
    return 0xFF; // Invalid
}

// Helper function to enable ADC peripheral clock
static void AdcDriver_EnableClock(adc_type* adc_x) {
    if (adc_x == ADC1) {
        crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);
    } else if (adc_x == ADC2) {
        crm_periph_clock_enable(CRM_ADC2_PERIPH_CLOCK, TRUE);
    } else if (adc_x == ADC3) {
        crm_periph_clock_enable(CRM_ADC3_PERIPH_CLOCK, TRUE);
    }
}

// Helper function to get DMA channel for ADC
static dma_channel_type* AdcDriver_GetDmaChannel(adc_type* adc_x) {
    if (adc_x == ADC1) {
        return DMA1_CHANNEL1;
    } else if (adc_x == ADC3) {
        return DMA2_CHANNEL5;
    }
    return NULL; // ADC2 doesn't have dedicated DMA
}

error_status AdcDriver_Init(const AdcConfig_t* config) {
    if (config == NULL || config->adc_peripheral == NULL) {
        return ERROR;
    }

    adc_type* adc_x = config->adc_peripheral;
    uint8_t adc_index = AdcDriver_GetIndex(adc_x);
    
    if (adc_index == 0xFF) {
        return ERROR;
    }

    // Store configuration
    adc_memcpy(&s_adc_configs[adc_index], config, sizeof(AdcConfig_t));

    // Enable peripheral clocks
    AdcDriver_EnableClock(adc_x);
    crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
    
    if (adc_x == ADC3) {
        crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);
    }

    // Configure ADC clock prescaler (via CRM)
    crm_adc_clock_div_set(config->clock_div);

    // Configure ADC in independent mode
    adc_combine_mode_select(ADC_INDEPENDENT_MODE);

    // Configure ADC peripheral
    adc_base_config_type adc_base_struct;
    adc_base_default_para_init(&adc_base_struct);
    
    // Set mode-specific configuration
    switch (config->mode) {
        case ADC_MODE_SINGLE:
            adc_base_struct.sequence_mode = FALSE;
            adc_base_struct.repeat_mode = FALSE;
            break;
        case ADC_MODE_CONTINUOUS:
            adc_base_struct.sequence_mode = FALSE;
            adc_base_struct.repeat_mode = TRUE;
            break;
        case ADC_MODE_SCAN:
        case ADC_MODE_DMA:
            adc_base_struct.sequence_mode = TRUE;
            adc_base_struct.repeat_mode = TRUE;
            break;
    }
    
    adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
    adc_base_struct.ordinary_channel_length = 1;
    adc_base_config(adc_x, &adc_base_struct);

    // Note: AT32F403A/407 ADC is fixed at 12-bit resolution
    // Resolution field is provided for compatibility but not applied

    // Enable temperature sensor if requested
    if (config->enable_temp_sensor) {
        adc_tempersensor_vintrv_enable(TRUE);
    }

    // Enable VREFINT if requested (same function as temperature sensor)
    if (config->enable_vrefint) {
        adc_tempersensor_vintrv_enable(TRUE);
    }

    // Enable ADC before calibration (critical for AT32F403A/407 - prevents lockup)
    adc_enable(adc_x, TRUE);
    
    // Small delay to ensure ADC is stable
    for (volatile uint32_t i = 0; i < 1000; i++);

    // Perform calibration (critical for accuracy)
    AdcDriver_Calibrate(adc_x);

    return SUCCESS;
}

void AdcDriver_ConfigurePin(gpio_type* gpio_x, uint32_t pin) {
    // Configure GPIO pin as analog input
    pinInit(gpio_x, pin, INPUT_ANALOG, GPIO_PULL_NONE);
}

void AdcDriver_Calibrate(adc_type* adc_x) {
    // Per AT32F403A/407 errata: ADC must be enabled before calibration
    // to prevent program lockup
    
    // Initialize calibration
    adc_calibration_init(adc_x);
    
    // Wait for calibration init to complete
    while (adc_calibration_init_status_get(adc_x));
    
    // Start calibration
    adc_calibration_start(adc_x);
    
    // Wait for calibration to complete
    while (adc_calibration_status_get(adc_x));
}

uint16_t AdcDriver_ReadChannel(adc_type* adc_x, AdcChannel_t channel, AdcSampleTime_t sample_time) {
    // Configure the channel
    adc_ordinary_channel_set(adc_x, channel, 1, sample_time);
    
    // Start conversion
    adc_ordinary_software_trigger_enable(adc_x, TRUE);
    
    // Wait for conversion to complete
    while (!adc_flag_get(adc_x, ADC_CCE_FLAG));
    
    // Clear flag
    adc_flag_clear(adc_x, ADC_CCE_FLAG);
    
    // Return conversion result
    return adc_ordinary_conversion_data_get(adc_x);
}

uint16_t AdcDriver_ReadChannelDefault(adc_type* adc_x, AdcChannel_t channel) {
    uint8_t adc_index = AdcDriver_GetIndex(adc_x);
    
    if (adc_index == 0xFF) {
        return 0;
    }
    
    AdcSampleTime_t default_time = s_adc_configs[adc_index].default_sample_time;
    return AdcDriver_ReadChannel(adc_x, channel, default_time);
}

void AdcDriver_StartConversion(adc_type* adc_x, AdcChannel_t channel, AdcSampleTime_t sample_time) {
    // Configure the channel
    adc_ordinary_channel_set(adc_x, channel, 1, sample_time);
    
    // Start conversion
    adc_ordinary_software_trigger_enable(adc_x, TRUE);
}

bool AdcDriver_IsConversionComplete(adc_type* adc_x) {
    return adc_flag_get(adc_x, ADC_CCE_FLAG) ? true : false;
}

uint16_t AdcDriver_GetConversionValue(adc_type* adc_x) {
    // Clear flag
    adc_flag_clear(adc_x, ADC_CCE_FLAG);
    
    // Return conversion result
    return adc_ordinary_conversion_data_get(adc_x);
}

error_status AdcDriver_InitMultiChannel(adc_type* adc_x, const AdcMultiChannel_t* multi_config) {
    if (multi_config == NULL || multi_config->num_channels == 0 || 
        multi_config->num_channels > 16 || multi_config->buffer == NULL) {
        return ERROR;
    }

    uint8_t adc_index = AdcDriver_GetIndex(adc_x);
    if (adc_index == 0xFF) {
        return ERROR;
    }

    // Store multi-channel configuration
    adc_memcpy(&s_multi_configs[adc_index], multi_config, sizeof(AdcMultiChannel_t));

    // Configure ADC for scan mode
    adc_base_config_type adc_base_struct;
    adc_base_default_para_init(&adc_base_struct);
    adc_base_struct.sequence_mode = TRUE;
    adc_base_struct.repeat_mode = TRUE;
    adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
    adc_base_struct.ordinary_channel_length = multi_config->num_channels;
    adc_base_config(adc_x, &adc_base_struct);

    // Configure channels
    AdcSampleTime_t default_time = s_adc_configs[adc_index].default_sample_time;
    for (uint8_t i = 0; i < multi_config->num_channels; i++) {
        adc_ordinary_channel_set(adc_x, 
                                multi_config->channels[i], 
                                i + 1, 
                                default_time);
    }

    // Configure DMA
    dma_channel_type* dma_channel = AdcDriver_GetDmaChannel(adc_x);
    if (dma_channel == NULL) {
        return ERROR;
    }

    // Reset DMA channel
    dma_reset(dma_channel);
    
    dma_init_type dma_init_struct;
    dma_default_para_init(&dma_init_struct);
    
    dma_init_struct.buffer_size = multi_config->num_channels;
    dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_base_addr = (uint32_t)multi_config->buffer;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
    dma_init_struct.memory_inc_enable = TRUE;
    dma_init_struct.peripheral_base_addr = (uint32_t)&(adc_x->odt);
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_init_struct.loop_mode_enable = TRUE;
    
    dma_init(dma_channel, &dma_init_struct);
    
    // Enable DMA channel
    dma_channel_enable(dma_channel, TRUE);
    
    // Enable ADC DMA mode
    adc_dma_mode_enable(adc_x, TRUE);

    return SUCCESS;
}

void AdcDriver_StartMultiChannel(adc_type* adc_x) {
    // Start continuous conversion
    adc_ordinary_software_trigger_enable(adc_x, TRUE);
}

void AdcDriver_StopMultiChannel(adc_type* adc_x) {
    // Stop continuous conversion
    adc_ordinary_software_trigger_enable(adc_x, FALSE);
    
    // Disable DMA
    dma_channel_type* dma_channel = AdcDriver_GetDmaChannel(adc_x);
    if (dma_channel != NULL) {
        dma_channel_enable(dma_channel, FALSE);
    }
    
    adc_dma_mode_enable(adc_x, FALSE);
}

int16_t AdcDriver_ReadTemperature(adc_type* adc_x) {
    // Read temperature sensor channel (channel 16)
    uint16_t adc_value = AdcDriver_ReadChannel(adc_x, ADC_CHANNEL_16, ADC_SAMPLE_TIME_239_5);
    
    // Convert to voltage (assuming 12-bit resolution and 3.3V reference)
    // V_SENSE = (adc_value / 4095) * 3.3V
    uint32_t v_sense_mv = (adc_value * 3300) / 4095;
    
    // Temperature calculation per AT32F403A/407 datasheet:
    // Temperature = ((V_SENSE - V_25) / Avg_Slope) + 25
    // V_25 ≈ 1430 mV (typical value at 25°C)
    // Avg_Slope ≈ 4.3 mV/°C
    
    const int32_t V_25 = 1430;      // mV
    const int32_t AVG_SLOPE = 43;   // mV/°C * 10 (4.3 mV/°C)
    
    // Calculate temperature in °C * 10 (e.g., 235 = 23.5°C)
    int32_t temp = ((v_sense_mv - V_25) * 100) / AVG_SLOPE + 250;
    
    return (int16_t)temp;
}

uint16_t AdcDriver_ReadVREFINT(adc_type* adc_x) {
    // Read VREFINT channel (channel 17)
    uint16_t adc_value = AdcDriver_ReadChannel(adc_x, ADC_CHANNEL_17, ADC_SAMPLE_TIME_239_5);
    
    // VREFINT is typically 1.2V (1200 mV)
    // Calculate actual VREFINT voltage
    // Assuming 12-bit resolution
    uint32_t vrefint_mv = (adc_value * 3300) / 4095;
    
    return (uint16_t)vrefint_mv;
}

uint32_t AdcDriver_ConvertToMillivolts(uint16_t adc_value, uint16_t vref_mv, uint8_t resolution) {
    uint32_t max_value;
    
    switch (resolution) {
        case 12:
            max_value = 4095;
            break;
        case 10:
            max_value = 1023;
            break;
        case 8:
            max_value = 255;
            break;
        case 6:
            max_value = 63;
            break;
        default:
            max_value = 4095;
            break;
    }
    
    return (adc_value * (uint32_t)vref_mv) / max_value;
}

void AdcDriver_DeInit(adc_type* adc_x) {
    // Disable ADC
    adc_enable(adc_x, FALSE);
    
    // Disable DMA if it was enabled
    dma_channel_type* dma_channel = AdcDriver_GetDmaChannel(adc_x);
    if (dma_channel != NULL) {
        dma_channel_enable(dma_channel, FALSE);
    }
    
    // Reset ADC peripheral
    adc_reset(adc_x);
    
    // Clear stored configuration
    uint8_t adc_index = AdcDriver_GetIndex(adc_x);
    if (adc_index != 0xFF) {
        adc_memset(&s_adc_configs[adc_index], 0, sizeof(AdcConfig_t));
        adc_memset(&s_multi_configs[adc_index], 0, sizeof(AdcMultiChannel_t));
    }
}
