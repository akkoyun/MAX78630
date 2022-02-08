#include "MAX78630.h"
#include "VT100.h"

void setup() {

  // Start Terminal
	Serial.begin(115200);

  // Start Analayser
	Energy_Analayser.Begin();

}

void loop() {

  VT100.setTextColor(VT_CYAN); VT100.setCursor(6, 36); Serial.print(" ");
  VT100.setTextColor(VT_CYAN); VT100.setCursor(7, 36); Serial.print(" ");
  VT100.setTextColor(VT_CYAN); VT100.setCursor(8, 36); Serial.print(" ");

  VT100.setTextColor(VT_CYAN); VT100.setCursor(6, 30); Serial.print(Energy_Analayser.Voltage_Instantaneous('R'));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(7, 30); Serial.print(Energy_Analayser.Voltage_Instantaneous('S'));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(8, 30); Serial.print(Energy_Analayser.Voltage_Instantaneous('T'));

  VT100.setTextColor(VT_CYAN); VT100.setCursor(10, 30); Serial.print(Energy_Analayser.Voltage_RMS('R'));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(11, 30); Serial.print(Energy_Analayser.Voltage_RMS('S'));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(12, 30); Serial.print(Energy_Analayser.Voltage_RMS('T'));

  VT100.setTextColor(VT_CYAN); VT100.setCursor(14, 30); Serial.print(Energy_Analayser.Voltage_Fundamental('R'));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(15, 30); Serial.print(Energy_Analayser.Voltage_Fundamental('S'));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(16, 30); Serial.print(Energy_Analayser.Voltage_Fundamental('T'));

  VT100.setTextColor(VT_CYAN); VT100.setCursor(18, 30); Serial.print(Energy_Analayser.Voltage_Harmonic('R'));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(19, 30); Serial.print(Energy_Analayser.Voltage_Harmonic('S'));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(20, 30); Serial.print(Energy_Analayser.Voltage_Harmonic('T'));


  VT100.setTextColor(VT_CYAN); VT100.setCursor(6, 70); Serial.print(Energy_Analayser.Current_Instantaneous('R'));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(7, 70); Serial.print(Energy_Analayser.Current_Instantaneous('S'));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(8, 70); Serial.print(Energy_Analayser.Current_Instantaneous('T'));

  VT100.setTextColor(VT_CYAN); VT100.setCursor(10, 70); Serial.print(Energy_Analayser.Current_RMS('R'));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(11, 70); Serial.print(Energy_Analayser.Current_RMS('S'));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(12, 70); Serial.print(Energy_Analayser.Current_RMS('T'));

  VT100.setTextColor(VT_CYAN); VT100.setCursor(14, 70); Serial.print(Energy_Analayser.Current_Fundamental('R'));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(15, 70); Serial.print(Energy_Analayser.Current_Fundamental('S'));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(16, 70); Serial.print(Energy_Analayser.Current_Fundamental('T'));

  VT100.setTextColor(VT_CYAN); VT100.setCursor(18, 70); Serial.print(Energy_Analayser.Current_Harmonic('R'));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(19, 70); Serial.print(Energy_Analayser.Current_Harmonic('S'));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(20, 70); Serial.print(Energy_Analayser.Current_Harmonic('T'));






/*

Serial.print("R Phase RMS Voltage         : "); Serial.println(Energy_Analayser.Voltage_RMS('R'));
Serial.print("S Phase RMS Voltage         : "); Serial.println(Energy_Analayser.Voltage_RMS('S'));
Serial.print("T Phase RMS Voltage         : "); Serial.println(Energy_Analayser.Voltage_RMS('T'));

Serial.print("R Phase Instant Voltage     : "); Serial.println(Energy_Analayser.Voltage_Instantaneous('R'));
Serial.print("S Phase Instant Voltage     : "); Serial.println(Energy_Analayser.Voltage_Instantaneous('S'));
Serial.print("T Phase Instant Voltage     : "); Serial.println(Energy_Analayser.Voltage_Instantaneous('T'));

Serial.print("R Phase Fundamental Voltage : "); Serial.println(Energy_Analayser.Voltage_Fundamental('R'));
Serial.print("S Phase Fundamental Voltage : "); Serial.println(Energy_Analayser.Voltage_Fundamental('S'));
Serial.print("T Phase Fundamental Voltage : "); Serial.println(Energy_Analayser.Voltage_Fundamental('T'));

Serial.print("R Phase Harmonic Voltage    : "); Serial.println(Energy_Analayser.Voltage_Harmonic('R'));
Serial.print("S Phase Harmonic Voltage    : "); Serial.println(Energy_Analayser.Voltage_Harmonic('S'));
Serial.print("T Phase Harmonic Voltage    : "); Serial.println(Energy_Analayser.Voltage_Harmonic('T'));

Serial.print("Voltage Target              : "); Serial.println(Energy_Analayser.Voltage_V_Target());

Serial.print("RMS Min Voltage             : "); Serial.println(Energy_Analayser.Voltage_RMS_Min());
Serial.print("RMS Max Voltage             : "); Serial.println(Energy_Analayser.Voltage_RMS_Max());

Serial.print("Voltage SAG Limit           : "); Serial.println(Energy_Analayser.Voltage_SAG_Limit());

Serial.print("IC Temperature              : "); Serial.print(Energy_Analayser.IC_Temperature()); Serial.println(" C");
Serial.println("------------------------------------------");
*/
//delay(5000);
}
