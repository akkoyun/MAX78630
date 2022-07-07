// Include Arduino Library
#ifndef __Arduino__
	#include <Arduino.h>
#endif

// Library Console Definations
#ifndef __Console__
	#include <Console.h>
#endif

// Define Libraries
#include <MAX78630.h>

// Declare Energy Analyzer
MAX78630 Energy_Analyzer;
Console Terminal(Serial);

void setup() {

	// Start Serial Terminal Communication
	Serial.begin(115200);

	// Start MAX78630 Communication
	Serial2.begin(38400);

    // Start Terminal Console
    Terminal.Begin();

    // Draw Base Console
	Terminal.MAX78630_Voltmeter();

	// Start Power Analyzer
	Energy_Analyzer.Begin(Serial2);

}

void loop() {

    // Print Measurements
    Terminal.Text(7, 18, CYAN, String(Energy_Analyzer.Voltage(__Phase_R__, __RMS__),2));
    Terminal.Text(7, 45, CYAN, String(Energy_Analyzer.Voltage(__Phase_S__, __RMS__),2));
    Terminal.Text(7, 72, CYAN, String(Energy_Analyzer.Voltage(__Phase_T__, __RMS__),2));
    Terminal.Text(13, 3, CYAN, String(Energy_Analyzer.Energy(__Phase_R__, __Active_Received__)));

	// Terminal Command Routine
	if (Serial.available() > 0) {
	
		uint16_t Key = Serial.read();

		// Terminal Beep
		Terminal.Beep();

		switch (Key) {
		case '1':
            Energy_Analyzer.Clear_Min_Value(1);
            Energy_Analyzer.Clear_Max_Value(1);
			break;		
		case '2':
            Energy_Analyzer.Clear_Min_Value(2);
            Energy_Analyzer.Clear_Max_Value(2);
			break;		
		case '3':
            Energy_Analyzer.Clear_Min_Value(3);
            Energy_Analyzer.Clear_Max_Value(3);
			break;

		default:
			break;
		}
	}

	// Loop Delay
	delay(500);

}
