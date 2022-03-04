#include "MAX78630.h"

void setup() {

	// Define Serial Object
	Serial.begin(115200);

	// Define Serial Object
    Energy_Analayser.Begin();

}

void loop() {

	// Print Settings
	Serial.print("Baud           : "); Serial.print(Energy_Analayser.Get_Baud()); Serial.println("");
	Serial.print("Device Addres  : 0x"); Serial.print(Energy_Analayser.Get_Device_Address(), HEX); Serial.println("");
	Serial.print("System Stat    : 0b"); Serial.print(Energy_Analayser.Get_System_Stat(), BIN); Serial.println("");
	Serial.print("Firmware       : 0x"); Serial.print(Energy_Analayser.Get_Firmware(), HEX); Serial.println("");

	Serial.print("VScale         : "); Serial.print(Energy_Analayser.VScale(0)); Serial.println("");
	Serial.print("IScale         : "); Serial.print(Energy_Analayser.IScale(0)); Serial.println("");

	Serial.print("Voltage Gain   : "); Serial.print(Energy_Analayser.Get_Voltage_Gain('R'), 5); Serial.println("");
	Serial.print("Current Gain   : "); Serial.print(Energy_Analayser.Get_Current_Gain('R'), 5); Serial.println("");
	Serial.print("Voltage Offset : "); Serial.print(Energy_Analayser.Get_Voltage_Offset('R'), 5); Serial.println("");
	Serial.print("Current Offset : "); Serial.print(Energy_Analayser.Get_Current_Offset('R'), 5); Serial.println("");

	// Print Measurements
	Serial.print("RMS Voltage    : "); Serial.print(Energy_Analayser.Voltage_RMS('R'), 3); Serial.print(" V"); Serial.print(" - ["); Serial.print(Energy_Analayser.Get_Min_Value(1)); Serial.print(" - "); Serial.print(Energy_Analayser.Get_Max_Value(1));Serial.print("]");Serial.println("");
	Serial.print("RMS Current    : "); Serial.print(Energy_Analayser.Current_RMS('R'), 3); Serial.print(" A"); Serial.print(" - ["); Serial.print(Energy_Analayser.Get_Min_Value(4)); Serial.print(" - "); Serial.print(Energy_Analayser.Get_Max_Value(4));Serial.print("]");Serial.println("");

	Serial.print("Energy         : "); Serial.print(Energy_Analayser.Active_Energy_Recieved('R'), 0); Serial.println(" Wh");
	Serial.print("Reactive Energy: "); Serial.print(Energy_Analayser.ReActive_Energy_Recieved('R'), 0); Serial.println(" VARh");
	Serial.print("Active Power   : "); Serial.print(Energy_Analayser.Active_Power('R')); Serial.println(" W");
	Serial.print("ReActive Power : "); Serial.print(Energy_Analayser.ReActive_Power('R')); Serial.println(" VAR");
	Serial.print("Apparent Power : "); Serial.print(Energy_Analayser.Apparent_Power('R')); Serial.println(" VA");
	Serial.print("Power Factor   : "); Serial.print(Energy_Analayser.Power_Factor('R')); Serial.println(" ");
	Serial.print("Frequency      : "); Serial.print(Energy_Analayser.Frequency()); Serial.println(" Hz");
	Serial.print("IC Temperature : "); Serial.print(Energy_Analayser.IC_Temperature()); Serial.println(" C");
	Serial.println("------------------------------------------");

	// Loop Delay
	delay(10000);

}
