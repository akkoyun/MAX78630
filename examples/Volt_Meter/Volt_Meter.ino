// Include Arduino Library
#ifndef Arduino_h
	#include <Arduino.h>
#endif

// Include MAX78630 Library
#ifndef __MAX78630__
	#include <MAX78630.h>
#endif

// Declare Energy Analyzer
MAX78630 Energy_Analyzer(__MAX78630_Serial__);

void setup() {

	// Start Serial Terminal Communication
	Serial.begin(115200);

	// Start MAX78630 Communication
	Serial2.begin(38400);

	// Start Power Analyzer
	Energy_Analyzer.Begin();

}

void loop() {

    // Print Measurements
	Serial.println("Phase R: " + String(Energy_Analyzer.Voltage(__Phase_R__, __RMS__),2) + " V");
	Serial.println("Phase S: " + String(Energy_Analyzer.Voltage(__Phase_S__, __RMS__),2) + " V");
	Serial.println("Phase T: " + String(Energy_Analyzer.Voltage(__Phase_T__, __RMS__),2) + " V");

	// Loop Delay
	delay(500);

}
