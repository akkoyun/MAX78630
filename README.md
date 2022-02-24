# MAX78630 Library <sup>V4.1</sup>

![GitHub release (latest by date)](https://img.shields.io/github/v/release/akkoyun/MAX78630) ![arduino-library-badge](https://www.ardu-badge.com/badge/MAX78630.svg?) ![Visits Badge](https://badges.pufler.dev/visits/akkoyun/MAX78630) ![GitHub stars](https://img.shields.io/github/stars/akkoyun/MAX78630?style=flat&logo=github) ![Updated Badge](https://badges.pufler.dev/updated/akkoyun/MAX78630) ![PlatformIO Registry](https://badges.registry.platformio.org/packages/akkoyun/library/MAX78630.svg)

	Polly Phase Energy Meter - 04.01.00

---

<img src="B201.png" width="800">

**Abstract**

B201 module is an energy measurement module (EMM) for polyphase power monitoring systems. It is designed for real-time monitoring for a variety of typical three-phase configurations in industrial applications. It is available in a 3x4cm size PCB module.

The B201 provides up to six analog inputs for interfacing to voltage and current sensors. Scaled voltages from the sensors are fed to the single converter front-end utilizing a high-resolution delta-sigma converter. Supported current sensors include current transformers (CT), Rogowski coils, and resistive shunts.

An embedded 24-bit measurement processor and firmware perform all necessary computations and data formatting for accurate reporting to the host. With integrated flash memory for storing nonvolatile calibration coefficients and device configuration settings, the B201 is capable of being a completely autonomous solution.

The B201 is designed to interface to the host processor via the UART interface.

**Module PinOut**

<img src="PinOut.jpg" width="600">

**Measured Parameters**

In this Arduino Library we can read all data of energy parameters.

| Parameter                  | Phase R | Phase S | Phase T |
|----------------------------|---------|---------|---------|
| Instant Voltage            | Yes     | Yes     | Yes     |
| RMS Voltage                | Yes     | Yes     | Yes     |
| Fundamental Voltage        | Yes     | Yes     | Yes     |
| Harmonic Voltage           | Yes     | Yes     | Yes     |
| Frequency                  | -       | -       | -       |
| Instant Current            | Yes     | Yes     | Yes     |
| RMS Current                | Yes     | Yes     | Yes     |
| Peak Current               | Yes     | Yes     | Yes     |
| Fundamental Current        | Yes     | Yes     | Yes     |
| Harmonic Current           | Yes     | Yes     | Yes     |
| Active Power               | Yes     | Yes     | Yes     |
| ReActive Power             | Yes     | Yes     | Yes     |
| Apparent Power             | Yes     | Yes     | Yes     |
| Fundamental Power          | Yes     | Yes     | Yes     |
| Harmonic Power             | Yes     | Yes     | Yes     |
| Power Factor               | Yes     | Yes     | Yes     |
| Fundamental ReActive Power | Yes     | Yes     | Yes     |
| Harmonic Reactive Power    | Yes     | Yes     | Yes     |
| Fundamental VA Power       | Yes     | Yes     | Yes     |
| IC Temperature             | -       | -       | -       |

Also set limits for alarm monitoring.

<img src="/Documents/Terminal.png" width="800">

---

**Want to buy ?**

[![Want to buu](https://img.shields.io/badge/I_Sell_on-Tindie-blue.svg)](mailto:akkoyun@me.com)

---

[![Support me](https://img.shields.io/badge/Support-PATREON-GREEN.svg)](https://www.patreon.com/bePatron?u=62967889) ![Twitter Follow](https://img.shields.io/twitter/follow/gunceakkoyun?style=social) ![YouTube Channel Views](https://img.shields.io/youtube/channel/views/UCIguQGdaBT1GnnVMz5qAZ2Q?style=social) ![Repos Badge](https://badges.pufler.dev/repos/akkoyun) [![E-Mail](https://img.shields.io/badge/E_Mail-Mehmet_Gunce_Akkoyun-blue.svg)](mailto:akkoyun@me.com) ![GitHub](https://img.shields.io/github/license/akkoyun/Statistical) 