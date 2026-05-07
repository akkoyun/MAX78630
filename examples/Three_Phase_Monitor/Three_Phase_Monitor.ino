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

    // Print Header
    Serial.println("--- MAX78630 Three-Phase Monitor ---");

}

void loop() {

    // Read Voltage (RMS)
    float V_R = Energy_Analyzer.Voltage(MAX78630Def::Phase_R, MAX78630Def::RMS);
    float V_S = Energy_Analyzer.Voltage(MAX78630Def::Phase_S, MAX78630Def::RMS);
    float V_T = Energy_Analyzer.Voltage(MAX78630Def::Phase_T, MAX78630Def::RMS);

    // Read Current (RMS)
    float I_R = Energy_Analyzer.Current(MAX78630Def::Phase_R, MAX78630Def::RMS);
    float I_S = Energy_Analyzer.Current(MAX78630Def::Phase_S, MAX78630Def::RMS);
    float I_T = Energy_Analyzer.Current(MAX78630Def::Phase_T, MAX78630Def::RMS);

    // Read Active Power
    float P_R = Energy_Analyzer.Power(MAX78630Def::Phase_R, MAX78630Def::Active);
    float P_S = Energy_Analyzer.Power(MAX78630Def::Phase_S, MAX78630Def::Active);
    float P_T = Energy_Analyzer.Power(MAX78630Def::Phase_T, MAX78630Def::Active);

    // Read Power Factor
    float PF_R = Energy_Analyzer.Power_Factor(MAX78630Def::Phase_R);
    float PF_S = Energy_Analyzer.Power_Factor(MAX78630Def::Phase_S);
    float PF_T = Energy_Analyzer.Power_Factor(MAX78630Def::Phase_T);

    // Read Line Frequency
    float Hz = Energy_Analyzer.Frequency();

    // Read IC Temperature
    float Temp = Energy_Analyzer.Temperature();

    // Print All Measurements
    Serial.println("-------------------------------");
    Serial.println("Voltage  R:" + String(V_R, 1) + " S:" + String(V_S, 1) + " T:" + String(V_T, 1) + " V");
    Serial.println("Current  R:" + String(I_R, 3) + " S:" + String(I_S, 3) + " T:" + String(I_T, 3) + " A");
    Serial.println("Power    R:" + String(P_R, 1) + " S:" + String(P_S, 1) + " T:" + String(P_T, 1) + " W");
    Serial.println("PF       R:" + String(PF_R, 3) + " S:" + String(PF_S, 3) + " T:" + String(PF_T, 3));
    Serial.println("Freq: " + String(Hz, 2) + " Hz  Temp: " + String(Temp, 1) + " C");

    delay(1000);

}
