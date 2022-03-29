// Define Libraries
#include <MAX78630.h>
#include <Console.h>

void setup() {

	// Define Serial Object
	Serial.begin(115200);

    // Start Terminal Console
    Terminal.Begin(Serial);

    // Draw Base Console
	Terminal.MAX78630();

	// Define Serial Object
    Serial2.begin(38400);
    Energy_Analayser.Begin(Serial2);

    // Print Constants
    Terminal.Text(6, 22, MAGENTA, String(Energy_Analayser.VScale(0)));
    Terminal.Text(7, 22, MAGENTA, String(Energy_Analayser.Voltage_Gain('R', -999),5));
    Terminal.Text(8, 22, MAGENTA, String(Energy_Analayser.Voltage_Gain('S', -999),5));
    Terminal.Text(9, 22, MAGENTA, String(Energy_Analayser.Voltage_Gain('T', -999),5));
    Terminal.Text(10, 22, MAGENTA, String(Energy_Analayser.Voltage_Offset('R', -999),5));
    Terminal.Text(11, 22, MAGENTA, String(Energy_Analayser.Voltage_Offset('S', -999),5));
    Terminal.Text(12, 22, MAGENTA, String(Energy_Analayser.Voltage_Offset('T', -999),5));
    Terminal.Text(13, 22, MAGENTA, String(Energy_Analayser.Voltage_HPF_COEF(-999),5));

    Terminal.Text(6, 53, MAGENTA, String(Energy_Analayser.IScale(0)));
    Terminal.Text(7, 53, MAGENTA, String(Energy_Analayser.Current_Gain('R', -999),5));
    Terminal.Text(8, 53, MAGENTA, String(Energy_Analayser.Current_Gain('S', -999),5));
    Terminal.Text(9, 53, MAGENTA, String(Energy_Analayser.Current_Gain('T', -999),5));
    Terminal.Text(10, 53, MAGENTA, String(Energy_Analayser.Current_Offset('R', -999),5));
    Terminal.Text(11, 53, MAGENTA, String(Energy_Analayser.Current_Offset('S', -999),5));
    Terminal.Text(12, 53, MAGENTA, String(Energy_Analayser.Current_Offset('T', -999),5));
    Terminal.Text(13, 53, MAGENTA, String(Energy_Analayser.Current_HPF_COEF(-999),5));

    Terminal.Text(31, 22, MAGENTA, String(Energy_Analayser.Limit(4, -999),2));
    Terminal.Text(32, 22, MAGENTA, String(Energy_Analayser.Limit(3, -999),2));

}

void loop() {

    // Print Measurements
    Terminal.Text(14, 22, CYAN, String(Energy_Analayser.Voltage_RMS('R'),2));
    Terminal.Text(15, 22, CYAN, String(Energy_Analayser.Voltage_RMS('S'),2));
    Terminal.Text(16, 22, CYAN, String(Energy_Analayser.Voltage_RMS('T'),2));
    Terminal.Text(17, 22, CYAN, String(Energy_Analayser.Voltage_RMS('A'),2));
    Terminal.Text(18, 22, CYAN, String("       "));
    Terminal.Text(19, 22, CYAN, String("       "));
    Terminal.Text(20, 22, CYAN, String("       "));
    Terminal.Text(18, 22, CYAN, String(Energy_Analayser.Voltage_Instantaneous('R'),2));
    Terminal.Text(19, 22, CYAN, String(Energy_Analayser.Voltage_Instantaneous('S'),2));
    Terminal.Text(20, 22, CYAN, String(Energy_Analayser.Voltage_Instantaneous('T'),2));
    Terminal.Text(21, 22, CYAN, String(Energy_Analayser.Voltage_Fundamental('R'),2));
    Terminal.Text(22, 22, CYAN, String(Energy_Analayser.Voltage_Fundamental('S'),2));
    Terminal.Text(23, 22, CYAN, String(Energy_Analayser.Voltage_Fundamental('T'),2));
    Terminal.Text(24, 22, CYAN, String(Energy_Analayser.Voltage_Harmonic('R'),2));
    Terminal.Text(25, 22, CYAN, String(Energy_Analayser.Voltage_Harmonic('S'),2));
    Terminal.Text(26, 22, CYAN, String(Energy_Analayser.Voltage_Harmonic('T'),2));

    Terminal.Text(14, 53, CYAN, String(Energy_Analayser.Current_RMS('R'),2));
    Terminal.Text(15, 53, CYAN, String(Energy_Analayser.Current_RMS('S'),2));
    Terminal.Text(16, 53, CYAN, String(Energy_Analayser.Current_RMS('T'),2));
    Terminal.Text(17, 53, CYAN, String(Energy_Analayser.Current_RMS('A'),2));
    Terminal.Text(18, 53, CYAN, String("       "));
    Terminal.Text(19, 53, CYAN, String("       "));
    Terminal.Text(20, 53, CYAN, String("       "));
    Terminal.Text(18, 53, CYAN, String(Energy_Analayser.Current_Instantaneous('R'),2));
    Terminal.Text(19, 53, CYAN, String(Energy_Analayser.Current_Instantaneous('S'),2));
    Terminal.Text(20, 53, CYAN, String(Energy_Analayser.Current_Instantaneous('T'),2));
    Terminal.Text(21, 53, CYAN, String(Energy_Analayser.Current_Fundamental('R'),2));
    Terminal.Text(22, 53, CYAN, String(Energy_Analayser.Current_Fundamental('S'),2));
    Terminal.Text(23, 53, CYAN, String(Energy_Analayser.Current_Fundamental('T'),2));
    Terminal.Text(24, 53, CYAN, String(Energy_Analayser.Current_Harmonic('R'),2));
    Terminal.Text(25, 53, CYAN, String(Energy_Analayser.Current_Harmonic('S'),2));
    Terminal.Text(26, 53, CYAN, String(Energy_Analayser.Current_Harmonic('T'),2));

    Terminal.Text(30, 22, CYAN, String(Energy_Analayser.Frequency(),2));

    Terminal.Text(35, 10, CYAN, String(Energy_Analayser.Get_Min_Value(1),2));
    Terminal.Text(36, 10, CYAN, String(Energy_Analayser.Get_Max_Value(1),2));

    Terminal.Text(35, 20, CYAN, String(Energy_Analayser.Get_Min_Value(2),2));
    Terminal.Text(36, 20, CYAN, String(Energy_Analayser.Get_Max_Value(2),2));

    Terminal.Text(35, 30, CYAN, String(Energy_Analayser.Get_Min_Value(3),2));
    Terminal.Text(36, 30, CYAN, String(Energy_Analayser.Get_Max_Value(3),2));

	// Loop Delay
	delay(1000);

}
