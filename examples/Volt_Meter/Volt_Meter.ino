// Define Libraries
#include <MAX78630.h>
#include <Console.h>

// Declare Object
MAX78630 Energy_Analayser;

void setup() {

	// Define Serial Object
	Serial.begin(115200);

    // Start Terminal Console
    Terminal.Begin(Serial);

    // Draw Base Console
	Terminal.MAX78630_Voltmeter();

	// Define Serial Object
    Serial2.begin(38400);
    Energy_Analayser.Begin(Serial2);

	// Get Limits
	Terminal.Text(20, 18, WHITE, String(Energy_Analayser.Read_Voltage_Max_Limit()));
	Terminal.Text(21, 18, WHITE, String(Energy_Analayser.Read_Voltage_Min_Limit()));

	Terminal.Text(20, 30, WHITE, String(Energy_Analayser.Read_Frequency_Max_Limit()));
	Terminal.Text(21, 30, WHITE, String(Energy_Analayser.Read_Frequency_Min_Limit()));

	Terminal.Text(20, 50, WHITE, String(Energy_Analayser.Read_Voltage_Imbalance_Max_Limit()));
	Terminal.Text(21, 50, WHITE, String(Energy_Analayser.Read_Current_Imbalance_Max_Limit()));
}

void loop() {

    // Print Measurements
    Terminal.Text(7, 18, CYAN, String(Energy_Analayser.Voltage('R', __RMS__),2));
    Terminal.Text(8, 18, CYAN, String(Energy_Analayser.Get_Min_Value(1),2));
    Terminal.Text(9, 18, CYAN, String(Energy_Analayser.Get_Max_Value(1),2));

    Terminal.Text(7, 45, CYAN, String(Energy_Analayser.Voltage('S', __RMS__),2));
    Terminal.Text(8, 45, CYAN, String(Energy_Analayser.Get_Min_Value(2),2));
    Terminal.Text(9, 45, CYAN, String(Energy_Analayser.Get_Max_Value(2),2));

    Terminal.Text(7, 72, CYAN, String(Energy_Analayser.Voltage('T', __RMS__),2));
    Terminal.Text(8, 72, CYAN, String(Energy_Analayser.Get_Min_Value(3),2));
    Terminal.Text(9, 72, CYAN, String(Energy_Analayser.Get_Max_Value(3),2));

	// Terminal Command Routine
	if (Serial.available() > 0) {
	
		uint16_t Key = Serial.read();

		// Terminal Beep
		Terminal.Beep();

		switch (Key) {
		case '1':
            Energy_Analayser.Clear_Min_Value(1);
            Energy_Analayser.Clear_Max_Value(1);
			break;		
		case '2':
            Energy_Analayser.Clear_Min_Value(2);
            Energy_Analayser.Clear_Max_Value(2);
			break;		
		case '3':
            Energy_Analayser.Clear_Min_Value(3);
            Energy_Analayser.Clear_Max_Value(3);
			break;

		default:
			break;
		}
	}

	// Loop Delay
	delay(500);

}
