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
    Serial.println("--- MAX78630 Energy Monitor ---");

}

void loop() {

    // Read Active Energy (Received / Imported)
    float Wh_R_in  = Energy_Analyzer.Energy(MAX78630Def::Phase_R, MAX78630Def::Active_Received);
    float Wh_S_in  = Energy_Analyzer.Energy(MAX78630Def::Phase_S, MAX78630Def::Active_Received);
    float Wh_T_in  = Energy_Analyzer.Energy(MAX78630Def::Phase_T, MAX78630Def::Active_Received);

    // Read Active Energy (Delivered / Exported)
    float Wh_R_out = Energy_Analyzer.Energy(MAX78630Def::Phase_R, MAX78630Def::Active_Delivered);
    float Wh_S_out = Energy_Analyzer.Energy(MAX78630Def::Phase_S, MAX78630Def::Active_Delivered);
    float Wh_T_out = Energy_Analyzer.Energy(MAX78630Def::Phase_T, MAX78630Def::Active_Delivered);

    // Read Reactive Energy
    float VARh_R = Energy_Analyzer.Energy(MAX78630Def::Phase_R, MAX78630Def::ReActive_Received);
    float VARh_S = Energy_Analyzer.Energy(MAX78630Def::Phase_S, MAX78630Def::ReActive_Received);
    float VARh_T = Energy_Analyzer.Energy(MAX78630Def::Phase_T, MAX78630Def::ReActive_Received);

    // Print Energy Measurements
    Serial.println("-------------------------------");
    Serial.println("Active Received  R:" + String(Wh_R_in, 2) + " S:" + String(Wh_S_in, 2) + " T:" + String(Wh_T_in, 2) + " Wh");
    Serial.println("Active Delivered R:" + String(Wh_R_out, 2) + " S:" + String(Wh_S_out, 2) + " T:" + String(Wh_T_out, 2) + " Wh");
    Serial.println("Reactive         R:" + String(VARh_R, 2) + " S:" + String(VARh_S, 2) + " T:" + String(VARh_T, 2) + " VARh");

    delay(2000);

}
