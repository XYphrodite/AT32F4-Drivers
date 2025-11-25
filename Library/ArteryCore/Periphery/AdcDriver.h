#ifndef _ADC_DRIVER_H_
#define _ADC_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "at32f403a_407.h"
#include "at32f403a_407_adc.h"
#include "at32f403a_407_crm.h"
#include "at32f403a_407_dma.h"
#include "at32f403a_407_gpio.h"

/**
 * @brief ADC operating modes
 */
typedef enum {
    ADC_MODE_SINGLE,           ///< Single conversion mode
    ADC_MODE_CONTINUOUS,       ///< Continuous conversion mode
    ADC_MODE_SCAN,             ///< Scan mode for multiple channels
    ADC_MODE_DMA               ///< DMA mode for continuous data transfer
} AdcMode_t;

/**
 * @brief ADC channel selection (0-17)
 * Channels 0-15: External pins
 * Channel 16: Temperature sensor
 * Channel 17: Internal reference voltage (VREFINT)
 * Uses AT32 HAL adc_channel_select_type directly
 */
typedef adc_channel_select_type AdcChannel_t;

/**
 * @brief ADC sampling time configuration
 * Longer sampling times provide better accuracy for high-impedance sources
 * Uses AT32 HAL adc_sampletime_select_type directly
 */
typedef adc_sampletime_select_type AdcSampleTime_t;

#define ADC_SAMPLE_TIME_1_5    ADC_SAMPLETIME_1_5    ///< 1.5 cycles
#define ADC_SAMPLE_TIME_7_5    ADC_SAMPLETIME_7_5    ///< 7.5 cycles
#define ADC_SAMPLE_TIME_13_5   ADC_SAMPLETIME_13_5   ///< 13.5 cycles
#define ADC_SAMPLE_TIME_28_5   ADC_SAMPLETIME_28_5   ///< 28.5 cycles
#define ADC_SAMPLE_TIME_41_5   ADC_SAMPLETIME_41_5   ///< 41.5 cycles
#define ADC_SAMPLE_TIME_55_5   ADC_SAMPLETIME_55_5   ///< 55.5 cycles
#define ADC_SAMPLE_TIME_71_5   ADC_SAMPLETIME_71_5   ///< 71.5 cycles
#define ADC_SAMPLE_TIME_239_5  ADC_SAMPLETIME_239_5  ///< 239.5 cycles (highest accuracy)

/**
 * @brief ADC resolution configuration
 * Note: AT32F403A/407 only supports 12-bit resolution
 */
typedef enum {
    ADC_RESOLUTION_12BIT = 12,   ///< 12-bit resolution (0-4095) - AT32F403A/407 only
    ADC_RESOLUTION_10BIT = 10,   ///< 10-bit resolution (0-1023) - for compatibility
    ADC_RESOLUTION_8BIT  = 8,    ///< 8-bit resolution (0-255) - for compatibility
    ADC_RESOLUTION_6BIT  = 6     ///< 6-bit resolution (0-63) - for compatibility
} AdcResolution_t;

/**
 * @brief ADC clock prescaler
 * ADC clock = PCLK2 / prescaler
 * Max ADC clock: 14 MHz (per AT32F403A/407 datasheet)
 * Uses AT32 HAL crm_adc_div_type directly
 */
typedef crm_adc_div_type AdcClockDiv_t;

#define ADC_CLOCK_DIV_2   CRM_ADC_DIV_2    ///< PCLK2 / 2
#define ADC_CLOCK_DIV_4   CRM_ADC_DIV_4    ///< PCLK2 / 4
#define ADC_CLOCK_DIV_6   CRM_ADC_DIV_6    ///< PCLK2 / 6
#define ADC_CLOCK_DIV_8   CRM_ADC_DIV_8    ///< PCLK2 / 8
#define ADC_CLOCK_DIV_12  CRM_ADC_DIV_12   ///< PCLK2 / 12
#define ADC_CLOCK_DIV_16  CRM_ADC_DIV_16   ///< PCLK2 / 16

/**
 * @brief ADC configuration structure
 */
typedef struct {
    adc_type*        adc_peripheral;      ///< ADC peripheral (ADC1, ADC2, ADC3)
    AdcMode_t        mode;                ///< Operating mode
    AdcResolution_t  resolution;          ///< ADC resolution
    AdcClockDiv_t    clock_div;           ///< Clock prescaler
    AdcSampleTime_t  default_sample_time; ///< Default sampling time for all channels
    bool             enable_dma;          ///< Enable DMA transfer
    bool             enable_temp_sensor;  ///< Enable internal temperature sensor
    bool             enable_vrefint;      ///< Enable internal voltage reference
} AdcConfig_t;

/**
 * @brief Multi-channel ADC configuration
 */
typedef struct {
    AdcChannel_t    channels[16];         ///< Array of channels to scan
    uint8_t         num_channels;         ///< Number of channels (1-16)
    uint16_t*       buffer;               ///< DMA buffer for results (if DMA enabled)
    uint16_t        buffer_size;          ///< Size of DMA buffer
} AdcMultiChannel_t;

/**
 * @brief Initialize ADC peripheral with configuration
 * 
 * @param config Pointer to ADC configuration structure
 * @return error_status SUCCESS if initialization successful, ERROR otherwise
 * 
 * @note This function:
 *       1. Enables ADC and GPIO clocks
 *       2. Configures ADC clock prescaler
 *       3. Sets resolution and operating mode
 *       4. Performs ADC calibration (critical for AT32F403A/407)
 *       5. Enables temperature sensor/VREFINT if requested
 * 
 * @warning Always call this function before using any ADC operations
 * 
 * Example:
 * @code
 *   AdcConfig_t adc_config = {
 *       .adc_peripheral = ADC1,
 *       .mode = ADC_MODE_SINGLE,
 *       .resolution = ADC_RESOLUTION_12BIT,
 *       .clock_div = ADC_CLOCK_DIV_6,
 *       .default_sample_time = ADC_SAMPLE_TIME_55_5,
 *       .enable_dma = false,
 *       .enable_temp_sensor = false,
 *       .enable_vrefint = false
 *   };
 *   AdcDriver_Init(&adc_config);
 * @endcode
 */
error_status AdcDriver_Init(const AdcConfig_t* config);

/**
 * @brief Configure GPIO pin for analog input
 * 
 * @param gpio_x GPIO port (GPIOA, GPIOB, etc.)
 * @param pin GPIO pin number (GPIO_PINS_0, GPIO_PINS_1, etc.)
 * 
 * @note This function configures the specified GPIO pin as analog input
 *       Call this for each external ADC channel before using it
 * 
 * Example:
 * @code
 *   // Configure PA0 as analog input for ADC1_IN0
 *   AdcDriver_ConfigurePin(GPIOA, GPIO_PINS_0);
 * @endcode
 */
void AdcDriver_ConfigurePin(gpio_type* gpio_x, uint32_t pin);

/**
 * @brief Perform ADC calibration
 * 
 * @param adc_x ADC peripheral (ADC1, ADC2, ADC3)
 * 
 * @note Calibration is critical for AT32F403A/407 ADC accuracy
 *       Per AT32 errata: Must enable ADC before calibration to prevent lockup
 *       This function is called automatically by AdcDriver_Init()
 * 
 * @warning In dual ADC mode, enable both ADCs before calibration
 */
void AdcDriver_Calibrate(adc_type* adc_x);

/**
 * @brief Read single ADC channel (blocking)
 * 
 * @param adc_x ADC peripheral (ADC1, ADC2, ADC3)
 * @param channel ADC channel to read (ADC_CHANNEL_0 to ADC_CHANNEL_17)
 * @param sample_time Sampling time for this conversion
 * @return uint16_t ADC conversion result (0-4095 for 12-bit resolution)
 * 
 * @note This function:
 *       1. Configures the specified channel
 *       2. Starts conversion
 *       3. Waits for completion
 *       4. Returns the result
 * 
 * Example:
 * @code
 *   // Read ADC1 channel 0 with 55.5 cycle sampling time
 *   uint16_t adc_value = AdcDriver_ReadChannel(ADC1, ADC_CHANNEL_0, ADC_SAMPLE_TIME_55_5);
 *   // Convert to millivolts (assuming 3.3V reference)
 *   uint32_t millivolts = (adc_value * 3300) / 4095;
 * @endcode
 */
uint16_t AdcDriver_ReadChannel(adc_type* adc_x, AdcChannel_t channel, AdcSampleTime_t sample_time);

/**
 * @brief Read single ADC channel with default sampling time (blocking)
 * 
 * @param adc_x ADC peripheral (ADC1, ADC2, ADC3)
 * @param channel ADC channel to read
 * @return uint16_t ADC conversion result
 * 
 * @note Uses default sampling time configured during initialization
 */
uint16_t AdcDriver_ReadChannelDefault(adc_type* adc_x, AdcChannel_t channel);

/**
 * @brief Start ADC conversion (non-blocking)
 * 
 * @param adc_x ADC peripheral (ADC1, ADC2, ADC3)
 * @param channel ADC channel to convert
 * @param sample_time Sampling time for this conversion
 * 
 * @note Use AdcDriver_IsConversionComplete() to check completion
 *       Use AdcDriver_GetConversionValue() to read result
 */
void AdcDriver_StartConversion(adc_type* adc_x, AdcChannel_t channel, AdcSampleTime_t sample_time);

/**
 * @brief Check if ADC conversion is complete
 * 
 * @param adc_x ADC peripheral (ADC1, ADC2, ADC3)
 * @return true if conversion complete, false otherwise
 */
bool AdcDriver_IsConversionComplete(adc_type* adc_x);

/**
 * @brief Get ADC conversion result
 * 
 * @param adc_x ADC peripheral (ADC1, ADC2, ADC3)
 * @return uint16_t ADC conversion result
 * 
 * @warning Call this only after AdcDriver_IsConversionComplete() returns true
 */
uint16_t AdcDriver_GetConversionValue(adc_type* adc_x);

/**
 * @brief Initialize multi-channel scan mode with DMA
 * 
 * @param adc_x ADC peripheral (ADC1, ADC2, ADC3)
 * @param multi_config Multi-channel configuration
 * @return error_status SUCCESS if initialization successful, ERROR otherwise
 * 
 * @note This function sets up ADC in scan mode with DMA for automatic data transfer
 *       DMA continuously fills the buffer with conversion results
 * 
 * Example:
 * @code
 *   uint16_t adc_buffer[3];
 *   AdcMultiChannel_t multi_config = {
 *       .channels = {ADC_CHANNEL_0, ADC_CHANNEL_1, ADC_CHANNEL_2},
 *       .num_channels = 3,
 *       .buffer = adc_buffer,
 *       .buffer_size = 3
 *   };
 *   AdcDriver_InitMultiChannel(ADC1, &multi_config);
 *   AdcDriver_StartMultiChannel(ADC1);
 *   // Results continuously updated in adc_buffer
 * @endcode
 */
error_status AdcDriver_InitMultiChannel(adc_type* adc_x, const AdcMultiChannel_t* multi_config);

/**
 * @brief Start multi-channel continuous conversion with DMA
 * 
 * @param adc_x ADC peripheral (ADC1, ADC2, ADC3)
 * 
 * @note Call AdcDriver_InitMultiChannel() first
 *       Conversions continue until AdcDriver_StopMultiChannel() is called
 */
void AdcDriver_StartMultiChannel(adc_type* adc_x);

/**
 * @brief Stop multi-channel continuous conversion
 * 
 * @param adc_x ADC peripheral (ADC1, ADC2, ADC3)
 */
void AdcDriver_StopMultiChannel(adc_type* adc_x);

/**
 * @brief Read internal temperature sensor
 * 
 * @param adc_x ADC peripheral (ADC1, ADC2, ADC3)
 * @return int16_t Temperature in degrees Celsius * 10 (e.g., 235 = 23.5°C)
 * 
 * @note Temperature sensor must be enabled in AdcConfig_t during initialization
 *       Formula: Temperature = ((V_SENSE - V_25) / Avg_Slope) + 25
 *       V_25 ≈ 1.43V (typical), Avg_Slope ≈ 4.3 mV/°C (per AT32F403A/407 datasheet)
 * 
 * Example:
 * @code
 *   int16_t temp = AdcDriver_ReadTemperature(ADC1);
 *   // temp = 235 means 23.5°C
 *   printf("Temperature: %d.%d C\n", temp / 10, temp % 10);
 * @endcode
 */
int16_t AdcDriver_ReadTemperature(adc_type* adc_x);

/**
 * @brief Read internal voltage reference (VREFINT)
 * 
 * @param adc_x ADC peripheral (ADC1, ADC2, ADC3)
 * @return uint16_t VREFINT voltage in millivolts (typically ~1200 mV)
 * 
 * @note VREFINT must be enabled in AdcConfig_t during initialization
 *       VREFINT is typically 1.2V and can be used to calculate VDD voltage
 * 
 * Example:
 * @code
 *   uint16_t vrefint = AdcDriver_ReadVREFINT(ADC1);
 *   // Calculate actual VDD voltage
 *   uint32_t vdd = (1200 * 4095) / vrefint; // in millivolts
 * @endcode
 */
uint16_t AdcDriver_ReadVREFINT(adc_type* adc_x);

/**
 * @brief Convert ADC value to millivolts
 * 
 * @param adc_value Raw ADC value (0-4095 for 12-bit)
 * @param vref_mv Reference voltage in millivolts (typically 3300)
 * @param resolution ADC resolution (12, 10, 8, or 6 bits)
 * @return uint32_t Voltage in millivolts
 * 
 * Example:
 * @code
 *   uint16_t adc_value = AdcDriver_ReadChannel(ADC1, ADC_CHANNEL_0, ADC_SAMPLE_TIME_55_5);
 *   uint32_t voltage_mv = AdcDriver_ConvertToMillivolts(adc_value, 3300, 12);
 * @endcode
 */
uint32_t AdcDriver_ConvertToMillivolts(uint16_t adc_value, uint16_t vref_mv, uint8_t resolution);

/**
 * @brief De-initialize ADC peripheral
 * 
 * @param adc_x ADC peripheral (ADC1, ADC2, ADC3)
 * 
 * @note Disables ADC and releases resources
 */
void AdcDriver_DeInit(adc_type* adc_x);

#ifdef __cplusplus
}
#endif

#endif // _ADC_DRIVER_H_

