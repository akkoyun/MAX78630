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

    // Initialise Power Analyzer (alarm pin masks enabled)
    Energy_Analyzer.Begin();

    // Print Header
    Serial.println("--- MAX78630 Alarm Monitor ---");

}

void loop() {

    // Adjust alarm hysteresis limits before reading alarms
    Energy_Analyzer.Adjust_Limit();

    // Read Status Register
    uint32_t st = Energy_Analyzer.Status(MAX78630Def::GET);

    // Check Individual Alarms
    if (Energy_Analyzer.Alarm(MAX78630Def::Alarm_HighVoltage)) Serial.println("ALARM: Over-Voltage detected!");
    if (Energy_Analyzer.Alarm(MAX78630Def::Alarm_LowVoltage))  Serial.println("ALARM: Under-Voltage detected!");
    if (Energy_Analyzer.Alarm(MAX78630Def::Alarm_HighCurrent)) Serial.println("ALARM: Over-Current detected!");
    if (Energy_Analyzer.Alarm(MAX78630Def::Alarm_HighFreq))    Serial.println("ALARM: Frequency too high!");
    if (Energy_Analyzer.Alarm(MAX78630Def::Alarm_LowFreq))     Serial.println("ALARM: Frequency too low!");
    if (Energy_Analyzer.Alarm(MAX78630Def::Alarm_VImbal))      Serial.println("ALARM: Voltage imbalance!");
    if (Energy_Analyzer.Alarm(MAX78630Def::Alarm_IImbal))      Serial.println("ALARM: Current imbalance!");
    if (Energy_Analyzer.Alarm(MAX78630Def::Alarm_HighTemp))    Serial.println("ALARM: Temperature too high!");
    if (Energy_Analyzer.Alarm(MAX78630Def::Alarm_LowTemp))     Serial.println("ALARM: Temperature too low!");
    if (Energy_Analyzer.Alarm(MAX78630Def::Alarm_LowPF))       Serial.println("ALARM: Power Factor too low!");

    // Print Min / Max recorded values (Monitor1–6: VRMS R/S/T, IRMS R/S/T)
    float V_min_R = Energy_Analyzer.Min_Value(MAX78630Def::GET, MAX78630Def::Monitor1, MAX78630Config::VScale);
    float V_max_R = Energy_Analyzer.Max_Value(MAX78630Def::GET, MAX78630Def::Monitor1, MAX78630Config::VScale);
    float I_min_R = Energy_Analyzer.Min_Value(MAX78630Def::GET, MAX78630Def::Monitor4, MAX78630Config::IScale);
    float I_max_R = Energy_Analyzer.Max_Value(MAX78630Def::GET, MAX78630Def::Monitor4, MAX78630Config::IScale);

    Serial.println("--- Phase R Min/Max ---");
    Serial.println("V_min:" + String(V_min_R, 1) + " V_max:" + String(V_max_R, 1) + " V");
    Serial.println("I_min:" + String(I_min_R, 3) + " I_max:" + String(I_max_R, 3) + " A");
    Serial.println("Status: 0x" + String(st, HEX));

    delay(1000);

}
