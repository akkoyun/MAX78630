#include "MAX78630.h"
#include "VT100.h"

void setup() {

  // Start Terminal
	Serial.begin(115200);

  // Start Analayser
	Energy_Analayser.Begin();


  VT100.setCursor(26, 54); Serial.print(Energy_Analayser.Get_Limit(1));
  VT100.setCursor(25, 54); Serial.print(Energy_Analayser.Get_Limit(2));
  VT100.setCursor(26, 21); Serial.print(Energy_Analayser.Get_Limit(3));
  VT100.setCursor(25, 21); Serial.print(Energy_Analayser.Get_Limit(4));
  VT100.setCursor(21, 21); Serial.print(Energy_Analayser.Get_Limit(5));
  VT100.setCursor(20, 21); Serial.print(Energy_Analayser.Get_Limit(6));
  VT100.setCursor(21, 55); Serial.print(Energy_Analayser.Get_Limit(7));
  VT100.setCursor(28, 21); Serial.print(Energy_Analayser.Get_Limit(9));
  VT100.setCursor(29, 21); Serial.print(Energy_Analayser.Get_Limit(10));

}

void loop() {

  Energy_Analayser.Control_Limits();

  VT100.setTextColor(VT_CYAN); 

  VT100.setCursor(9, 21); Serial.print(Energy_Analayser.Voltage_RMS('R'), 3);
  VT100.setCursor(10, 21); Serial.print(Energy_Analayser.Voltage_RMS('S'), 3);
  VT100.setCursor(11, 21); Serial.print(Energy_Analayser.Voltage_RMS('T'), 3);

  VT100.setCursor(6, 55); Serial.print(abs(Energy_Analayser.Current_Instantaneous('R')), 3);
  VT100.setCursor(7, 55); Serial.print(abs(Energy_Analayser.Current_Instantaneous('S')), 3);
  VT100.setCursor(8, 55); Serial.print(abs(Energy_Analayser.Current_Instantaneous('T')), 3);

  VT100.setCursor(12, 55); Serial.print(Energy_Analayser.Current_RMS('R'), 3);
  VT100.setCursor(13, 55); Serial.print(Energy_Analayser.Current_RMS('S'), 3);
  VT100.setCursor(14, 55); Serial.print(Energy_Analayser.Current_RMS('T'), 3);

  VT100.setCursor(24, 21); Serial.print(Energy_Analayser.Frequency());
  
  VT100.setCursor(24, 54); Serial.print(Energy_Analayser.IC_Temperature());

  VT100.setCursor(32, 21); Serial.print(Energy_Analayser.Get_Bucket_HIGH(), HEX);
  VT100.setCursor(33, 21); Serial.print(Energy_Analayser.Get_Bucket_LOW(), HEX);

  VT100.setCursor(35, 21); Serial.print(Energy_Analayser.Active_Energy_Recieved('R'), 10);
  VT100.setCursor(36, 21); Serial.print(Energy_Analayser.Active_Energy_Delivered('R'), 10);

  VT100.setCursor(38, 21); Serial.print(Energy_Analayser.Get_Samples());


  VT100.setCursor(6, 95); if (Energy_Analayser.Limit.Current_Imbalance) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(7, 95); if (Energy_Analayser.Limit.Voltage_Imbalance) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(8, 95); if (Energy_Analayser.Limit.Sag_VR) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(9, 95); if (Energy_Analayser.Limit.Sag_VS) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(10, 95); if (Energy_Analayser.Limit.Sag_VT) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(11, 95); if (Energy_Analayser.Limit.Current_Over_Limit_VR) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(12, 95); if (Energy_Analayser.Limit.Current_Over_Limit_VS) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(13, 95); if (Energy_Analayser.Limit.Current_Over_Limit_VT) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(14, 95); if (Energy_Analayser.Limit.Power_Factor_Under_Limit_VR) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(15, 95); if (Energy_Analayser.Limit.Power_Factor_Under_Limit_VS) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(16, 95); if (Energy_Analayser.Limit.Power_Factor_Under_Limit_VT) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(17, 95); if (Energy_Analayser.Limit.Voltage_Under_Limit_VR) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(18, 95); if (Energy_Analayser.Limit.Voltage_Under_Limit_VS) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(19, 95); if (Energy_Analayser.Limit.Voltage_Under_Limit_VT) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(20, 95); if (Energy_Analayser.Limit.Voltage_Over_Limit_VR) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(21, 95); if (Energy_Analayser.Limit.Voltage_Over_Limit_VS) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(22, 95); if (Energy_Analayser.Limit.Voltage_Over_Limit_VT) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(23, 95); if (Energy_Analayser.Limit.Temperature_Under_Limit) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(24, 95); if (Energy_Analayser.Limit.Temperature_Over_Limit) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(25, 95); if (Energy_Analayser.Limit.Frequency_Under_Limit) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  VT100.setCursor(26, 95); if (Energy_Analayser.Limit.Frequency_Over_Limit) {VT100.setTextColor(VT_RED); Serial.print("X");} else {VT100.setTextColor(VT_CYAN); Serial.print("-");}
  
//delay(5000);
}
