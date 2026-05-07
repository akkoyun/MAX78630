// Include Arduino Library
#ifndef Arduino_h
    #include <Arduino.h>
#endif

// Include MAX78630 Library
#ifndef __MAX78630__
    #include <MAX78630.h>
#endif

// Declare Energy Analyzer
MAX78630 Energy_Analyzer(Serial2);

void setup() {

    // Start Serial Terminal Communication
    Serial.begin(115200);

    // Start MAX78630 Communication
    Serial2.begin(38400);

    // Initialise Power Analyzer
    Energy_Analyzer.Begin();

}

void loop() {

    // Print Measurements
    Serial.println("Phase R: " + String(Energy_Analyzer.Voltage(MAX78630Def::Phase_R, MAX78630Def::RMS), 2) + " V");
    Serial.println("Phase S: " + String(Energy_Analyzer.Voltage(MAX78630Def::Phase_S, MAX78630Def::RMS), 2) + " V");
    Serial.println("Phase T: " + String(Energy_Analyzer.Voltage(MAX78630Def::Phase_T, MAX78630Def::RMS), 2) + " V");

    // Loop Delay
    delay(500);

}
