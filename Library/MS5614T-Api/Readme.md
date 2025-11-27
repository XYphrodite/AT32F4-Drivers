# MS5614 / MS5614T  

## 2.5 V to 5.5 V, 12-Bit, Quad Voltage-Output DAC with Power-Down  

**Hangzhou Ruimeng Technology Co., Ltd**  

### Product Description

MS5614/5614T is a 12bit four channel output voltage DAC, the interface uses the four wire serial port mode, it can be compatible with TMS320, SPI, QSPI and Microwire serial port. MS5614/5614T control data has 16bit, including DAC address, control byte, and 12bitDAC data, the power range is 2.7V to 5.5V. The output of resistor series is connected to a class AB rail to rail buffer with a gain of 6dB. The output buffer improves the stability and reduces the setup time. MS5614/5614T has power down mode, which can optimize power consumption at work.

- **MS5614** – SOP-16 package  
- **MS5614T** – TSSOP-16 package  

### Features

- 12-bit resolution  
- Programmable settling time: 3 µs (fast) or 9 µs (slow)  
- SPI/QSPI/MICROWIRE/TMS320 compatible  
- Internal power-on reset  
- Integrated reference buffer  
- Output range = 2 × VREF (rail-to-rail)  
- Software and hardware power-down  
- Supply voltage: 2.7 V to 5.5 V  

### Applications

- Digital servo system control  
- Digital Compensation and Gain Adjustment  
- Industrial process control  
- Mechanical and Mobile Control Equipment
- High-capacity storage devices  

### Ordering Information

| Part Number | Package  | Marking   |
|-------------|----------|-----------|
| MS5614      | SOP-16   | MS5614    |
| MS5614T     | TSSOP-16 | MS5614T   |

---

### Pin Configuration and Description (SOP-16 / TSSOP-16)

```text
        ┌──────────────┐
    DVDD│ 1          16│AVDD
     PD │ 2          15│REFINAB
   LDAC │ 3          14│OUTA
    DIN │ 4          13│OUTB
   SCLK │ 5          12│OUTC
     CS │ 6          11│OUTD
     FS │ 7          10│REFINCD
   DGND │ 8           9│AGND
        └──────────────┘
```

| Pin | Name    | Type | Description                                      |
|-----|---------|------|--------------------------------------------------|
| 1   | DVDD    | –    | Digital power supply                             |
| 2   | PD      | I    | Power Down Pin, when the input low level, turn off the chip                          |
| 3   | LDAC    | I    | When LDAC input is high level, DAC output is not updated; when LDAC input is low level, DAC output is updated.                 |
| 4   | DIN     | I    | Serial data input                                |
| 5   | SCLK    | I    | Serial Digital Clock Input                                     |
| 6   | CS      | I    | Chip select (active low)                         |
| 7   | FS      | I    | Frame Synchronization Input Signal (TMS320 mode)                         |
| 8   | DGND    | –    | Digital ground                                   |
| 9   | AGND    | –    | Analog ground                                    |
| 10  | REFINCD | I    | Reference voltage for channels C & D             |
| 11  | OUTD    | O    | Channel D analog output                          |
| 12  | OUTC    | O    | Channel C analog output                          |
| 13  | OUTB    | O    | Channel B analog output                          |
| 14  | OUTA    | O    | Channel A analog output                          |
| 15  | REFINAB | I    | Reference voltage for channels A & B             |
| 16  | AVDD    | –    | Analog power supply                              |

### Functional Block Diagram

```text
                    ┌─────────────────────────────────────────┐
                    │  Input Interface and Logic Control      │
    DIN ────────────┤                                         │
    FS ─────────────┤                                         │
    SCLK ───────────┤                                         │
    CS ─────────────┤                                         │
                    └─────┬───────────┬───────────┬───────────┘
                          │           │           │
                          │           │           │
        ┌─────────────────┴───┐       │           │
        │                     │       │           │
        ▼                     ▼       ▼           ▼
    ┌─────────────────────────────────────────────────────┐
    │  DAC A                                               │
    │                                                       │
    │  REFINAB ──┐                                          │
    │            │                                          │
    │            ▼                                          │
    │        ┌───────┐                                      │
    │        │Buffer │                                      │
    │        └───┬───┘                                      │
    │            │                                          │
    │            ▼                                          │
    │    ┌───────────────┐                                 │
    │    │ Resistor      │                                 │
    │    │ Network       │◄─── 12-bit data                 │
    │    │ (12-bit)      │                                 │
    │    └───────┬───────┘                                 │
    │            │                                          │
    │            ▼                                          │
    │        ┌───────┐                                      │
    │        │ 6dB   │                                      │
    │        │ Amp   │                                      │
    │        └───┬───┘                                      │
    │            │                                          │
    │            ▼                                          │
    │          OUTA                                         │
    │                                                       │
    │  Control: 2-bit control, rate & power down           │
    └─────────────────────────────────────────────────────┘
                          │
                          │
        ┌─────────────────┼─────────────────┐
        │                 │                 │
        ▼                 ▼                 ▼
    ┌─────────┐      ┌─────────┐      ┌─────────┐
    │  DAC B  │      │  DAC C  │      │  DAC D  │
    │         │      │         │      │         │
    │ REFINAB │      │REFINCD  │      │REFINCD  │
    │    │    │      │    │    │      │    │    │
    │    ▼    │      │    ▼    │      │    ▼    │
    │  OUTB   │      │  OUTC   │      │  OUTD   │
    └────┬────┘      └────┬────┘      └────┬────┘
         │                │                │
         └────────────────┼────────────────┘
                          │
                    ┌─────┴─────┐
                    │  LDAC     │
                    │  PD       │
                    └───────────┘
                          │
        ┌─────────────────┴─────────────────┐
        │                                   │
        ▼                                   ▼
    ┌──────────────┐                  ┌──────────────┐
    │ Power On     │                  │   Ground     │
    │ Reset        │                  │   AGND       │
    │ Circuit      │                  │   DGND       │
    └──────────────┘                  └──────────────┘
```

### Output Voltage Formula

Vout = 2 × VREF × (D / 4096)

where D = decimal value of the 12-bit DAC code (0–4095)

---

### Absolute Maximum Ratings

Any exceeding absolute maximum rating application causes permanent damage to device. Because long time absolute operation state affects device reliability. Absolute ratings just conclude from a series of extreme tests. It doesn’t represent chip can operate normally in these extreme conditions.

| Parameter                  | Symbol   | Rating               | Unit |
|--------------------------------|----------|----------------------|------|
| Analog supply voltage          | AVDD     | –0.3 to +7           | V    |
| Digital supply voltage         | DVDD     | –0.3 to +7           | V    |
| AVDD to DVDD difference        |          | –2.8 to +2.8         | V    |
| Digital input voltage          | VIN      | –0.3 to DVDD+0.3     | V    |
| Reference input voltage        | VREFIN   | –0.3 to AVDD+0.3     | V    |
| Operating temperature          | TA       | –40 to +105          | °C   |
| Storage temperature            | Tstg     | –65 to +150          | °C   |
| Max junction temperature       | TJ       | 150                  | °C   |
| Welding Temperature(10s) |   | 260 | °C |

---

### ELECTRICAL CHARACTERISTICS (3.3V)

#### Recommended Operating Conditions

| Parameter                  | Condition          | Min   | Typ    | Max     | Unit  |
|----------------------------|--------------------|-------|--------|---------|-------|
| **Supply Voltage**         | 5V Power           | 4.5   | 5      | 5.5     | V     |
|                            | 3V Power           | 2.7   | 3      | 3.3     | V     |
| **Digital Input High Level VIH** | DVDD=2.7V        | 2     |        |         | V     |
|                            | DVDD=5.5V          | 2.4   |        |         | V     |
| **Digital Input Low Level VIL**  | DVDD=2.7V        |       |        | 0.6     | V     |
|                            | DVDD=5.5V          |       |        | 1       | V     |
| **REF Voltage**            | 5V Power (See Note 1) | 0   | 2.048  | VDD–1.5 | V     |
|                            | 3V Power (See Note 1) | 0   | 1.024  | VDD–1.5 | V     |
| **Load Resistance**        |                    | 2     | 10     |         | kΩ    |
| **Load Capacitance**       |                    |       |        | 100     | pF    |
| **SCLK Rate**              |                    |       |        | 20      | MHz   |

> **Note:** Input voltage greater than AVDD/2 will result in saturated output at large DAC input codes.

#### Static DAC Specifications

| Parameter                              | Condition              | Min   | Typ     | Max   | Unit     |
|----------------------------------------|------------------------|-------|---------|-------|----------|
| Resolution  |        |   12    |         |    | Bits   |
| DNL                               | See Note 2             |       | ±1.5    | ±4    | LSB      |
| INL                                | See Note 3             |       | ±0.5    | ±1    | LSB      |
| Zero Scale Error                  | See Note 4             |       |         | ±12   | mV       |
| Zero Scale Error Temperature Coefficient | See Note 5       |       | 10      |       | ppm/°C   |
| Gain Error                        | See Note 6             |       |         | ±0.6  | % of FS Voltage |
| Gain Error Temperature Coefficient | See Note 7             |       | 10      |       | ppm/°C   |
| PSRR                               | Zero Scale<br>See Note 8 and 9 |       | -80     |       | dB       |
|                                        | Full Scale<br>See Note 8 and 9 |       | -80     |       | dB       |

> **Note:**

2. Relative accuracy or integrated nonlinearity (INL) refers to linear error, which is the maximum deviation of the output from the ideal output by eliminating zero error and full error.

3. Differential Nonlinearity (DNL), the differential error, refers to the maximum amplitude change adjacent to LSB.

4. Zero scale offset refers to the analog output of zero input.

5. Zero scale temperature drift refers to the temperature change of the analog output when digital input is zero.

6. Gain error refers to the deviation between analog output and ideal output after zero error is removed.

7. Gain Error Temperature Drift refers to the variation of the deviation between analog output and ideal output with temperature after zero error is removed.

8. The power rejection ratio at zero points refers to the ratio of change in output caused by a change in AVDD of 5 ±0.5 V and 3 ±0.3 V when the digital input is all zero.

9. When the full-amplitude output power rejection ratio is all higher than the exponential word input, the change ratio of output is caused by the change of AVDD by 5 ±0.5 V and 3 ±0.3 V.

#### DAC Output Specifications

| Parameter                          | Condition             | Min   | Typ   | Max      | Unit     |
|------------------------------------|-----------------------|-------|-------|----------|----------|
| **Voltage Output Range**           | RL=10kΩ               | 0     |       | AVDD–0.4 | V        |
| **Output Load Regulation Accuracy**| RL=2kΩ to 10kΩ       |       | 0.1   | 0.25     | % of FS  |

#### Reference Inputs

| Parameter                          | Condition                                    | Min   | Typ   | Max   | Unit |
|------------------------------------|----------------------------------------------|-------|-------|-------|------|
| **Input Voltage Range**            | See Note 10                                  | 0     |       | AVDD–1.5 | V    |
| **Reference Feed Through**         | REFIN = 1Vpp(1 kHz) + 1.024 V (See Note 11)  |       | -75   |       | dB   |
| **Reference Input Bandwidth**      | REFIN = 0.2Vpp + 1.024 V                     |       |       |       |      |
|                                    | Slow                                         | 0.5   |       |       | MHz  |
|                                    | Fast                                         | 1     |       |       | MHz  |

> **Note:**

10. Reference input voltage over VDD/2 will cause output saturation distortion.

11. Reference feed through refers to the analog output rejection ratio when the output number is zero and REFIN = 1Vpp (1 kHz) + 1.024 V.

#### Digital Inputs

| Parameter                       | Condition   | Min | Typ | Max | Unit |
|---------------------------------|-------------|-----|-----|-----|------|
| **High-level Digital Input Current** | VI = VDD   |     |     | ±1  | µA   |
| **Low-level Digital Input Current**  | VI = 0V    |     |     | ±1  | µA   |

#### Power Supply

| Parameter                       | Condition                                | Min | Typ | Max | Unit |
|---------------------------------|------------------------------------------|-----|-----|-----|------|
| **Power Supply Current**        | 5-V supply, No load Clock running, All inputs 0 V or VDD |     |     |     |      |
|                                 | Slow                                     |     | 1.6 | 2.4 | mA   |
|                                 | Fast                                     |     | 3.8 | 5.6 | mA   |
| **Power Supply Current**        | 3-V supply, No load Clock running, All inputs 0 V or VDD |     |     |     |      |
|                                 | Slow                                     |     | 1.2 | 1.6 | mA   |
|                                 | Fast                                     |     | 3.2 | 4.8 | mA   |
| **Power Down Supply Current**   |                                          |     |     | 10  | nA   |

#### Analog Output Dynamic Performance

| Parameter               | Condition                                                                 | Min | Typ  | Max | Unit    |
|-------------------------|---------------------------------------------------------------------------|-----|------|-----|---------|
| **SR** (Slew Rate)      | CL=100pF, RL=10kΩ, Vo=10% to 90%, Vref=2.048, 1.024                         |     |      |     | V/µs    |
|                         | Slow                                                                      |     | 5    |     | V/µs    |
|                         | Fast                                                                      |     | 1    |     | V/µs    |
| **Ts** (Settling Time)  | to ±0.5LSB, CL=100pF, RL=10kΩ                                             |     |      |     | V/µs    |
|                         | Slow                                                                      |     | 3    | 5.5 | V/µs    |
|                         | Fast                                                                      |     | 9    | 20  | V/µs    |
| **Ts(c)** (Settling Time to ±0.5LSB, Capacitive load) | CL=100pF, RL=10kΩ                   |     |      |     | µs      |
|                         | Slow                                                                      |     | 1    |     | µs      |
|                         | Fast                                                                      |     | 2    |     | µs      |
| **Glitch Energy**       | from 7FF to 800                                                           |     | 10   |     | nV-sec  |
| **SNR**                 | Vref=1.024@3V                                                             |     | 74   |     | dB      |
| **S/(N+D)**             | Vref=2.048@5V, fs=400kSPS, fout=1.1kHz                                    |     | 66   |     | dB      |
| **THD**                 | sinewave, CL=100pF, RL=10kΩ, BW=20kHz                                     |     | -68  |     | dB      |
| **SFDR**                |                                                                           |     | 70   |     | dB      |

#### Digital Input Timing Requirements

| Parameter                  | Description                                                                 | Min | Typ | Max | Unit |
|----------------------------|-----------------------------------------------------------------------------|-----|-----|-----|------|
| **tSU(CS–FS)**             | Setup time, CS low before FS falling                                        | 10  |     |     | ns   |
| **tSU(FS–CK)**             | FS low before first negative SCLK edge                                      | 8   |     |     | ns   |
| **tSU(C16–FS)**            | Sixteenth negative SCLK edge after FS low on which bit of FS DO is sampled before rising edge of FS | 10  |     |     | ns   |
| **tSU(C16–CS)**            | CS rising edge. If FS is used instead of the SCLK positive edge to update the DAC, then the setup time is between the FS rising edge and CS rising edge. | 10  |     |     | ns   |
| **tWH**                    | SCLK high Pulse duration                                                    | 25  |     |     | ns   |
| **tWL**                    | SCLK low Pulse duration                                                     | 25  |     |     | ns   |
| **tSU(D)**                 | data ready before SCLK falling edge                                         | 8   |     |     | ns   |
| **tH(D)**                  | data held valid after SCLK falling edge                                     | 5   |     |     | ns   |
| **tWH(FS)**                | FS high Pulse duration                                                      | 20  |     |     | ns   |

#### Timing Diagram (SPI interface with FS and CS control)

```
                tWL          tWH
            ┌─────┐     ┌─────┐     ┌─────┐
SCLK    ────┘     └─────┘     └─────┘     └─────...──────┐
            1     2     3     4     5          15   16   └─

                  tSU(D)   tH(D)
            ┌───────┐       ┌───────┐
DIN     ────┤ D15    ├───────┤ D14   ├────...───┤ D1    ├───────┤ D0    ├────
                   │               │                 │               │

                            tSU(FS-CK)
            ┌───────────────┐
CS      ────┘               └────────────────────────────────────────────┐
            └─ tSU(CS-FS)                            tSU(C16-CS) ───────┘

            ┌─────────────────────────────────────────────────────────────┐
FS      ────┘                   tWH(FS)             tSU(C16-FS)         └────

```

**Legend:**

- tSU(CS–FS) – CS low before FS falling
- tSU(FS–CK) – FS low before first negative SCLK edge
- tSU(D) – Data setup before SCLK falling edge
- tH(D) – Data hold after SCLK falling edge
- tWL, tWH – SCLK low/high pulse width
- tSU(C16–FS) – 16th SCLK falling edge to FS rising
- tSU(C16–CS) – 16th SCLK falling edge to CS rising (when using CS instead of FS for update)
- tWH(FS) – FS high pulse duration

Data is shifted in on the falling edge of SCLK (MSB first: D15 → D0).

The DAC register is updated on the rising edge of FS (or CS if FS is tied high).

## APPLICATIONS INFORMATION

#### Functional Description

MS5614/5614T is a **12-bit single-power digital-to-analog converter**. Its architecture uses a resistance array structure, which integrates:

- serial interface
- rate and interrupt logic control
- reference input buffer
- resistance string
- output track-to-track amplifier

The output voltage can be expressed as:

$$ V_{OUT} = 2 \times V_{REF} \times \frac{D}{2^{12}} $$

where:  

- \( V_{REF} \) = reference input voltage  
- \( D \) = decimal value of the 12-bit digital input code (0 to 4095)

#### Serial Interface

MS5614/5614T must be set to be valid at low CS level, then the internal DAC updates the corresponding output level at the beginning of bitwise input data (starting at high level) along the descent of FS, after 16 bits have been transferred or when FS becomes higher.

The MS5614/5614T serial port can use two basic modes: four wire (using chip selected CS) and three wire (not using chip selected CS), which allows multiple devices to connect to the serial port German data source

#### Data Format

The data digits of MS5614/5614T consist of two parts: control bits (D15-D12) and digital data (D11-D0).

| MSB | | | | LSB |
|---|---|---|---|---|
| A1 | A0 | PWR | SPD | D11-D0 |


A1, A0 – Device address bits (for multi-device systems; usually 00 for single DAC)
PWR   – Power-down control bit
0 = Normal operation
1 = Power-down mode (output typically high-impedance or pulled to ground via 100 kΩ)

SPD   – Speed mode select
0 = Slow mode (lower power, slower settling)
1 = Fast mode (higher speed, higher power consumption)

D11–D0 – 12-bit digital input code (0 to 4095) that sets the output voltage

The 16-bit word is shifted in MSB-first (A1 first, D0 last).

A1, A0 are the internal DAC channel address selection bits, the true value table is as follows:

| A1 | A0 | Selected DAC |
|----|----|--------------|
| 0  | 0  | DAC A        |
| 0  | 1  | DAC B        |
| 1  | 0  | DAC C        |
| 1  | 1  | DAC D        |

- **PWR** = 1 → Power-down mode  
- **SPD** = 1 → Fast mode (3 µs), 0 → Slow mode (9 µs)

### Timing Requirements (minimum values)

| Symbol       | Description                              | Min | Unit |
|--------------|------------------------------------------|-----|------|
| tSU(CS-FS)   | CS low before FS falling                 | 10  | ns   |
| tSU(FS-CK)   | FS low before first SCLK falling edge    | 8   | ns   |
| tSU(C16-FS)  | D0 setup before FS rising                | 10  | ns   |
| tW(H/L)      | SCLK high/low time                       | 25  | ns   |
| tSU(D)       | DIN setup before SCLK falling           | 8   | ns   |
| tH(D)        | DIN hold after SCLK falling              | 5   | ns   |

---

### Package Dimensions

#### SOP-16

| Symbol | mm (min–max)     | inches (min–max) |
|--------|------------------|------------------|
| A      | 1.350 – 1.750    | 0.053 – 0.069    |
| D      | 9.800 – 10.200   | 0.386 – 0.402    |
| E1     | 5.800 – 6.200    | 0.228 – 0.244    |
| e      | 1.27 BSC         | 0.050 BSC        |

#### TSSOP-16

| Symbol | mm (min–max)     | inches (min–max) |
|--------|------------------|------------------|
| D      | 4.900 – 5.100    | 0.193 – 0.201    |
| E      | 6.250 – 6.550    | 0.246 – 0.258    |
| E1     | 4.300 – 4.500    | 0.169 – 0.177    |
| e      | 0.65 BSC         | 0.026 BSC        |

### Packaging

| Device   | Package  | Qty/Reel | Reels/Box | Qty/Carton |
|----------|----------|----------|-----------|------------|
| MS5614   | SOP-16   | 2500     | 8         | 20 000     |
| MS5614T  | TSSOP-16 | 3000     | 8         | 24 000     |

---

**© Hangzhou Ruimeng Technology Co., Ltd – All rights reserved**  
Latest datasheet: <http://www.relmon.com>  
