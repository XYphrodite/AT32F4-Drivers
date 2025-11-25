# ADC Driver Usage Examples

## Overview
The ADC Driver provides a comprehensive interface for analog-to-digital conversion on the AT32F403A/407 microcontroller family. It supports single-channel, multi-channel, DMA-based continuous conversion, temperature sensing, and internal voltage reference reading.

## Table of Contents
1. [Basic Single-Channel Reading](#basic-single-channel-reading)
2. [Continuous Single-Channel Reading](#continuous-single-channel-reading)
3. [Multi-Channel Scan with DMA](#multi-channel-scan-with-dma)
4. [Temperature Sensor Reading](#temperature-sensor-reading)
5. [Internal Voltage Reference (VREFINT)](#internal-voltage-reference-vrefint)
6. [Non-Blocking ADC Conversion](#non-blocking-adc-conversion)
7. [Complete Universal Input Example](#complete-universal-input-example)

---

## Basic Single-Channel Reading

Read a single ADC channel (e.g., a potentiometer on PA0).

```cpp
#include "AdcDriver.h"
#include "SystemApi.h"

void example_single_channel(void) {
    // Initialize system
    SystemApi::Init(NVIC_PRIORITY_GROUP_4, _240Mhz, true, 0);
    
    // Configure ADC1
    AdcConfig_t adc_config = {
        .adc_peripheral = ADC1,
        .mode = ADC_MODE_SINGLE,
        .resolution = ADC_RESOLUTION_12BIT,
        .clock_div = ADC_CLOCK_DIV_6,          // 240MHz / 6 = 40MHz / 2.5 = 16MHz ADC clock
        .default_sample_time = ADC_SAMPLE_TIME_55_5,
        .enable_dma = false,
        .enable_temp_sensor = false,
        .enable_vrefint = false
    };
    
    AdcDriver_Init(&adc_config);
    
    // Configure PA0 as analog input for ADC1_IN0
    AdcDriver_ConfigurePin(GPIOA, GPIO_PINS_0);
    
    // Read ADC channel 0
    uint16_t adc_value = AdcDriver_ReadChannelDefault(ADC1, ADC_CHANNEL_0);
    
    // Convert to millivolts (assuming 3.3V reference)
    uint32_t voltage_mv = AdcDriver_ConvertToMillivolts(adc_value, 3300, 12);
    
    // Use the value
    printf("ADC Value: %u, Voltage: %lu mV\n", adc_value, voltage_mv);
}
```

---

## Continuous Single-Channel Reading

Read a single channel repeatedly in a loop.

```cpp
#include "AdcDriver.h"
#include "FreeRTOS.h"
#include "task.h"

void adc_task(void *pvParameters) {
    // Configure ADC for continuous mode
    AdcConfig_t adc_config = {
        .adc_peripheral = ADC1,
        .mode = ADC_MODE_CONTINUOUS,
        .resolution = ADC_RESOLUTION_12BIT,
        .clock_div = ADC_CLOCK_DIV_6,
        .default_sample_time = ADC_SAMPLE_TIME_55_5,
        .enable_dma = false,
        .enable_temp_sensor = false,
        .enable_vrefint = false
    };
    
    AdcDriver_Init(&adc_config);
    AdcDriver_ConfigurePin(GPIOA, GPIO_PINS_1);  // PA1 = ADC1_IN1
    
    while (1) {
        // Read channel 1
        uint16_t adc_value = AdcDriver_ReadChannelDefault(ADC1, ADC_CHANNEL_1);
        uint32_t voltage_mv = AdcDriver_ConvertToMillivolts(adc_value, 3300, 12);
        
        // Process the reading
        process_analog_input(voltage_mv);
        
        // Delay between readings
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
```

---

## Multi-Channel Scan with DMA

Read multiple ADC channels continuously using DMA for automatic data transfer.

```cpp
#include "AdcDriver.h"

// Global buffer for DMA results
uint16_t adc_dma_buffer[4];

void example_multi_channel_dma(void) {
    // Configure ADC for DMA mode
    AdcConfig_t adc_config = {
        .adc_peripheral = ADC1,
        .mode = ADC_MODE_DMA,
        .resolution = ADC_RESOLUTION_12BIT,
        .clock_div = ADC_CLOCK_DIV_6,
        .default_sample_time = ADC_SAMPLE_TIME_55_5,
        .enable_dma = true,
        .enable_temp_sensor = false,
        .enable_vrefint = false
    };
    
    AdcDriver_Init(&adc_config);
    
    // Configure GPIO pins
    AdcDriver_ConfigurePin(GPIOA, GPIO_PINS_0);  // ADC1_IN0
    AdcDriver_ConfigurePin(GPIOA, GPIO_PINS_1);  // ADC1_IN1
    AdcDriver_ConfigurePin(GPIOA, GPIO_PINS_2);  // ADC1_IN2
    AdcDriver_ConfigurePin(GPIOA, GPIO_PINS_3);  // ADC1_IN3
    
    // Configure multi-channel scan
    AdcMultiChannel_t multi_config = {
        .channels = {ADC_CHANNEL_0, ADC_CHANNEL_1, ADC_CHANNEL_2, ADC_CHANNEL_3},
        .num_channels = 4,
        .buffer = adc_dma_buffer,
        .buffer_size = 4
    };
    
    AdcDriver_InitMultiChannel(ADC1, &multi_config);
    
    // Start continuous conversion
    AdcDriver_StartMultiChannel(ADC1);
    
    // Results are now continuously updated in adc_dma_buffer
    while (1) {
        // Read results directly from buffer (no polling needed!)
        uint32_t ch0_mv = AdcDriver_ConvertToMillivolts(adc_dma_buffer[0], 3300, 12);
        uint32_t ch1_mv = AdcDriver_ConvertToMillivolts(adc_dma_buffer[1], 3300, 12);
        uint32_t ch2_mv = AdcDriver_ConvertToMillivolts(adc_dma_buffer[2], 3300, 12);
        uint32_t ch3_mv = AdcDriver_ConvertToMillivolts(adc_dma_buffer[3], 3300, 12);
        
        // Process readings
        printf("CH0: %lu mV, CH1: %lu mV, CH2: %lu mV, CH3: %lu mV\n", 
               ch0_mv, ch1_mv, ch2_mv, ch3_mv);
        
        delay_ms(1000);
    }
}
```

---

## Temperature Sensor Reading

Read the internal temperature sensor.

```cpp
#include "AdcDriver.h"

void example_temperature_sensor(void) {
    // Configure ADC with temperature sensor enabled
    AdcConfig_t adc_config = {
        .adc_peripheral = ADC1,
        .mode = ADC_MODE_SINGLE,
        .resolution = ADC_RESOLUTION_12BIT,
        .clock_div = ADC_CLOCK_DIV_6,
        .default_sample_time = ADC_SAMPLE_TIME_239_5,  // Longer sampling time for accuracy
        .enable_dma = false,
        .enable_temp_sensor = true,   // Enable temperature sensor
        .enable_vrefint = false
    };
    
    AdcDriver_Init(&adc_config);
    
    // Read temperature (returns value * 10, e.g., 235 = 23.5°C)
    int16_t temp_x10 = AdcDriver_ReadTemperature(ADC1);
    
    // Display temperature
    printf("MCU Temperature: %d.%d C\n", temp_x10 / 10, abs(temp_x10 % 10));
}
```

---

## Internal Voltage Reference (VREFINT)

Read the internal voltage reference to calculate actual VDD voltage.

```cpp
#include "AdcDriver.h"

void example_vrefint(void) {
    // Configure ADC with VREFINT enabled
    AdcConfig_t adc_config = {
        .adc_peripheral = ADC1,
        .mode = ADC_MODE_SINGLE,
        .resolution = ADC_RESOLUTION_12BIT,
        .clock_div = ADC_CLOCK_DIV_6,
        .default_sample_time = ADC_SAMPLE_TIME_239_5,
        .enable_dma = false,
        .enable_temp_sensor = false,
        .enable_vrefint = true    // Enable VREFINT
    };
    
    AdcDriver_Init(&adc_config);
    
    // Read VREFINT (typically ~1200 mV)
    uint16_t vrefint_mv = AdcDriver_ReadVREFINT(ADC1);
    
    // Calculate actual VDD voltage
    // VREFINT is a stable 1.2V reference
    // VDD = (1200 * 4095) / ADC_reading
    uint32_t vdd_mv = (1200UL * 4095UL) / vrefint_mv;
    
    printf("VREFINT: %u mV, Calculated VDD: %lu mV\n", vrefint_mv, vdd_mv);
}
```

---

## Non-Blocking ADC Conversion

Start conversion and check completion asynchronously.

```cpp
#include "AdcDriver.h"

typedef enum {
    ADC_STATE_IDLE,
    ADC_STATE_CONVERTING,
    ADC_STATE_COMPLETE
} AdcState_t;

AdcState_t adc_state = ADC_STATE_IDLE;
uint16_t adc_result = 0;

void adc_start_reading(void) {
    if (adc_state == ADC_STATE_IDLE) {
        // Start non-blocking conversion
        AdcDriver_StartConversion(ADC1, ADC_CHANNEL_0, ADC_SAMPLE_TIME_55_5);
        adc_state = ADC_STATE_CONVERTING;
    }
}

void adc_check_completion(void) {
    if (adc_state == ADC_STATE_CONVERTING) {
        if (AdcDriver_IsConversionComplete(ADC1)) {
            // Read result
            adc_result = AdcDriver_GetConversionValue(ADC1);
            adc_state = ADC_STATE_COMPLETE;
        }
    }
}

void main_loop(void) {
    // Initialize ADC
    AdcConfig_t adc_config = {
        .adc_peripheral = ADC1,
        .mode = ADC_MODE_SINGLE,
        .resolution = ADC_RESOLUTION_12BIT,
        .clock_div = ADC_CLOCK_DIV_6,
        .default_sample_time = ADC_SAMPLE_TIME_55_5,
        .enable_dma = false,
        .enable_temp_sensor = false,
        .enable_vrefint = false
    };
    AdcDriver_Init(&adc_config);
    AdcDriver_ConfigurePin(GPIOA, GPIO_PINS_0);
    
    while (1) {
        // State machine approach
        switch (adc_state) {
            case ADC_STATE_IDLE:
                adc_start_reading();
                break;
                
            case ADC_STATE_CONVERTING:
                adc_check_completion();
                break;
                
            case ADC_STATE_COMPLETE:
                // Process result
                uint32_t voltage_mv = AdcDriver_ConvertToMillivolts(adc_result, 3300, 12);
                printf("Voltage: %lu mV\n", voltage_mv);
                
                // Reset state for next reading
                adc_state = ADC_STATE_IDLE;
                delay_ms(100);
                break;
        }
        
        // Do other work here without blocking
        do_other_tasks();
    }
}
```

---

## Complete Universal Input Example

Complete example for P-Block universal input system (11 configurable inputs).

```cpp
#include "AdcDriver.h"
#include "FreeRTOS.h"
#include "task.h"

// Universal input configuration
typedef enum {
    UNIVERSAL_INPUT_ANALOG,      // 0-10V analog input
    UNIVERSAL_INPUT_DIGITAL,     // Digital input
    UNIVERSAL_INPUT_TEMP_DOL12,  // Temperature sensor Dol12
    UNIVERSAL_INPUT_TEMP_TAFCO   // Temperature sensor Tafco
} UniversalInputMode_t;

typedef struct {
    UniversalInputMode_t mode;
    AdcChannel_t adc_channel;
    gpio_type* gpio_port;
    uint32_t gpio_pin;
    uint16_t analog_value;       // Raw ADC value
    bool digital_value;          // Digital state
} UniversalInput_t;

// 11 universal inputs for P-Block
UniversalInput_t universal_inputs[11];
uint16_t adc_dma_buffer[11];

void init_universal_inputs(void) {
    // Configure ADC for multi-channel DMA
    AdcConfig_t adc_config = {
        .adc_peripheral = ADC1,
        .mode = ADC_MODE_DMA,
        .resolution = ADC_RESOLUTION_12BIT,
        .clock_div = ADC_CLOCK_DIV_6,
        .default_sample_time = ADC_SAMPLE_TIME_55_5,
        .enable_dma = true,
        .enable_temp_sensor = false,
        .enable_vrefint = false
    };
    AdcDriver_Init(&adc_config);
    
    // Initialize universal inputs
    for (uint8_t i = 0; i < 11; i++) {
        universal_inputs[i].mode = UNIVERSAL_INPUT_ANALOG;
        universal_inputs[i].adc_channel = (AdcChannel_t)i;
        universal_inputs[i].gpio_port = GPIOA;
        universal_inputs[i].gpio_pin = GPIO_PINS_0 << i;
        universal_inputs[i].analog_value = 0;
        universal_inputs[i].digital_value = false;
        
        // Configure GPIO pin
        AdcDriver_ConfigurePin(universal_inputs[i].gpio_port, 
                              universal_inputs[i].gpio_pin);
    }
    
    // Configure multi-channel scan
    AdcMultiChannel_t multi_config = {
        .channels = {
            ADC_CHANNEL_0, ADC_CHANNEL_1, ADC_CHANNEL_2, ADC_CHANNEL_3,
            ADC_CHANNEL_4, ADC_CHANNEL_5, ADC_CHANNEL_6, ADC_CHANNEL_7,
            ADC_CHANNEL_8, ADC_CHANNEL_9, ADC_CHANNEL_10
        },
        .num_channels = 11,
        .buffer = adc_dma_buffer,
        .buffer_size = 11
    };
    
    AdcDriver_InitMultiChannel(ADC1, &multi_config);
    AdcDriver_StartMultiChannel(ADC1);
}

void universal_input_task(void *pvParameters) {
    init_universal_inputs();
    
    while (1) {
        // Update all universal inputs
        for (uint8_t i = 0; i < 11; i++) {
            // Get value from DMA buffer
            universal_inputs[i].analog_value = adc_dma_buffer[i];
            
            // Process based on mode
            switch (universal_inputs[i].mode) {
                case UNIVERSAL_INPUT_ANALOG: {
                    // Convert to millivolts (0-10000 mV range)
                    uint32_t mv = AdcDriver_ConvertToMillivolts(
                        universal_inputs[i].analog_value, 10000, 12
                    );
                    // Store in P-Block registers
                    update_universal_input_register(i, mv);
                    break;
                }
                
                case UNIVERSAL_INPUT_DIGITAL: {
                    // Digital threshold at 2.5V
                    uint32_t mv = AdcDriver_ConvertToMillivolts(
                        universal_inputs[i].analog_value, 10000, 12
                    );
                    universal_inputs[i].digital_value = (mv > 2500);
                    // Update discrete input registers
                    update_discrete_input_register(i, universal_inputs[i].digital_value);
                    break;
                }
                
                case UNIVERSAL_INPUT_TEMP_DOL12:
                case UNIVERSAL_INPUT_TEMP_TAFCO: {
                    // Temperature sensor processing
                    int16_t temp = process_temperature_sensor(
                        universal_inputs[i].analog_value,
                        universal_inputs[i].mode
                    );
                    update_temperature_register(i, temp);
                    break;
                }
            }
        }
        
        // Update every 100ms
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Helper functions (implement based on your system)
void update_universal_input_register(uint8_t channel, uint32_t value_mv) {
    // Write to Modbus input registers (30001-30011)
}

void update_discrete_input_register(uint8_t channel, bool state) {
    // Write to Modbus discrete inputs
}

int16_t process_temperature_sensor(uint16_t adc_value, UniversalInputMode_t sensor_type) {
    // Implement temperature sensor conversion based on sensor type
    // Return temperature in °C * 10
    return 0;
}

void update_temperature_register(uint8_t channel, int16_t temp) {
    // Write temperature to appropriate register
}
```

---

## Pin Mapping Reference

### ADC1 Channels (AT32F403A/407)
| Channel | Pin   | Description |
|---------|-------|-------------|
| 0       | PA0   | ADC1_IN0    |
| 1       | PA1   | ADC1_IN1    |
| 2       | PA2   | ADC1_IN2    |
| 3       | PA3   | ADC1_IN3    |
| 4       | PA4   | ADC1_IN4    |
| 5       | PA5   | ADC1_IN5    |
| 6       | PA6   | ADC1_IN6    |
| 7       | PA7   | ADC1_IN7    |
| 8       | PB0   | ADC1_IN8    |
| 9       | PB1   | ADC1_IN9    |
| 10      | PC0   | ADC1_IN10   |
| 11      | PC1   | ADC1_IN11   |
| 12      | PC2   | ADC1_IN12   |
| 13      | PC3   | ADC1_IN13   |
| 14      | PC4   | ADC1_IN14   |
| 15      | PC5   | ADC1_IN15   |
| 16      | -     | Temperature |
| 17      | -     | VREFINT     |

---

## Important Notes

### AT32F403A/407 ADC Errata (from Context7 docs)

1. **Calibration Lockup Prevention**: Always enable ADC before calibration
   ```cpp
   adc_enable(ADC1, TRUE);  // Enable FIRST
   AdcDriver_Calibrate(ADC1);  // Then calibrate
   ```

2. **Dual ADC Mode**: Enable both ADCs before calibrating either one
   ```cpp
   adc_enable(ADC1, TRUE);
   adc_enable(ADC2, TRUE);
   AdcDriver_Calibrate(ADC1);
   AdcDriver_Calibrate(ADC2);
   ```

3. **Maximum ADC Clock**: 14 MHz (per datasheet)
   - At 240 MHz system clock, use ADC_CLOCK_DIV_6 or higher

4. **Sampling Time**: Longer sampling times provide better accuracy for high-impedance sources
   - Use ADC_SAMPLE_TIME_239_5 for temperature sensor and VREFINT
   - Use ADC_SAMPLE_TIME_55_5 or higher for external analog inputs

### DMA Limitations

- ADC1: Uses DMA1 Channel 1
- ADC2: No dedicated DMA channel
- ADC3: Uses DMA2 Channel 5

### FreeRTOS Integration

When using with FreeRTOS, ensure proper task priorities and delays to avoid ADC overload.

---

## Troubleshooting

### ADC reads always return 0 or 4095
- Check GPIO pin is configured as analog input
- Verify ADC clock is within specification (≤14 MHz)
- Ensure calibration was performed

### DMA buffer not updating
- Verify DMA clock is enabled
- Check buffer address is valid
- Ensure ADC DMA mode is enabled

### Inaccurate readings
- Increase sampling time for high-impedance sources
- Perform calibration after power-up
- Check reference voltage is stable

---

## License
This driver follows the same license as TafcoPigstore project (MIT License with attribution to Artery Technology).

