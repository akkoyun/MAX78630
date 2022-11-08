#define FULL_ENERGY_TEMPLATE

// Include Arduino Library
#ifndef __Arduino__
	#include <Arduino.h>
#endif

// Library Console Definations
#ifndef __Console__
	#include <Console.h>
#endif

// Define Libraries
#include <MAX78630.h>

// Declare Energy Analyzer
MAX78630 Energy_Analyzer;
Console Terminal(Serial);

void setup() {

	// Start Serial Terminal Communication
	Serial.begin(115200);

	// Start MAX78630 Communication
	Serial2.begin(38400);

    // Start Terminal Console
    Terminal.Begin();

    // Draw Base Console
	Terminal.Full_Energy_Analayzer();

	// Start Power Analyzer
	Energy_Analyzer.Begin(Serial2);

    Terminal.Text(16, 151, RED, String(Energy_Analyzer.Get_Device_Address(), HEX));
    Terminal.Text(17, 151, RED, String(Energy_Analyzer.Get_Firmware(), HEX));
    Terminal.Text(18, 151, RED, String(Energy_Analyzer.Get_Baud()));

    Terminal.Text(20, 151, RED, String(Energy_Analyzer.Get_VSCale()));
    Terminal.Text(21, 151, RED, String(Energy_Analyzer.Get_ISCale()));

    Terminal.Text(33, 15, RED, String(Energy_Analyzer.Read_Voltage_Imbalance_Max_Limit(),2));
    Terminal.Text(33, 27, RED, String(Energy_Analyzer.Read_Current_Imbalance_Max_Limit(),2));
    Terminal.Text(33, 39, RED, String(Energy_Analyzer.Read_Voltage_Min_Limit(),2));
    Terminal.Text(33, 50, RED, String(Energy_Analyzer.Read_Voltage_Max_Limit(),2));
    Terminal.Text(33, 61, RED, String(Energy_Analyzer.Get_Voltage_SAG_Limit(),2));
    Terminal.Text(33, 72, RED, String(Energy_Analyzer.Read_Current_Max_Limit(),2));
    Terminal.Text(33, 83, RED, String(Energy_Analyzer.Read_PowerFactor_Min_Limit(),2));
    Terminal.Text(33, 91, RED, String(Energy_Analyzer.Read_Temperature_Min_Limit(),2));
    Terminal.Text(33, 99, RED, String(Energy_Analyzer.Read_Temperature_Max_Limit(),2));
    Terminal.Text(33, 107, RED, String(Energy_Analyzer.Read_Frequency_Min_Limit(),2));
    Terminal.Text(33, 116, RED, String(Energy_Analyzer.Read_Frequency_Max_Limit(),2));

}

void loop() {

    Terminal.Text(19, 151, CYAN, String(Energy_Analyzer.Temperature(),2));

    // Print Measurements
    Terminal.Text(8, 15, CYAN, String(Energy_Analyzer.Voltage(__Phase_R__, __Instant__),2));
    Terminal.Text(9, 15, CYAN, String(Energy_Analyzer.Voltage(__Phase_S__, __Instant__),2));
    Terminal.Text(10, 15, CYAN, String(Energy_Analyzer.Voltage(__Phase_T__, __Instant__),2));
    Terminal.Text(8, 26, CYAN, String(Energy_Analyzer.Voltage(__Phase_R__, __RMS__),2));
    Terminal.Text(9, 26, CYAN, String(Energy_Analyzer.Voltage(__Phase_S__, __RMS__),2));
    Terminal.Text(10, 26, CYAN, String(Energy_Analyzer.Voltage(__Phase_T__, __RMS__),2));
    Terminal.Text(11, 26, CYAN, String(Energy_Analyzer.Voltage(__Phase_Avg__, __RMS__),2));
    Terminal.Text(8, 37, CYAN, String(Energy_Analyzer.Voltage(__Phase_R__, __Fundamental__),2));
    Terminal.Text(9, 37, CYAN, String(Energy_Analyzer.Voltage(__Phase_S__, __Fundamental__),2));
    Terminal.Text(10, 37, CYAN, String(Energy_Analyzer.Voltage(__Phase_T__, __Fundamental__),2));
    Terminal.Text(8, 48, CYAN, String(Energy_Analyzer.Voltage(__Phase_R__, __Harmonic__),2));
    Terminal.Text(9, 48, CYAN, String(Energy_Analyzer.Voltage(__Phase_S__, __Harmonic__),2));
    Terminal.Text(10, 48, CYAN, String(Energy_Analyzer.Voltage(__Phase_T__, __Harmonic__),2));

    Terminal.Text(8, 91, CYAN, String(Energy_Analyzer.Current(__Phase_R__, __Instant__),2));
    Terminal.Text(9, 91, CYAN, String(Energy_Analyzer.Current(__Phase_S__, __Instant__),2));
    Terminal.Text(10, 91, CYAN, String(Energy_Analyzer.Current(__Phase_T__, __Instant__),2));
    Terminal.Text(8, 101, CYAN, String(Energy_Analyzer.Current(__Phase_R__, __Peak__),2));
    Terminal.Text(9, 101, CYAN, String(Energy_Analyzer.Current(__Phase_S__, __Peak__),2));
    Terminal.Text(10, 101, CYAN, String(Energy_Analyzer.Current(__Phase_T__, __Peak__),2));
    Terminal.Text(8, 111, CYAN, String(Energy_Analyzer.Current(__Phase_R__, __RMS__),2));
    Terminal.Text(9, 111, CYAN, String(Energy_Analyzer.Current(__Phase_S__, __RMS__),2));
    Terminal.Text(10, 111, CYAN, String(Energy_Analyzer.Current(__Phase_T__, __RMS__),2));
    Terminal.Text(11, 111, CYAN, String(Energy_Analyzer.Current(__Phase_Avg__, __RMS__),2));
    Terminal.Text(8, 121, CYAN, String(Energy_Analyzer.Current(__Phase_R__, __Fundamental__),2));
    Terminal.Text(9, 121, CYAN, String(Energy_Analyzer.Current(__Phase_S__, __Fundamental__),2));
    Terminal.Text(10, 121, CYAN, String(Energy_Analyzer.Current(__Phase_T__, __Fundamental__),2));
    Terminal.Text(8, 131, CYAN, String(Energy_Analyzer.Current(__Phase_R__, __Harmonic__),2));
    Terminal.Text(9, 131, CYAN, String(Energy_Analyzer.Current(__Phase_S__, __Harmonic__),2));
    Terminal.Text(10, 131, CYAN, String(Energy_Analyzer.Current(__Phase_T__, __Harmonic__),2));

    Terminal.Text(18, 14, CYAN, String(Energy_Analyzer.Power(__Phase_R__, __Active__),2));
    Terminal.Text(19, 14, CYAN, String(Energy_Analyzer.Power(__Phase_S__, __Active__),2));
    Terminal.Text(20, 14, CYAN, String(Energy_Analyzer.Power(__Phase_T__, __Active__),2));
    Terminal.Text(21, 14, CYAN, String(Energy_Analyzer.Power(__Phase_Avg__, __Active__),2));

    Terminal.Text(18, 25, CYAN, String(Energy_Analyzer.Power(__Phase_R__, __ReActive__),2));
    Terminal.Text(19, 25, CYAN, String(Energy_Analyzer.Power(__Phase_S__, __ReActive__),2));
    Terminal.Text(20, 25, CYAN, String(Energy_Analyzer.Power(__Phase_T__, __ReActive__),2));
    Terminal.Text(21, 25, CYAN, String(Energy_Analyzer.Power(__Phase_Avg__, __ReActive__),2));

    Terminal.Text(18, 36, CYAN, String(Energy_Analyzer.Power(__Phase_R__, __Apparent__),2));
    Terminal.Text(19, 36, CYAN, String(Energy_Analyzer.Power(__Phase_S__, __Apparent__),2));
    Terminal.Text(20, 36, CYAN, String(Energy_Analyzer.Power(__Phase_T__, __Apparent__),2));
    Terminal.Text(21, 36, CYAN, String(Energy_Analyzer.Power(__Phase_Avg__, __Apparent__),2));

    Terminal.Text(18, 47, CYAN, String(Energy_Analyzer.Power(__Phase_R__, __Harmonic__),2));
    Terminal.Text(19, 47, CYAN, String(Energy_Analyzer.Power(__Phase_S__, __Harmonic__),2));
    Terminal.Text(20, 47, CYAN, String(Energy_Analyzer.Power(__Phase_T__, __Harmonic__),2));

    Terminal.Text(18, 58, CYAN, String(Energy_Analyzer.Power(__Phase_R__, __Fundamental__),2));
    Terminal.Text(19, 58, CYAN, String(Energy_Analyzer.Power(__Phase_S__, __Fundamental__),2));
    Terminal.Text(20, 58, CYAN, String(Energy_Analyzer.Power(__Phase_T__, __Fundamental__),2));

    Terminal.Text(18, 69, CYAN, String(Energy_Analyzer.Power(__Phase_R__, __Harm_ReActive__),2));
    Terminal.Text(19, 69, CYAN, String(Energy_Analyzer.Power(__Phase_S__, __Harm_ReActive__),2));
    Terminal.Text(20, 69, CYAN, String(Energy_Analyzer.Power(__Phase_T__, __Harm_ReActive__),2));

    Terminal.Text(18, 80, CYAN, String(Energy_Analyzer.Power(__Phase_R__, __Fund_ReActive__),2));
    Terminal.Text(19, 80, CYAN, String(Energy_Analyzer.Power(__Phase_S__, __Fund_ReActive__),2));
    Terminal.Text(20, 80, CYAN, String(Energy_Analyzer.Power(__Phase_T__, __Fund_ReActive__),2));

    Terminal.Text(18, 91, CYAN, String(Energy_Analyzer.Power(__Phase_R__, __Fund_VA__),2));
    Terminal.Text(19, 91, CYAN, String(Energy_Analyzer.Power(__Phase_S__, __Fund_VA__),2));
    Terminal.Text(20, 91, CYAN, String(Energy_Analyzer.Power(__Phase_T__, __Fund_VA__),2));

    Terminal.Text(18, 103, CYAN, String(Energy_Analyzer.Power_Factor(__Phase_R__),2));
    Terminal.Text(19, 103, CYAN, String(Energy_Analyzer.Power_Factor(__Phase_S__),2));
    Terminal.Text(20, 103, CYAN, String(Energy_Analyzer.Power_Factor(__Phase_T__),2));
    Terminal.Text(20, 103, CYAN, String(Energy_Analyzer.Power_Factor(__Phase_Avg__),2));

    Terminal.Text(27, 17, CYAN, String(Energy_Analyzer.Get_Min_Value(1),2));
    Terminal.Text(28, 17, CYAN, String(Energy_Analyzer.Get_Max_Value(1),2));
    Terminal.Text(27, 28, CYAN, String(Energy_Analyzer.Get_Min_Value(2),2));
    Terminal.Text(28, 28, CYAN, String(Energy_Analyzer.Get_Max_Value(2),2));
    Terminal.Text(27, 39, CYAN, String(Energy_Analyzer.Get_Min_Value(3),2));
    Terminal.Text(28, 39, CYAN, String(Energy_Analyzer.Get_Max_Value(3),2));
    Terminal.Text(27, 50, CYAN, String(Energy_Analyzer.Get_Min_Value(4),2));
    Terminal.Text(28, 50, CYAN, String(Energy_Analyzer.Get_Max_Value(4),2));
    Terminal.Text(27, 61, CYAN, String(Energy_Analyzer.Get_Min_Value(5),2));
    Terminal.Text(28, 61, CYAN, String(Energy_Analyzer.Get_Max_Value(5),2));
    Terminal.Text(27, 72, CYAN, String(Energy_Analyzer.Get_Min_Value(6),2));
    Terminal.Text(28, 72, CYAN, String(Energy_Analyzer.Get_Max_Value(6),2));
    Terminal.Text(27, 83, CYAN, String(Energy_Analyzer.Get_Min_Value(7),2));
    Terminal.Text(28, 83, CYAN, String(Energy_Analyzer.Get_Max_Value(7),2));
    Terminal.Text(27, 94, CYAN, String(Energy_Analyzer.Get_Min_Value(8),2));
    Terminal.Text(28, 94, CYAN, String(Energy_Analyzer.Get_Max_Value(8),2));

    Terminal.Text(26, 116, CYAN, String(Energy_Analyzer.Frequency(),2));

    if (Energy_Analyzer.Status.Voltage.OV_VRMSA == true) {Terminal.Text(25, 139, CYAN, "X");} else {Terminal.Text(25, 139, CYAN, "-");}
    if (Energy_Analyzer.Status.Voltage.UN_VRMSA == true) {Terminal.Text(26, 139, CYAN, "X");} else {Terminal.Text(26, 139, CYAN, "-");}
    if (Energy_Analyzer.Status.Voltage.OV_VRMSB == true) {Terminal.Text(27, 139, CYAN, "X");} else {Terminal.Text(27, 139, CYAN, "-");}
    if (Energy_Analyzer.Status.Voltage.UN_VRMSB == true) {Terminal.Text(28, 139, CYAN, "X");} else {Terminal.Text(28, 139, CYAN, "-");}
    if (Energy_Analyzer.Status.Voltage.OV_VRMSC == true) {Terminal.Text(29, 139, CYAN, "X");} else {Terminal.Text(29, 139, CYAN, "-");}
    if (Energy_Analyzer.Status.Voltage.UN_VRMSC == true) {Terminal.Text(30, 139, CYAN, "X");} else {Terminal.Text(30, 139, CYAN, "-");}
    if (Energy_Analyzer.Status.Current.OV_IRMSA == true) {Terminal.Text(31, 139, CYAN, "X");} else {Terminal.Text(31, 139, CYAN, "-");}
    if (Energy_Analyzer.Status.Current.OV_IRMSB == true) {Terminal.Text(32, 139, CYAN, "X");} else {Terminal.Text(32, 139, CYAN, "-");}
    if (Energy_Analyzer.Status.Current.OV_IRMSC == true) {Terminal.Text(33, 139, CYAN, "X");} else {Terminal.Text(33, 139, CYAN, "-");}

    if (Energy_Analyzer.Status.Frequency.OV_Freq == true) {Terminal.Text(25, 156, CYAN, "X");} else {Terminal.Text(25, 156, CYAN, "-");}
    if (Energy_Analyzer.Status.Frequency.UN_Freq == true) {Terminal.Text(26, 156, CYAN, "X");} else {Terminal.Text(26, 156, CYAN, "-");}
    if (Energy_Analyzer.Status.PowerFactor.UN_PFA == true) {Terminal.Text(27, 156, CYAN, "X");} else {Terminal.Text(27, 156, CYAN, "-");}
    if (Energy_Analyzer.Status.PowerFactor.UN_PFB == true) {Terminal.Text(28, 156, CYAN, "X");} else {Terminal.Text(28, 156, CYAN, "-");}
    if (Energy_Analyzer.Status.PowerFactor.UN_PFC == true) {Terminal.Text(29, 156, CYAN, "X");} else {Terminal.Text(29, 156, CYAN, "-");}
    if (Energy_Analyzer.Status.SAG.VA_SAG == true) {Terminal.Text(30, 156, CYAN, "X");} else {Terminal.Text(30, 156, CYAN, "-");}
    if (Energy_Analyzer.Status.SAG.VB_SAG == true) {Terminal.Text(31, 156, CYAN, "X");} else {Terminal.Text(31, 156, CYAN, "-");}
    if (Energy_Analyzer.Status.SAG.VC_SAG == true) {Terminal.Text(32, 156, CYAN, "X");} else {Terminal.Text(32, 156, CYAN, "-");}
    if (Energy_Analyzer.Status.Imbalance.V_IMBAL == true) {Terminal.Text(33, 156, CYAN, "X");} else {Terminal.Text(33, 156, CYAN, "-");}

    Energy_Analyzer.Control_Limits();

    Terminal.Text(39, 15, YELLOW, String(Energy_Analyzer.Energy(__Phase_R__,__Active_Received__)));
    Terminal.Text(39, 26, YELLOW, String(Energy_Analyzer.Energy(__Phase_R__,__Active_Delivered__)));
    Terminal.Text(39, 37, YELLOW, String(Energy_Analyzer.Energy(__Phase_S__,__Active_Received__)));
    Terminal.Text(39, 48, YELLOW, String(Energy_Analyzer.Energy(__Phase_S__,__Active_Delivered__)));
    Terminal.Text(39, 59, YELLOW, String(Energy_Analyzer.Energy(__Phase_T__,__Active_Received__)));
    Terminal.Text(39, 70, YELLOW, String(Energy_Analyzer.Energy(__Phase_T__,__Active_Delivered__)));

    Terminal.Text(39, 82, YELLOW, String(Energy_Analyzer.Energy(__Phase_R__,__ReActive_Received__)));
    Terminal.Text(39, 95, YELLOW, String(Energy_Analyzer.Energy(__Phase_R__,__ReActive_Delivered__)));
    Terminal.Text(39, 108, YELLOW, String(Energy_Analyzer.Energy(__Phase_S__,__ReActive_Received__)));
    Terminal.Text(39, 121, YELLOW, String(Energy_Analyzer.Energy(__Phase_S__,__ReActive_Delivered__)));
    Terminal.Text(39, 134, YELLOW, String(Energy_Analyzer.Energy(__Phase_T__,__ReActive_Received__)));
    Terminal.Text(39, 147, YELLOW, String(Energy_Analyzer.Energy(__Phase_T__,__ReActive_Delivered__)));


}
