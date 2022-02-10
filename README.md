## Description
The MAX78630 is an energy measurement processor for polyphase power-monitoring systems. It is designed for real-time monitoring for a variety of typical three-phase configurations in industrial applications. The MAX78630 provides up to six analog inputs for interfacing to voltage and current sensors. Scaled voltages from the sensors are fed to the single converter front-end using a high-resolution delta-sigma converter. Supported current sensors include current transformers (CTs), Rogowski coils, and resistive shunts.  

In this Arduino Library we can read all data of energy parameters.

## Measured Parameters

* Instant voltage for specified phase (R,S,T)
* RMS voltage for specified phase (R,S,T)
* Fundamental voltage for specified phase (R,S,T)
* Harmonic voltage for specified phase (R,S,T) and specified harmonic
* Line frequency
* Instant current for specified phase (R,S,T)
* RMS current for specified phase (R,S,T)
* Peak current for specified phase (R,S,T)
* Fundamental current for specified phase (R,S,T)
* Harmonic current for specified phase (R,S,T) and specified harmonic
* Active power for specified phase (R,S,T)
* ReActive power for specified phase (R,S,T)
* Apparent power for specified phase (R,S,T)
* Fundamental power for specified phase (R,S,T)
* Harmonic power for specified phase (R,S,T) and specified harmonic.
* Power factor for specified phase (R,S,T)
* Fundamental ReActive power for specified phase (R,S,T)
* Harmonic ReActive power for specified phase (R,S,T)
* Fundamental VA power for specified phase (R,S,T)
* IC Temperature

Also set limits for alarm monitoring.

![Terminal](/Documents/Terminal.png)