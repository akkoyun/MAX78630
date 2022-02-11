#include "MAX78630.h"

void setup() {

  // Start Terminal
	Serial.begin(115200);

  // Start Analayser
	Energy_Analayser.Begin();

}

void loop() {

  Energy_Analayser.Control_Limits();

  Serial.print(Energy_Analayser.Voltage_RMS('R'), 3);
  
  delay(1000);

}
