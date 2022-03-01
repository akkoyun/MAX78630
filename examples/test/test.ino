#include "MAX78630.h"

void setup() {

	Serial.begin(115200);
	Energy_Analayser.Begin();

}

void loop() {

Serial.print("RMS Voltage    : "); Serial.print(Energy_Analayser.Voltage_RMS('R')); Serial.println(" V");
Serial.print("RMS Current    : "); Serial.print(Energy_Analayser.Current_RMS('R'), 3); Serial.println(" A");
Serial.print("Energy         : "); Serial.print(Energy_Analayser.Active_Energy_Recieved('R'), 0); Serial.println(" Wh");
Serial.print("Reactive Energy: "); Serial.print(Energy_Analayser.ReActive_Energy_Recieved('R'), 0); Serial.println(" VARh");



Serial.print("Active Power   : "); Serial.print(Energy_Analayser.Active_Power('R')); Serial.println(" W");
Serial.print("ReActive Power : "); Serial.print(Energy_Analayser.ReActive_Power('R')); Serial.println(" VAR");
Serial.print("Apparent Power : "); Serial.print(Energy_Analayser.Apparent_Power('R')); Serial.println(" VA");

Serial.print("Power Factor   : "); Serial.print(Energy_Analayser.Power_Factor('R')); Serial.println(" ");

Serial.print("Frequency      : "); Serial.print(Energy_Analayser.Frequency()); Serial.println(" Hz");

Serial.print("IC Temperature : "); Serial.print(Energy_Analayser.IC_Temperature()); Serial.println(" C");
Serial.println("------------------------------------------");

delay(60000);
}
