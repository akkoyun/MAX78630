#include "MAX78630.h"
#include "VT100.h"

void setup() {

  // Start Terminal
	Serial.begin(115200);

  // Start Analayser
	Energy_Analayser.Begin();

}

void loop() {

  VT100.setTextColor(VT_CYAN); VT100.setCursor(6, 30); Serial.print(Energy_Analayser.Voltage_Harmonic('R', 1));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(7, 30); Serial.print(Energy_Analayser.Voltage_Harmonic('R', 3));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(8, 30); Serial.print(Energy_Analayser.Voltage_Harmonic('R', 5));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(9, 30); Serial.print(Energy_Analayser.Voltage_Harmonic('R', 7));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(10, 30); Serial.print(Energy_Analayser.Voltage_Harmonic('R', 9));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(11, 30); Serial.print(Energy_Analayser.Voltage_Harmonic('R', 11));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(12, 30); Serial.print(Energy_Analayser.Voltage_Harmonic('R', 13));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(13, 30); Serial.print(Energy_Analayser.Voltage_Harmonic('R', 15));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(14, 30); Serial.print(Energy_Analayser.Voltage_Harmonic('R', 17));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(15, 30); Serial.print(Energy_Analayser.Voltage_Harmonic('R', 19));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(16, 30); Serial.print(Energy_Analayser.Voltage_Harmonic('R', 21));
  VT100.setTextColor(VT_CYAN); VT100.setCursor(17, 30); Serial.print(Energy_Analayser.Voltage_Harmonic('R', 23));



//delay(5000);
}
