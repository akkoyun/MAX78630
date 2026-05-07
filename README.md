# MAX78630 Library V5.0.1

![GitHub release (latest by date)](https://img.shields.io/github/v/release/akkoyun/MAX78630) ![arduino-library-badge](https://www.ardu-badge.com/badge/MAX78630.svg?) ![GitHub stars](https://img.shields.io/github/stars/akkoyun/MAX78630?style=flat&logo=github) [![PlatformIO Registry](https://badges.registry.platformio.org/packages/akkoyun/library/MAX78630.svg)](https://registry.platformio.org/libraries/akkoyun/MAX78630)

---

## Abstract

The **B201** is a three-phase energy measurement module (EMM) built around the **MAX78630+PPM** polyphase energy-metering processor. It is designed for real-time monitoring in industrial three-phase applications and fits on a compact 3×4 cm PCB.

The module provides six analog inputs for voltage and current sensors (current transformers, Rogowski coils, or resistive shunts). A 24-bit delta-sigma converter front-end feeds an embedded measurement processor that computes all energy parameters and communicates results to the host via a UART (SSI) interface.

---

## Installation

### Arduino IDE
1. **Sketch → Include Library → Manage Libraries…**
2. Search for `MAX78630` and click **Install**.

### PlatformIO
```ini
lib_deps = akkoyun/MAX78630
```

### Manual
Clone or download this repository and place it in your Arduino `libraries/` folder.

---

## Quick Start

```cpp
#include "MAX78630.h"

// Pass the hardware serial port used to communicate with the module
MAX78630 Energy(Serial2);

void setup() {
    Serial2.begin(38400);
    Energy.Begin();          // initialise with default address (4)
}

void loop() {
    float V_R = Energy.Voltage(MAX78630Def::Phase_R, MAX78630Def::RMS);
    float I_R = Energy.Current(MAX78630Def::Phase_R, MAX78630Def::RMS);
    float P_R = Energy.Power  (MAX78630Def::Phase_R, MAX78630Def::Active);
    float F   = Energy.Frequency();
    delay(1000);
}
```

---

## Default Serial Port

The macro `__MAX78630_Serial__` (default: `Serial2`) is reserved for compile-time override. You always supply the stream through the constructor, so any `HardwareSerial` or `Stream`-compatible object works.

To change the default at compile time:
```cpp
#define __MAX78630_Serial__ Serial1
#include "MAX78630.h"
```

---

## Constructor

```cpp
explicit MAX78630(Stream &_Serial);
```

Binds the library instance to the given serial stream. Configure baud rate before calling `Begin()`.

```cpp
MAX78630 Energy(Serial2);
```

---

## Initialisation

### `Begin()`

```cpp
bool Begin(
    uint8_t _Address  = 4,
    bool    _Scale    = true,
    bool    _Bucket   = true,
    bool    _Limit    = true,
    bool    _MinMax   = true,
    bool    _Alarm    = true
);
```

Connects to the module, verifies firmware, and applies all configuration from `Config.h`. Returns `true` if the device was found and initialised successfully.

| Parameter | Default | Description |
|-----------|---------|-------------|
| `_Address` | `4` | Device bus address `0`–`4`. Address 4 → bus byte `0xC4`. |
| `_Scale` | `true` | Write voltage / current full-scale scratch registers. |
| `_Bucket` | `true` | Write energy-bucket size registers. |
| `_Limit` | `true` | Write all alarm threshold registers from `Config.h`. |
| `_MinMax` | `true` | Assign and clear all 8 min/max monitors. |
| `_Alarm` | `true` | Configure AL1/AL2 alarm output pin masks. |

```cpp
// Minimal — address 3, skip alarm pin setup
bool ok = Energy.Begin(3, true, true, true, true, false);
```

---

## Measurement Functions

All measurement functions block for ~3 ms per call (UART round-trip). Use only as fast as needed.

### Phase Constants (`_Phase`)

| Constant | Description |
|----------|-------------|
| `MAX78630Def::Phase_R` | Phase R (L1) |
| `MAX78630Def::Phase_S` | Phase S (L2) |
| `MAX78630Def::Phase_T` | Phase T (L3) |
| `MAX78630Def::Phase_Avg` | Three-phase average / total |

---

### `Voltage()` — Volts

```cpp
float Voltage(uint8_t _Phase, uint8_t _Type);
```

| `_Type` | Description |
|---------|-------------|
| `MAX78630Def::RMS` | True RMS voltage |
| `MAX78630Def::Instant` | Instantaneous sample |
| `MAX78630Def::Fundamental` | 50/60 Hz fundamental component |
| `MAX78630Def::Harmonic` | Harmonic component (order set via `Harmonic()`) |

`Phase_Avg` supports `RMS` only.

```cpp
float V_rms  = Energy.Voltage(MAX78630Def::Phase_R,   MAX78630Def::RMS);
float V_fund = Energy.Voltage(MAX78630Def::Phase_S,   MAX78630Def::Fundamental);
float V_harm = Energy.Voltage(MAX78630Def::Phase_T,   MAX78630Def::Harmonic);
float V_avg  = Energy.Voltage(MAX78630Def::Phase_Avg, MAX78630Def::RMS);
```

---

### `Current()` — Amperes

```cpp
float Current(uint8_t _Phase, uint8_t _Type);
```

| `_Type` | Description |
|---------|-------------|
| `MAX78630Def::RMS` | True RMS current |
| `MAX78630Def::Instant` | Instantaneous sample |
| `MAX78630Def::Fundamental` | Fundamental component |
| `MAX78630Def::Harmonic` | Harmonic component |
| `MAX78630Def::Peak` | Peak current |
| `MAX78630Def::Phase_Compensation` | Phase compensation offset register |

`Phase_Avg` supports `RMS` only.

```cpp
float I_rms  = Energy.Current(MAX78630Def::Phase_R,   MAX78630Def::RMS);
float I_peak = Energy.Current(MAX78630Def::Phase_S,   MAX78630Def::Peak);
float I_avg  = Energy.Current(MAX78630Def::Phase_Avg, MAX78630Def::RMS);
```

---

### `Frequency()` — Hz

```cpp
float Frequency(void);
```

Returns the measured line frequency.

```cpp
float Hz = Energy.Frequency();   // e.g. 50.01
```

---

### `Power_Factor()` — cos φ

```cpp
float Power_Factor(uint8_t _Phase);
```

Returns power factor in the range −1.0 … +1.0. `Phase_Avg` returns the total system power factor.

```cpp
float pf_R = Energy.Power_Factor(MAX78630Def::Phase_R);
float pf   = Energy.Power_Factor(MAX78630Def::Phase_Avg);
```

---

### `Power()` — W / VAR / VA

```cpp
float Power(uint8_t _Phase, uint8_t _Type);
```

| `_Type` | Unit | Description |
|---------|------|-------------|
| `MAX78630Def::Active` | W | Active (real) power |
| `MAX78630Def::ReActive` | VAR | Total reactive power |
| `MAX78630Def::Apparent` | VA | Apparent power |
| `MAX78630Def::Fundamental` | W | Fundamental active power |
| `MAX78630Def::Harmonic` | W | Harmonic active power |
| `MAX78630Def::Fund_ReActive` | VAR | Fundamental reactive power |
| `MAX78630Def::Harm_ReActive` | VAR | Harmonic reactive power |
| `MAX78630Def::Fund_VA` | VA | Fundamental apparent power |

`Phase_Avg` supports `Active`, `ReActive`, and `Apparent`.

```cpp
float W   = Energy.Power(MAX78630Def::Phase_R,   MAX78630Def::Active);
float VAR = Energy.Power(MAX78630Def::Phase_S,   MAX78630Def::ReActive);
float VA  = Energy.Power(MAX78630Def::Phase_T,   MAX78630Def::Apparent);
float W_T = Energy.Power(MAX78630Def::Phase_Avg, MAX78630Def::Active);
```

---

### `Energy()` — Wh / VARh

```cpp
float Energy(uint8_t _Phase, uint8_t _Type);
```

Returns or resets the accumulated energy counter. The energy bucket size is configured in `Config.h` (`BucketH` / `BucketL`).

| `_Type` | Description |
|---------|-------------|
| `MAX78630Def::Active_Received` | Received (imported) active energy |
| `MAX78630Def::Active_Delivered` | Delivered (exported) active energy |
| `MAX78630Def::ReActive_Received` | Leading reactive energy |
| `MAX78630Def::ReActive_Delivered` | Lagging reactive energy |
| `MAX78630Def::Energy_Reset` | Reset all energy counters for this phase |

```cpp
float Wh_in  = Energy.Energy(MAX78630Def::Phase_R, MAX78630Def::Active_Received);
float Wh_out = Energy.Energy(MAX78630Def::Phase_R, MAX78630Def::Active_Delivered);

// Reset energy counters for Phase R
Energy.Energy(MAX78630Def::Phase_R, MAX78630Def::Energy_Reset);
```

---

### `Temperature()` — °C

```cpp
float Temperature(void);
```

Returns the MAX78630 die temperature.

```cpp
float degC = Energy.Temperature();   // e.g. 28.5
```

---

## Status & Alarm Functions

### `Status()`

```cpp
uint32_t Status(uint8_t _Function = MAX78630Def::GET);
```

Reads the 24-bit alarm/status register.

| `_Function` | Description |
|-------------|-------------|
| `MAX78630Def::GET` | Read and cache the register (default) |
| `MAX78630Def::CLEAR` | Read then clear sticky alarm bits |

```cpp
uint32_t st = Energy.Status();
uint32_t st = Energy.Status(MAX78630Def::CLEAR);
```

---

### `Alarm()`

```cpp
bool Alarm(uint16_t _Alarm_Type);
```

Returns `true` if the given alarm condition is currently active.

| `_Alarm_Type` | Condition |
|---------------|-----------|
| `MAX78630Def::Alarm_HighVoltage` | Any phase over-voltage |
| `MAX78630Def::Alarm_LowVoltage` | Any phase under-voltage |
| `MAX78630Def::Alarm_HighCurrent` | Any phase over-current |
| `MAX78630Def::Alarm_HighFreq` | Frequency too high |
| `MAX78630Def::Alarm_LowFreq` | Frequency too low |
| `MAX78630Def::Alarm_VImbal` | Voltage imbalance exceeded |
| `MAX78630Def::Alarm_IImbal` | Current imbalance exceeded |
| `MAX78630Def::Alarm_LowTemp` | Temperature below lower limit |
| `MAX78630Def::Alarm_HighTemp` | Temperature above upper limit |
| `MAX78630Def::Alarm_LowPF` | Power factor below limit |

```cpp
if (Energy.Alarm(MAX78630Def::Alarm_HighVoltage)) { /* handle */ }
if (Energy.Alarm(MAX78630Def::Alarm_LowFreq))     { /* handle */ }
```

---

### `Adjust_Limit()` — hysteresis

```cpp
void Adjust_Limit(void);
```

Checks each active alarm and tightens the corresponding limit by the `*Diff` offset defined in `Config.h` while the alarm is active; restores the nominal limit once the alarm clears. Prevents alarm chatter. Call once per measurement cycle.

```cpp
void loop() {
    Energy.Adjust_Limit();
    // ... read measurements ...
}
```

---

## Min / Max Recording

The IC continuously tracks min/max for up to 8 user-selected signals. The default assignments are configured in `Config.h`.

### `Min_Value()` / `Max_Value()`

```cpp
float Min_Value(uint8_t _Function, uint8_t _MM_ADDR, uint16_t _Scale = 1);
float Max_Value(uint8_t _Function, uint8_t _MM_ADDR, uint16_t _Scale = 1);
```

Reads the recorded minimum or maximum for the selected channel, **then clears the hardware register** (so the next accumulation period starts from scratch). Passing `MAX78630Def::CLEAR` skips the read and only clears.

| Parameter | Description |
|-----------|-------------|
| `_Function` | `MAX78630Def::GET` — read & clear. `MAX78630Def::CLEAR` — clear only (returns `0`). |
| `_MM_ADDR` | Monitor channel: `MAX78630Def::Monitor1` … `Monitor8` |
| `_Scale` | Scale factor applied to the raw result. |

Default monitor assignments (from `Config.h`):

| Monitor | Signal | Recommended `_Scale` |
|---------|--------|----------------------|
| `Monitor1` | VRMS Phase R | `MAX78630Config::VScale` (667) |
| `Monitor2` | VRMS Phase S | `MAX78630Config::VScale` |
| `Monitor3` | VRMS Phase T | `MAX78630Config::VScale` |
| `Monitor4` | IRMS Phase R | `MAX78630Config::IScale` (7) |
| `Monitor5` | IRMS Phase S | `MAX78630Config::IScale` |
| `Monitor6` | IRMS Phase T | `MAX78630Config::IScale` |
| `Monitor7` | Total Power Factor | `1` |
| `Monitor8` | Line Frequency | `1` |

```cpp
float V_min = Energy.Min_Value(MAX78630Def::GET, MAX78630Def::Monitor1, MAX78630Config::VScale);
float I_max = Energy.Max_Value(MAX78630Def::GET, MAX78630Def::Monitor4, MAX78630Config::IScale);

// Clear monitor 7 without reading
Energy.Min_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor7);
```

---

## Harmonic Selector

```cpp
uint8_t Harmonic(uint8_t _Function, uint32_t _Harmonic);
```

Gets or sets the harmonic order (2–63) used for `Fundamental` / `Harmonic` measurement types.

```cpp
Energy.Harmonic(MAX78630Def::SET, 3);              // select 3rd harmonic
uint8_t h = Energy.Harmonic(MAX78630Def::GET, 0);  // read current order
```

---

## Configuration (`src/Config.h`)

All limits and scale factors are `constexpr` values in the `MAX78630Config` namespace. Edit `Config.h` to customise for your hardware.

| Constant | Default | Description |
|----------|---------|-------------|
| `Firmware` | `0x0004D912` | Expected IC firmware version |
| `VScale` | `667` | Voltage full-scale multiplier |
| `IScale` | `7` | Current full-scale multiplier |
| `BucketH` / `BucketL` | `0x000821` / `0xD0F4C2` | Energy accumulation bucket size |
| `StatusSticky` | `false` | Sticky mode for alarm bits |
| `VoltageMin` | `190.0` V | Lower voltage alarm threshold |
| `VoltageMax` | `253.0` V | Upper voltage alarm threshold |
| `VoltageMinDiff` | `10.0` V | Low-voltage hysteresis |
| `VoltageMaxDiff` | `10.0` V | High-voltage hysteresis |
| `CurrentMax` | `5.0` A | Over-current threshold |
| `CurrentMaxDiff` | `0.5` A | Over-current hysteresis |
| `FreqMin` | `47.0` Hz | Under-frequency threshold |
| `FreqMax` | `52.0` Hz | Over-frequency threshold |
| `FreqMinDiff` / `FreqMaxDiff` | `1.0` Hz | Frequency hysteresis |
| `TempMin` | `10.0` °C | Lower temperature threshold |
| `TempMax` | `35.0` °C | Upper temperature threshold |
| `TempMinDiff` / `TempMaxDiff` | `2.0` °C | Temperature hysteresis |
| `VImbMax` | `0.06` | Max voltage imbalance fraction |
| `IImbMax` | `0.06` | Max current imbalance fraction |
| `VSagLim` | `0.9` | Voltage sag threshold (fraction of nominal) |

---

## Measured Parameters

| Parameter | R | S | T | Avg/Total |
|-----------|:-:|:-:|:-:|:---------:|
| RMS Voltage | ✓ | ✓ | ✓ | ✓ |
| Instantaneous Voltage | ✓ | ✓ | ✓ | — |
| Fundamental Voltage | ✓ | ✓ | ✓ | — |
| Harmonic Voltage | ✓ | ✓ | ✓ | — |
| RMS Current | ✓ | ✓ | ✓ | ✓ |
| Instantaneous Current | ✓ | ✓ | ✓ | — |
| Peak Current | ✓ | ✓ | ✓ | — |
| Fundamental Current | ✓ | ✓ | ✓ | — |
| Harmonic Current | ✓ | ✓ | ✓ | — |
| Active Power | ✓ | ✓ | ✓ | ✓ |
| Reactive Power | ✓ | ✓ | ✓ | ✓ |
| Apparent Power | ✓ | ✓ | ✓ | ✓ |
| Fundamental Active Power | ✓ | ✓ | ✓ | — |
| Harmonic Active Power | ✓ | ✓ | ✓ | — |
| Fundamental Reactive Power | ✓ | ✓ | ✓ | — |
| Harmonic Reactive Power | ✓ | ✓ | ✓ | — |
| Fundamental Apparent Power | ✓ | ✓ | ✓ | — |
| Power Factor | ✓ | ✓ | ✓ | ✓ |
| Active Energy (received) | ✓ | ✓ | ✓ | — |
| Active Energy (delivered) | ✓ | ✓ | ✓ | — |
| Reactive Energy (leading) | ✓ | ✓ | ✓ | — |
| Reactive Energy (lagging) | ✓ | ✓ | ✓ | — |
| Line Frequency | — | — | — | ✓ |
| IC Die Temperature | — | — | — | ✓ |

---

## Examples

| Example | Description |
|---------|-------------|
| `Volt_Meter` | Reads and prints RMS voltage for all three phases every 500 ms. |
| `Three_Phase_Monitor` | Full three-phase readout: voltage, current, active power, power factor, frequency, and temperature. |
| `Energy_Monitor` | Reads and prints received/delivered active and reactive energy accumulators for all three phases. |
| `Alarm_Monitor` | Checks all alarm conditions, applies hysteresis via `Adjust_Limit()`, and prints min/max recorded values. |

---

## Version History

| Version | Changes |
|---------|---------|
| **5.0.1** | `pow()` loop replaced with sign-extension + single division (faster, eliminates FP accumulation error). Internal accumulator changed `double` → `float`. Missing `return 0.0f` added to all measurement functions. Monitor8 `Min_Value`/`Max_Value` clear calls fixed. `Min_Value`/`Max_Value` refactored from 8-case switches to lookup-table helper. All configuration and protocol constants moved to `constexpr` namespaces. All register definitions moved to `MAX78630Reg` namespace. Correct two's complement `FtoS` for negative values. |
| 4.12.0 | Previous release. |

---

## Want to Buy?

[![I Sell on Tindie](https://img.shields.io/badge/I_Sell_on-Tindie-blue.svg)](https://www.tindie.com/stores/akkoyun)

---

[![Support me](https://img.shields.io/badge/Support-PATREON-GREEN.svg)](https://www.patreon.com/bePatron?u=62967889) ![Twitter Follow](https://img.shields.io/twitter/follow/gunceakkoyun?style=social) ![YouTube Channel Views](https://img.shields.io/youtube/channel/views/UCIguQGdaBT1GnnVMz5qAZ2Q?style=social) [![E-Mail](https://img.shields.io/badge/E_Mail-Mehmet_Gunce_Akkoyun-blue.svg)](mailto:akkoyun@me.com) ![GitHub](https://img.shields.io/github/license/akkoyun/MAX78630)
