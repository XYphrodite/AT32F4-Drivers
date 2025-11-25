# ADC Driver for AT32F403A/407

## Overview
Comprehensive ADC (Analog-to-Digital Converter) driver for the AT32F403A/407 ARM Cortex-M4 microcontroller family. This driver provides a high-level interface for analog measurements, supporting single-channel, multi-channel DMA-based conversion, temperature sensing, and internal voltage reference.

## Features
- ✅ **Single-channel conversion** (blocking and non-blocking)
- ✅ **Multi-channel scan mode** with DMA for automatic data transfer
- ✅ **12-bit, 10-bit, 8-bit, and 6-bit resolution** support
- ✅ **Configurable sampling time** (1.5 to 239.5 cycles)
- ✅ **Internal temperature sensor** reading
- ✅ **Internal voltage reference (VREFINT)** for VDD calculation
- ✅ **Automatic ADC calibration** (with AT32 errata workarounds)
- ✅ **DMA support** for continuous multi-channel conversion
- ✅ **Helper functions** for voltage conversion
- ✅ **Comprehensive documentation** and examples

## AT32F403A/407 Errata Compliance
This driver implements critical workarounds from the AT32F403A/407 Errata Sheet (ES0002 v2.0.11):

### ADC Errata #2.1.1: Dual ADC Mode Calibration Lockup
**Issue**: Program gets stuck during ADC calibration when using dual ADC mode.

**Workaround Implemented**: Always enable ADC before calibration
```cpp
// Per AT32 errata: Enable ADC BEFORE calibration
adc_enable(adc_x, TRUE);
for (volatile uint32_t i = 0; i < 1000; i++);  // Stabilization delay
AdcDriver_Calibrate(adc_x);  // Now safe to calibrate
```

**Reference**: ES0002_AT32F403A_407_Errata_Sheet_EN_V2.0.11.md, Section 2.1.1

## File Structure
```
TafcoMcuCore/Library/ArteryCore/Periphery/
├── AdcDriver.h              # Header file with API declarations
├── AdcDriver.cpp            # Implementation file
├── AdcDriver_Examples.md    # Comprehensive usage examples
└── AdcDriver_README.md      # This file
```

## Quick Start

### 1. Basic Single-Channel Reading
```cpp
#include "AdcDriver.h"

// Configure ADC
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

// Configure PA0 as analog input
AdcDriver_ConfigurePin(GPIOA, GPIO_PINS_0);

// Read channel 0
uint16_t adc_value = AdcDriver_ReadChannelDefault(ADC1, ADC_CHANNEL_0);
uint32_t voltage_mv = AdcDriver_ConvertToMillivolts(adc_value, 3300, 12);
```

### 2. Multi-Channel DMA Reading
```cpp
// Global DMA buffer
uint16_t adc_dma_buffer[4];

// Configure ADC for DMA
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
AdcDriver_ConfigurePin(GPIOA, GPIO_PINS_0);
AdcDriver_ConfigurePin(GPIOA, GPIO_PINS_1);
AdcDriver_ConfigurePin(GPIOA, GPIO_PINS_2);
AdcDriver_ConfigurePin(GPIOA, GPIO_PINS_3);

// Setup multi-channel scan
AdcMultiChannel_t multi_config = {
    .channels = {ADC_CHANNEL_0, ADC_CHANNEL_1, ADC_CHANNEL_2, ADC_CHANNEL_3},
    .num_channels = 4,
    .buffer = adc_dma_buffer,
    .buffer_size = 4
};

AdcDriver_InitMultiChannel(ADC1, &multi_config);
AdcDriver_StartMultiChannel(ADC1);

// Results continuously updated in adc_dma_buffer
// Access directly: adc_dma_buffer[0], adc_dma_buffer[1], etc.
```

### 3. Temperature Sensor Reading
```cpp
// Configure with temperature sensor enabled
AdcConfig_t adc_config = {
    .adc_peripheral = ADC1,
    .mode = ADC_MODE_SINGLE,
    .resolution = ADC_RESOLUTION_12BIT,
    .clock_div = ADC_CLOCK_DIV_6,
    .default_sample_time = ADC_SAMPLE_TIME_239_5,  // Longer for accuracy
    .enable_dma = false,
    .enable_temp_sensor = true,  // Enable temperature sensor
    .enable_vrefint = false
};

AdcDriver_Init(&adc_config);

// Read temperature (returns value * 10, e.g., 235 = 23.5°C)
int16_t temp_x10 = AdcDriver_ReadTemperature(ADC1);
printf("Temperature: %d.%d C\n", temp_x10 / 10, abs(temp_x10 % 10));
```

## API Reference

### Initialization Functions
| Function | Description |
|----------|-------------|
| `AdcDriver_Init()` | Initialize ADC peripheral with configuration |
| `AdcDriver_ConfigurePin()` | Configure GPIO pin as analog input |
| `AdcDriver_Calibrate()` | Perform ADC calibration (automatic in Init) |
| `AdcDriver_DeInit()` | De-initialize ADC and release resources |

### Single-Channel Functions
| Function | Description |
|----------|-------------|
| `AdcDriver_ReadChannel()` | Read single channel with custom sampling time (blocking) |
| `AdcDriver_ReadChannelDefault()` | Read single channel with default sampling time (blocking) |
| `AdcDriver_StartConversion()` | Start conversion (non-blocking) |
| `AdcDriver_IsConversionComplete()` | Check if conversion is complete |
| `AdcDriver_GetConversionValue()` | Get conversion result |

### Multi-Channel Functions
| Function | Description |
|----------|-------------|
| `AdcDriver_InitMultiChannel()` | Initialize multi-channel scan with DMA |
| `AdcDriver_StartMultiChannel()` | Start continuous multi-channel conversion |
| `AdcDriver_StopMultiChannel()` | Stop continuous conversion |

### Special Functions
| Function | Description |
|----------|-------------|
| `AdcDriver_ReadTemperature()` | Read internal temperature sensor (°C * 10) |
| `AdcDriver_ReadVREFINT()` | Read internal voltage reference (mV) |
| `AdcDriver_ConvertToMillivolts()` | Convert ADC value to millivolts |

## Configuration Parameters

### Operating Modes (`AdcMode_t`)
- `ADC_MODE_SINGLE` - Single conversion mode
- `ADC_MODE_CONTINUOUS` - Continuous conversion mode
- `ADC_MODE_SCAN` - Scan mode for multiple channels
- `ADC_MODE_DMA` - DMA mode with continuous conversion

### Resolution (`AdcResolution_t`)
- `ADC_RESOLUTION_12BIT` - 12-bit (0-4095) [Default]
- `ADC_RESOLUTION_10BIT` - 10-bit (0-1023)
- `ADC_RESOLUTION_8BIT` - 8-bit (0-255)
- `ADC_RESOLUTION_6BIT` - 6-bit (0-63)

### Sampling Time (`AdcSampleTime_t`)
- `ADC_SAMPLE_TIME_1_5` - 1.5 cycles (fastest, least accurate)
- `ADC_SAMPLE_TIME_7_5` - 7.5 cycles
- `ADC_SAMPLE_TIME_13_5` - 13.5 cycles
- `ADC_SAMPLE_TIME_28_5` - 28.5 cycles
- `ADC_SAMPLE_TIME_41_5` - 41.5 cycles
- `ADC_SAMPLE_TIME_55_5` - 55.5 cycles [Recommended for external inputs]
- `ADC_SAMPLE_TIME_71_5` - 71.5 cycles
- `ADC_SAMPLE_TIME_239_5` - 239.5 cycles [Best for temp sensor/VREFINT]

### Clock Prescaler (`AdcClockDiv_t`)
ADC Clock = PCLK2 / prescaler
- `ADC_CLOCK_DIV_2` through `ADC_CLOCK_DIV_32`
- **Important**: Maximum ADC clock is 14 MHz per AT32F403A/407 datasheet
- At 240 MHz system clock, use `ADC_CLOCK_DIV_6` or higher

## Pin Mapping

### ADC1 Channels
| Channel | Pin | Channel | Pin |
|---------|-----|---------|-----|
| 0 | PA0 | 8 | PB0 |
| 1 | PA1 | 9 | PB1 |
| 2 | PA2 | 10 | PC0 |
| 3 | PA3 | 11 | PC1 |
| 4 | PA4 | 12 | PC2 |
| 5 | PA5 | 13 | PC3 |
| 6 | PA6 | 14 | PC4 |
| 7 | PA7 | 15 | PC5 |

### Internal Channels
| Channel | Description |
|---------|-------------|
| 16 | Internal temperature sensor |
| 17 | Internal voltage reference (VREFINT ≈ 1.2V) |

### ADC2/ADC3
Same pin mapping as ADC1 (shared pins, different peripherals)

## DMA Channel Mapping
| ADC Peripheral | DMA Channel | Notes |
|----------------|-------------|-------|
| ADC1 | DMA1 Channel 1 | Recommended for DMA mode |
| ADC2 | None | No dedicated DMA |
| ADC3 | DMA2 Channel 5 | Requires DMA2 clock |

## Integration with P-Block

The ADC driver is designed for P-Block's universal input system:
- **11 configurable inputs** (analog/digital/temperature)
- **Multi-channel DMA** for efficient continuous reading
- **0-10V input range** support
- **Temperature sensor** integration (Dol12/Tafco)

See `AdcDriver_Examples.md` for complete P-Block universal input example.

## Performance Characteristics

### Conversion Time (12-bit resolution)
| Sampling Time | Total Cycles | Time @ 14 MHz ADC Clock |
|---------------|--------------|------------------------|
| 1.5 cycles | 14 cycles | ~1.0 µs |
| 55.5 cycles | 68 cycles | ~4.9 µs |
| 239.5 cycles | 252 cycles | ~18.0 µs |

### DMA Transfer Rate
- Up to 16 channels per ADC
- Continuous conversion without CPU intervention
- Buffer updated automatically by DMA controller

## Troubleshooting

### ADC reads always 0 or 4095
**Cause**: GPIO not configured as analog input or reference voltage issue

**Solution**:
```cpp
// Ensure GPIO is configured before reading
AdcDriver_ConfigurePin(GPIOA, GPIO_PINS_0);
```

### DMA buffer not updating
**Cause**: DMA not properly configured or clock not enabled

**Solution**:
```cpp
// Ensure DMA is enabled in config
adc_config.enable_dma = true;
adc_config.mode = ADC_MODE_DMA;
```

### Inaccurate readings
**Cause**: Insufficient sampling time or no calibration

**Solution**:
```cpp
// Use longer sampling time for accuracy
adc_config.default_sample_time = ADC_SAMPLE_TIME_55_5;
// Calibration is automatic, but ensure it completes
```

### ADC clock too fast
**Cause**: Clock prescaler set too low

**Solution**:
```cpp
// At 240 MHz system clock, use DIV_6 or higher
adc_config.clock_div = ADC_CLOCK_DIV_6;  // 240/6 = 40 MHz / 2.5 = 16 MHz
```

## Best Practices

1. **Always configure GPIO pins** before reading ADC channels
   ```cpp
   AdcDriver_ConfigurePin(GPIOA, GPIO_PINS_0);
   ```

2. **Use appropriate sampling time** for your source impedance
   - Low impedance: `ADC_SAMPLE_TIME_7_5`
   - High impedance: `ADC_SAMPLE_TIME_55_5` or higher
   - Temperature/VREFINT: `ADC_SAMPLE_TIME_239_5`

3. **Enable DMA for multiple channels** to reduce CPU load
   ```cpp
   adc_config.mode = ADC_MODE_DMA;
   adc_config.enable_dma = true;
   ```

4. **Check ADC clock frequency**
   ```
   ADC_Clock = SystemClock / AHB_Prescaler / APB2_Prescaler / ADC_Prescaler
   Must be ≤ 14 MHz
   ```

5. **Calibrate after configuration changes**
   ```cpp
   AdcDriver_Calibrate(ADC1);
   ```

## Dependencies
- AT32F403A/407 HAL drivers
- GPIO driver (`gpio.h`, `gpio.cpp`)
- DMA controller (for multi-channel mode)
- Standard C library (`string.h`)

## Tested Hardware
- AT32F403ACGT7
- AT32F407VGT7
- AT32F403ACGU7

## References
1. AT32F403A/407 Datasheet (DS_AT32F403A_V2.04_EN)
2. AT32F403A/407 Reference Manual (RM_AT32F403A_407_EN_V2.06)
3. AT32F403A/407 Errata Sheet (ES0002_AT32F403A_407_Errata_Sheet_EN_V2.0.11)
4. Context7 AT32F403A/407 Documentation (`/xyphrodite/at32f403a-407-docs`)

## License
This driver is part of the TafcoPigstore project and follows the same license (MIT License with attribution to Artery Technology).

## Version History
- **v1.0.0** (2025-11-24): Initial release
  - Single-channel and multi-channel support
  - DMA integration
  - Temperature sensor and VREFINT support
  - AT32 errata workarounds implemented
  - Comprehensive documentation and examples

## Support
For questions, issues, or contributions, please refer to the main TafcoPigstore project documentation.

---
**Created using Context7 documentation**: `/xyphrodite/at32f403a-407-docs`

