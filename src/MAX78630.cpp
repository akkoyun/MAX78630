/* *******************************************************************************
 *  Copyright (C) 2014-2020 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: MAX78630 Energy Analayser
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#include <MAX78630.h>

// --------------------------------------------------------------------------------------------------------------------------
// Register Locations
// --------------------------------------------------------------------------------------------------------------------------
// Use Word addresses for I2C and SPI interfaces and Byte addresses for the SSI (UART) protocol. 
// Nonvolatile (NV) register defaults are indicated with a ‘Y’. All other registers are initialized 
// as described in the Functional Description.
// --------------------------------------------------------------------------------------------------------------------------

// Register Address Definations
Register COMMAND 		{0x00, 0x00, 0, true};		// Selects modes, functions, or options
Register FW_VERSION 	{0x00, 0x03, 0, false};		// Hardware and firmware version
Register CONFIG			{0x00, 0x06, 0, true};		// Selects input configuration
Register SAMPLES 		{0x00, 0x09, 0, true};		// Minimum high-rate samples per accumulation interval
Register DIVISOR 		{0x00, 0x0C, 0, false};		// Actual samples in previous accumulation interval
Register CYCLE 			{0x00, 0x0F, 0, false};		// High-rate sample counter
Register FRAME 			{0x00, 0x12, 0, false};		// Low-rate sample counter

// Alarm Registers
Register STATUS			{0x00, 0x15, 0, false};		// Alarm and device status bits
Register STATUS_CLEAR	{0x00, 0x18, 0, false};		// Used to reset alarm/status bits
Register STATUS_SET		{0x00, 0x1B, 0, false};		// Used to set/force alarm/status bits
Register MASK1			{0x00, 0x1E, 0, true};		// Alarm/status mask for AL1 output pin
Register MASK2			{0x00, 0x21, 0, true};		// Alarm/status mask for AL2 output pin
Register MASK3			{0x00, 0x24, 0, true};		// Alarm/status mask for AL3 output pin
Register MASK4			{0x00, 0x27, 0, true};		// Alarm/status mask for AL4 output pin
Register MASK5			{0x00, 0x2A, 0, true};		// Alarm/status mask for AL5 output pin
Register STICKY			{0x00, 0x2D, 0, true};		// Alarm/status bits to hold until cleared by host

// DIO Registers
Register DIO_STATE		{0x00, 0x30, 0, false};		// State of DIO pins
Register DIO_DIR		{0x00, 0x33, 0, true};		// Direction of DIO pins. 1=Input ; 0=Output
Register DIO_POL		{0x00, 0x36, 0, true};		// Polarity of DIO pins. 1=Active High ; 0=Active Low

// Phase Compensation Registers
Register PHASECOMP1		{0x00, 0x42, 21, true};		// Phase compensation A
Register PHASECOMP2		{0x00, 0x45, 21, true};		// Phase compensation B
Register PHASECOMP3		{0x00, 0x48, 21, true};		// Phase compensation C

// Calibration Registers
Register I1_GAIN		{0x00, 0x54, 21, true};		// Current Gain Calibration
Register I2_GAIN		{0x00, 0x57, 21, true};		// Current Gain Calibration
Register I3_GAIN		{0x00, 0x5A, 21, true};		// Current Gain Calibration
Register V1_GAIN		{0x00, 0x5D, 21, true};		// Voltage Gain Calibration
Register V2_GAIN		{0x00, 0x60, 21, true};		// Voltage Gain Calibration
Register V3_GAIN		{0x00, 0x63, 21, true};		// Voltage Gain Calibration
Register I1_OFFS		{0x00, 0x66, 23, true};		// Current Offset Calibration
Register I2_OFFS		{0x00, 0x69, 23, true};		// Current Offset Calibration
Register I3_OFFS		{0x00, 0x6C, 23, true};		// Current Offset Calibration
Register V1_OFFS		{0x00, 0x6F, 23, true};		// Voltage Offset Calibration
Register V2_OFFS		{0x00, 0x72, 23, true};		// Voltage Offset Calibration
Register V3_OFFS		{0x00, 0x75, 23, true};		// Voltage Offset Calibration
Register T_GAIN			{0x00, 0x78, 0, true};		// Temperature Slope Calibration
Register T_OFFS			{0x00, 0x7B, 0, true};		// Temperature Offset Calibration
Register VSAG_INT		{0x00, 0x7E, 0, true};		// Voltage sag detect interval
Register V_IMB_MAX		{0x00, 0x81, 23, true};		// Voltage imbalance alarm limit
Register I_IMB_MAX		{0x00, 0x84, 23, true};		// Current imbalance alarm limit
Register CALCYCS		{0x00, 0x39, 0, true};		// Number of calibration cycles to average
Register HPF_COEF_I		{0x00, 0x3C, 23, true};		// Current input HPF coefficient
Register HPF_COEF_V		{0x00, 0x3F, 23, true};		// Voltage input HPF coefficient

// Scale Registers
Register IFSCALE		{0x01, 0x41, 0, true};		// Current Scale
Register VSCALE			{0x01, 0x44, 0, true};		// Voltage Scale

// Harmonic Selection Registers
Register HARM			{0x00, 0x4B, 0, true};		// Harmonic Selector, default: 1 

// Voltage Registers
Register VA				{0x00, 0x87, 23, false};	// Instantaneous Voltage A
Register VB				{0x00, 0x8A, 23, false};	// Instantaneous Voltage B
Register VC				{0x00, 0x8D, 23, false};	// Instantaneous Voltage C
Register VA_RMS			{0x00, 0x90, 23, false};	// RMS Voltage A
Register VB_RMS			{0x00, 0x93, 23, false};	// RMS Voltage B
Register VC_RMS			{0x00, 0x96, 23, false};	// RMS Voltage C
Register VT_RMS			{0x00, 0x99, 23, false};	// RMS Voltage average
Register VFUND_A		{0x00, 0x9C, 23, false};	// Fundamental Voltage A
Register VFUND_B		{0x00, 0x9F, 23, false};	// Fundamental Voltage B
Register VFUND_C		{0x00, 0xA2, 23, false};	// Fundamental Voltage C
Register VHARM_A		{0x00, 0xA5, 23, false};	// Harmonic Voltage A
Register VHARM_B		{0x00, 0xA8, 23, false};	// Harmonic Voltage B
Register VHARM_C		{0x00, 0xAB, 23, false};	// Harmonic Voltage C
Register V_TARGET		{0x00, 0xAE, 23, true};		// Calibration Target for Voltages
Register VRMS_MIN		{0x00, 0xB1, 23, true};		// Voltage lower alarm limit
Register VRMS_MAX		{0x00, 0xB4, 23, true};		// Voltage upper alarm limit
Register VSAG_LIM		{0x00, 0xB7, 23, true};		// RMS Voltage Sag threshold

// Frequency Registers
Register FREQ			{0x01, 0x80, 16, false};	// Line Frequency
Register F_MIN			{0x01, 0x83, 16, true};		// Frequency Alarm Lower Limit
Register F_MAX			{0x01, 0x86, 16, true};		// Frequency Alarm Upper Limit

// Current Registers
Register IA				{0x00, 0xBA, 23, false};	// Instantaneous Current A
Register IB				{0x00, 0xBD, 23, false};	// Instantaneous Current B
Register IC				{0x00, 0xC0, 23, false};	// Instantaneous Current C
Register IARMS_OFF		{0x00, 0xC3, 23, true};		// RMS Current dynamic offset adjust A
Register IBRMS_OFF		{0x00, 0xC6, 23, true};		// RMS Current dynamic offset adjust B
Register ICRMS_OFF		{0x00, 0xC9, 23, true};		// RMS Current dynamic offset adjust C
Register IA_PEAK		{0x00, 0xCC, 23, false};	// Peak Current A
Register IB_PEAK		{0x00, 0xCF, 23, false};	// Peak Current B
Register IC_PEAK		{0x00, 0xD2, 23, false};	// Peak Current C
Register IA_RMS			{0x00, 0xD5, 23, false};	// RMS Current A
Register IB_RMS			{0x00, 0xD8, 23, false};	// RMS Current B
Register IC_RMS			{0x00, 0xDB, 23, false};	// RMS Current C
Register IT_RMS			{0x00, 0xDE, 23, false};	// RMS Current average
Register IFUND_A		{0x00, 0xE1, 23, false};	// Fundamental Current A
Register IFUND_B		{0x00, 0xE4, 23, false};	// Fundamental Current B
Register IFUND_C		{0x00, 0xE7, 23, false};	// Fundamental Current C
Register IHARM_A		{0x00, 0xEA, 23, false};	// Harmonic Current A
Register IHARM_B		{0x00, 0xED, 23, false};	// Harmonic Current B
Register IHARM_C		{0x00, 0xF0, 23, false};	// Harmonic Current C
Register IRMS_MAX		{0x00, 0xF3, 23, true};		// Current upper alarm limit
Register I_TARGET		{0x00, 0xF6, 23, true};		// Calibration Target for Currents

// Power Registers
Register QFUND_A		{0x00, 0xF9, 23, false};	// Fundamental Reactive Power A
Register QFUND_B		{0x00, 0xFC, 23, false};	// Fundamental Reactive Power B
Register QFUND_C		{0x00, 0xFF, 23, false};	// Fundamental Reactive Power C
Register QHARM_A		{0x01, 0x02, 23, false};	// Harmonic Reactive Power A
Register QHARM_B		{0x01, 0x05, 23, false};	// Harmonic Reactive Power B
Register QHARM_C		{0x01, 0x08, 23, false};	// Harmonic Reactive Power C
Register QA_OFFS		{0x01, 0x0B, 23, true};		// Reactive Power dynamic offset adjust A
Register QB_OFFS		{0x01, 0x0E, 23, true};		// Reactive Power dynamic offset adjust B
Register QC_OFFS		{0x01, 0x11, 23, true};		// Reactive Power dynamic offset adjust C
Register PA_OFFS		{0x01, 0x14, 23, true};		// Active Power dynamic offset adjust A
Register PB_OFFS		{0x01, 0x17, 23, true};		// Active Power dynamic offset adjust B
Register PC_OFFS		{0x01, 0x1A, 23, true};		// Active Power dynamic offset adjust C
Register WATT_A			{0x01, 0x1D, 23, false};	// Active Power A
Register WATT_B			{0x01, 0x20, 23, false};	// Active Power B
Register WATT_C			{0x01, 0x23, 23, false};	// Active Power C
Register VAR_A			{0x01, 0x26, 23, false};	// Reactive Power A
Register VAR_B			{0x01, 0x29, 23, false};	// Reactive Power B
Register VAR_C			{0x01, 0x2C, 23, false};	// Reactive Power C
Register VA_A			{0x01, 0x2F, 23, false};	// Apparent Power A
Register VA_B			{0x01, 0x32, 23, false};	// Apparent Power B
Register VA_C			{0x01, 0x35, 23, false};	// Apparent Power C
Register WATT_T			{0x01, 0x38, 23, false};	// Active Power average
Register VAR_T			{0x01, 0x3B, 23, false};	// Reactive Power average
Register VA_T			{0x01, 0x3E, 23, false};	// Apparent Power average
Register PFUND_A		{0x01, 0x4A, 23, false};	// Fundamental Power A
Register PFUND_B		{0x01, 0x4D, 23, false};	// Fundamental Power B
Register PFUND_C		{0x01, 0x50, 23, false};	// Fundamental Power C
Register PHARM_A		{0x01, 0x53, 23, false};	// Harmonic Power A
Register PHARM_B		{0x01, 0x56, 23, false};	// Harmonic Power B
Register PHARM_C		{0x01, 0x59, 23, false};	// Harmonic Power C
Register VAFUNDA		{0x01, 0x5C, 23, false};	// Fundamental Volt Amperes A
Register VAFUNDB		{0x01, 0x5F, 23, false};	// Fundamental Volt Amperes B
Register VAFUNDC		{0x01, 0x62, 23, false};	// Fundamental Volt Amperes C

// Power Factor Registers
Register PFA			{0x01, 0x65, 22, false};	// Power Factor A
Register PFB			{0x01, 0x68, 22, false};	// Power Factor B
Register PFC			{0x01, 0x6B, 22, false};	// Power Factor C
Register PF_T			{0x01, 0x6E, 22, false};	// T otal Power Factor
Register PF_MIN			{0x01, 0x71, 22, true};		// Power Factor lower alarm limit

// Temperature Registetrs
Register TEMPC			{0x01, 0x74, 10, false};	// Chip Temperature
Register T_TARGET		{0x01, 0x77, 10, true};		// Temperature calibration target
Register T_MIN			{0x01, 0x7A, 10, true};		// Temperature Alarm Lower Limit
Register T_MAX			{0x01, 0x7D, 10, true};		// Temperature Alarm Upper Limit

// Energy Registers
Register BUCKET_LOW		{0x01, 0xD1, 0, true};		// Energy Bucket Size – Low word
Register BUCKET_HIGH	{0x01, 0xD4, 0, true};		// Energy Bucket Size – High word
Register WHA_POS		{0x01, 0xDD, 0, false};		// Received Active Energy Counter A
Register WHA_NEG		{0x01, 0xE6, 0, false};		// Delivered Active Energy Counter A
Register WHB_POS		{0x01, 0xEF, 0, false};		// Received Active Energy Counter B
Register WHB_NEG		{0x01, 0xF8, 0, false};		// Delivered Active Energy Counter B
Register WHC_POS		{0x02, 0x01, 0, false};		// Received Active Energy Counter C
Register WHC_NEG		{0x02, 0x0A, 0, false};		// Delivered Active Energy Counter C
Register VARHA_POS		{0x02, 0x13, 0, false};		// Reactive Energy Leading Counter A
Register VARHA_NEG		{0x02, 0x1C, 0, false};		// Reactive Energy Lagging Counter A
Register VARHB_POS		{0x02, 0x25, 0, false};		// Reactive Energy Leading Counter B
Register VARHB_NEG		{0x02, 0x2E, 0, false};		// Reactive Energy Lagging Counter B
Register VARHC_POS		{0x02, 0x37, 0, false};		// Reactive Energy Leading Counter C
Register VARHC_NEG		{0x02, 0x40, 0, false};		// Reactive Energy Lagging Counter C

// Recorded MIN/MAX Registers
Register MMADDR0		{0x01, 0xB9, 0, true};		// Min/Max Monitor address 1
Register MIN0			{0x01, 0x89, 0, false};		// Minimum Recorded Value 1
Register MAX0			{0x01, 0xA1, 0, false};		// Maximum Recorded Value 1
Register MMADDR1		{0x01, 0xBC, 0, true};		// Min/Max Monitor address 2
Register MIN1			{0x01, 0x8C, 0, false};		// Minimum Recorded Value 2
Register MAX1			{0x01, 0xA4, 0, false};		// Maximum Recorded Value 2
Register MMADDR2		{0x01, 0xBF, 0, true};		// Min/Max Monitor address 3
Register MIN2			{0x01, 0x8F, 0, false};		// Minimum Recorded Value 3
Register MAX2			{0x01, 0xA7, 0, false};		// Maximum Recorded Value 3
Register MMADDR3		{0x01, 0xC2, 0, true};		// Min/Max Monitor address 4
Register MIN3			{0x01, 0x92, 0, false};		// Minimum Recorded Value 4
Register MAX3			{0x01, 0xAA, 0, false};		// Maximum Recorded Value 4
Register MMADDR4		{0x01, 0xC5, 0, true};		// Min/Max Monitor address 5
Register MIN4			{0x01, 0x95, 0, false};		// Minimum Recorded Value 5
Register MAX4			{0x01, 0xAD, 0, false};		// Maximum Recorded Value 5
Register MMADDR5		{0x01, 0xC8, 0, true};		// Min/Max Monitor address 6
Register MIN5			{0x01, 0x98, 0, false};		// Minimum Recorded Value 6
Register MAX5			{0x01, 0xB0, 0, false};		// Maximum Recorded Value 6
Register MMADDR6		{0x01, 0xCB, 0, true};		// Min/Max Monitor address 7
Register MIN6			{0x01, 0x9B, 0, false};		// Minimum Recorded Value 7
Register MAX6			{0x01, 0xB3, 0, false};		// Maximum Recorded Value 7
Register MMADDR7		{0x01, 0xCE, 0, true};		// Min/Max Monitor address 8
Register MIN7			{0x01, 0x9E, 0, false};		// Minimum Recorded Value 8
Register MAX7			{0x01, 0xB6, 0, false};		// Maximum Recorded Value 8

// Device Registers
Register DEVADDR		{0x00, 0x4E, 0, true};		// High order address bits for I2C and UART interfaces
Register BAUD			{0x00, 0x51, 0, true};		// Baud rate for UART interface
Register SYSSTAT		{0x02, 0x43, 0, false};		// Bit 23 is a sticky register with status of any SPI Errors

// Begin Functions
bool MAX78630::Begin(void) {

	// Start Serial Communication
	Energy_Serial.begin(Energy_Serial_Baud);

	// Clear Serial Buffer
	_Clear_Buffer();

	/*
		[7]-[6]-[5]-[4]-[3]-[2]-[1]-[0]
		[0]-[0]-[0]-[0]-[0]-[0]-[1]-[1]
         |   |   |   |   |   |   |   |
         |   |   |   |   |   |   |   *----------- ADDR0 Pin [0]
         |   |   |   |   |   |   *--------------- ADDR1 Pin [1]
         *---*---*---*---*---*------------------- DEVADDR [2:7]

		SSI ID = Device Address + 1
	*/		

	// Select IC Command
	Energy_Serial.write(0xAA);			// Header (0xAA)
	Energy_Serial.write(0x04);			// Total Sended Byte (0x04)
	Energy_Serial.write(0xC4);			// Setting Command (0xC4)
	Energy_Serial.write(0x8E);			// CheckSum (0x8E)

	// Command Delay
	delay(10);

	// Clear Serial Buffer
	_Clear_Buffer();

	/*
		[7]-[6]-[5]-[4]-[3]-[2]-[1]-[0]
		[0]-[0]-[1]-[1]-[0]-[0]-[0]-[0]
         |   |   |   |   |   |   |   |
         |   |   |   |   *---*---*---*------- Reserved
         |   |   |   *----------------------- Temperature Compensation. Should be set to “1” for proper operation.
         |   |   *--------------------------- Line Lock 1= lock to line cycle; 0= independent
         |   *------------------------------- 1= reset the minima and maxima registers for all monitored variables. This bit automatically clears to zero when the reset completes.
         *----------------------------------- 1= reset all energy accumulators. This bit automatically clears to zero when the reset completes. 
	*/		

	// User Register Defination
	uint8_t _Config_Reg = 0x00;
	
	// User Register Bit Definations
	bool _Config_Reg_Bits[8] = {false, false, false, false, false, false, false, false};

	// Set Configuration
	_Config_Reg_Bits[7] = false;	// 1= reset all energy accumulators. This bit automatically clears to zero when the reset completes. 
	_Config_Reg_Bits[6] = false;	// 1= reset the minima and maxima registers for all monitored variables. This bit automatically clears to zero when the reset completes.
	_Config_Reg_Bits[5] = true;		// Line Lock 1= lock to line cycle; 0= independent
	_Config_Reg_Bits[4] = true;		// Temperature Compensation. Should be set to “1” for proper operation.

	// Set Config Register
	if (_Config_Reg_Bits[0] == true) {_Config_Reg |= 0b00000001;} else {_Config_Reg &= 0b11111110;}	// User Register Bit 0
	if (_Config_Reg_Bits[1] == true) {_Config_Reg |= 0b00000010;} else {_Config_Reg &= 0b11111101;}	// User Register Bit 1
	if (_Config_Reg_Bits[2] == true) {_Config_Reg |= 0b00000100;} else {_Config_Reg &= 0b11111011;}	// User Register Bit 2
	if (_Config_Reg_Bits[3] == true) {_Config_Reg |= 0b00001000;} else {_Config_Reg &= 0b11110111;}	// User Register Bit 3
	if (_Config_Reg_Bits[4] == true) {_Config_Reg |= 0b00010000;} else {_Config_Reg &= 0b11101111;}	// User Register Bit 4
	if (_Config_Reg_Bits[5] == true) {_Config_Reg |= 0b00100000;} else {_Config_Reg &= 0b11011111;}	// User Register Bit 5
	if (_Config_Reg_Bits[6] == true) {_Config_Reg |= 0b01000000;} else {_Config_Reg &= 0b10111111;}	// User Register Bit 6
	if (_Config_Reg_Bits[7] == true) {_Config_Reg |= 0b10000000;} else {_Config_Reg &= 0b01111111;}	// User Register Bit 7

	// Calculate CheckSum
	uint8_t _Calibration_CheckSum = 0x100 - ((0xAA + 0x07 + 0xCA + 0x65 + 0xFF + _Config_Reg) % 256);

	// Calibration Command
	Energy_Serial.write(0xAA);						// Header (0xAA)
	Energy_Serial.write(0x07);						// Total Sended Byte (0x07)
	Energy_Serial.write(0xCA);						// Setting Command (0xCA)
	Energy_Serial.write(_Config_Reg);				// Config Register (0x)
	Energy_Serial.write(0xFF);						// Calibration Setting (0xFF)
	Energy_Serial.write(0x65);						// Setting Command (0x65)
	Energy_Serial.write(_Calibration_CheckSum);		// CheckSum (0xD1)

	// Command Delay
	delay(10);

	// VScale Set Command
	Set_VScale(667);

	// IScale Set Command
	Set_IScale(7.16);

	// Get Scales
	_VScale = _Register_Pointer_Read(VSCALE);
	_IScale = _Register_Pointer_Read(IFSCALE);

	// Close UART Connection
	Energy_Serial.end();
	
	// Start Console
	VT100.begin(Serial);
	VT100.cursorOff();

	// Print Diagnostic Abstract
	VT100.clearAll();
	VT100_Base();

}

// Set Scale Registers
bool MAX78630::Set_VScale(uint32_t _VScale) {

	// Set Register
	_Register_Pointer_Set(VSCALE, _VScale);

	// End Function
	return(true);

}
bool MAX78630::Set_IScale(uint32_t _IScale) {

	// Set Register
	_Register_Pointer_Set(IFSCALE, _IScale);

	// End Function
	return(true);

}
bool MAX78630::Set_Harmonic(uint32_t _Harmonic) {

	// Set Register
	_Register_Pointer_Set(HARM, _Harmonic);

	// End Function
	return(true);

}

// Voltage Measurements
float MAX78630::Voltage_RMS(char Phase) {

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(VA_RMS); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(VB_RMS); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(VC_RMS); // Measure Phase T
	if (Phase == 'A') _Result = _Register_Pointer_Read(VT_RMS); // Measure Phase Average
	
	// End Function
	return(_Result * _VScale);
	
}
float MAX78630::Voltage_Instantaneous(char Phase) {

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(VA); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(VB); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(VC); // Measure Phase T
	
	// End Function
	return(_Result * _VScale);

}
float MAX78630::Voltage_Fundamental(char Phase) {

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(VFUND_A); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(VFUND_B); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(VFUND_C); // Measure Phase T
	
	// End Function
	return(_Result * _VScale);

}
float MAX78630::Voltage_Harmonic(char Phase) {

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(VHARM_A); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(VHARM_B); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(VHARM_C); // Measure Phase T
	
	// End Function
	return(_Result * _VScale);

}
float MAX78630::Voltage_RMS_Min(void) {

	// Declare Variable
	float _Result = 0;

	_Result = _Register_Pointer_Read(VRMS_MIN); // Measure Phase R
	
	// End Function
	return(_Result * _VScale);

}
float MAX78630::Voltage_RMS_Max(void) {

	// Declare Variable
	float _Result = 0;

	_Result = _Register_Pointer_Read(VRMS_MAX); // Measure Phase R
	
	// End Function
	return(_Result * _VScale);

}
float MAX78630::Voltage_SAG_Limit(void) {

	// Declare Variable
	float _Result = 0;

	_Result = _Register_Pointer_Read(VSAG_LIM); // Measure Phase R
	
	// End Function
	return(_Result * _VScale);

}

// Current Measurements
float MAX78630::Current_RMS(char Phase) {

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(IA_RMS); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(IA_RMS); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(IA_RMS); // Measure Phase T
	if (Phase == 'A') _Result = _Register_Pointer_Read(IT_RMS); // Measure Phase Average
	
	// End Function
	return(_Result * _IScale);
	
}
float MAX78630::Current_Instantaneous(char Phase) {

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(IA); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(IB); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(IC); // Measure Phase T
	
	// End Function
	return(_Result * _IScale);

}
float MAX78630::Current_Peak(char Phase) {

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(IA_PEAK); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(IB_PEAK); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(IC_PEAK); // Measure Phase T
	
	// End Function
	return(_Result * _IScale);

}
float MAX78630::Current_Fundamental(char Phase) {

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(IFUND_A); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(IFUND_B); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(IFUND_C); // Measure Phase T
	
	// End Function
	return(_Result * _IScale);

}
float MAX78630::Current_Harmonic(char Phase) {

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(IHARM_A); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(IHARM_B); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(IHARM_C); // Measure Phase T
	
	// End Function
	return(_Result * _IScale);

}
float MAX78630::Current_RMS_Max(void) {

	// Declare Variable
	float _Result = 0;

	_Result = _Register_Pointer_Read(IRMS_MAX); // Measure Phase R
	
	// End Function
	return(_Result * _IScale);

}

// Power Measurements
float MAX78630::Active_Power(char Phase) {

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(WATT_A); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(WATT_B); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(WATT_C); // Measure Phase T
	if (Phase == 'A') _Result = _Register_Pointer_Read(WATT_T); // Measure Phase Average
	
	// End Function
	return(_Result * _IScale * _VScale);
	
}
float MAX78630::ReActive_Power(char Phase) {

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(VAR_A); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(VAR_B); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(VAR_C); // Measure Phase T
	if (Phase == 'A') _Result = _Register_Pointer_Read(VAR_T); // Measure Phase Average
	
	// End Function
	return(_Result * _IScale * _VScale);
	
}
float MAX78630::Apparent_Power(char Phase) {

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(VA_A); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(VA_B); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(VA_C); // Measure Phase T
	if (Phase == 'A') _Result = _Register_Pointer_Read(VA_T); // Measure Phase Average
	
	// End Function
	return(_Result * _IScale * _VScale);
	
}
float MAX78630::Fundamental_Power(char Phase) {

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(PFUND_A); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(PFUND_B); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(PFUND_C); // Measure Phase T
	
	// End Function
	return(_Result * _IScale * _VScale);
	
}
float MAX78630::Harmonic_Power(char Phase) {

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(PHARM_A); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(PHARM_B); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(PHARM_C); // Measure Phase T
	
	// End Function
	return(_Result * _IScale * _VScale);
	
}
float MAX78630::Power_Factor(char Phase) {

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(PFA); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(PFB); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(PFC); // Measure Phase T
	if (Phase == 'A') _Result = _Register_Pointer_Read(PF_T); // Measure Phase Average
	
	// End Function
	return(_Result);
	
}

// Temperature Measurements
float MAX78630::IC_Temperature(void) {

	// Declare Variable
	float _Result = 0;

	// Get Data
	_Result = _Register_Pointer_Read(TEMPC);

	// End Function
	return(_Result);
	
}

// Frequency Measurements
float MAX78630::Frequency(void) {

	// Declare Variable
	float _Result = 0;

	// Get Data
	_Result = _Register_Pointer_Read(FREQ);

	// End Function
	return(_Result);
	
}














bool MAX78630::_Register_Pointer_Set(Register _Command, uint32_t _Data) {

	// Clear Buffer
	_Clear_Buffer();

	// Convert Parameters
	char _Parameter1 = _Data;
	char _Parameter2 = _Data>>8;
	char _Parameter3 = _Data>>16;

	// Calculate CheckSum
	uint8_t ChkS = 0x100 - ((0xAA + 0x0A + 0xA3 + _Command.Low_Address + _Command.High_Address + 0xD3 + _Parameter1 + _Parameter2 + _Parameter3) % 256); // Calculate checksum

	// Clear Serial Buffer
	_Clear_Buffer();

	// Send Command
	Energy_Serial.write(0xAA);
	Energy_Serial.write(0x0A);
	Energy_Serial.write(0xA3);
	Energy_Serial.write(_Command.Low_Address);
	Energy_Serial.write(_Command.High_Address);
	Energy_Serial.write(0xD3);
	Energy_Serial.write(_Parameter1);
	Energy_Serial.write(_Parameter2);
	Energy_Serial.write(_Parameter3);
	Energy_Serial.write(ChkS);

	// Command Delay
	delay(20);

	// End Function
	return(true);

}
double MAX78630::_Register_Pointer_Read(Register _Command) {
	
	/*
		MAX78630 Serial Comminication Read Values Structure
		---------------------------------------------------
		Read Request : [1]-[2]-[3]-[4]-[5]-[6]-[7]
		---------------------------------------------------
		1. byte is the IC address byte (0xAA)
		2. byte is the total sended byte (0x07)
		3. byte is the package payload type (0xA3)
		4. byte is the request (RMS) byte (2)
		5. byte is the request (RMS) byte (1)
		6. byte is the requested byte count (0xE3)
		7. byte is the CRC correction byte (CHK)
	*/

	// Start Serial Connection
	Energy_Serial.begin(Energy_Serial_Baud);

	// Clear Serial Buffer
	_Clear_Buffer();

	// Calculate CheckSum
	uint8_t _Request_CheckSum = 0x100 - ((0xAA + 0x07 + 0xA3 + _Command.Low_Address + _Command.High_Address + 0xE3) % 256);

	// Send Command
	Energy_Serial.write(0xAA);
	Energy_Serial.write(0x07);
	Energy_Serial.write(0xA3);
	Energy_Serial.write(_Command.Low_Address);
	Energy_Serial.write(_Command.High_Address);
	Energy_Serial.write(0xE3);
	Energy_Serial.write(_Request_CheckSum);

	// Command Send Delay
	delay(20);

	// Declare Variable
	uint8_t _Response[6];
	memset(_Response, '\0', 6);
	uint8_t _Response_Order = 0;

	// Read UART Response
	while(Energy_Serial.available() > 0) {

		// Read Serial Char
		_Response[_Response_Order] = Energy_Serial.read();
		
		// Increase Read Order
		_Response_Order++;
		
		// Stream Delay
		delay(5);
		
	}

	// Calculate Response CheckSum
	uint8_t _Response_CheckSum = 0x100 - ((_Response[0] + _Response[1] + _Response[2] + _Response[3] + _Response[4]) % 256);

	// Declare Raw Data Variable
	uint32_t _Data_RAW = 0;
	double _Data_SUM = 0;

	// Control Recieved Data
	if (_Response[0] == 0xAA and _Response[1] == 0x06 and _Response[5] == _Response_CheckSum) {

		// Combine Read Bytes
		_Data_RAW = ((uint32_t)(_Response[4]) << 16 | ((uint32_t)_Response[3]) << 8 | ((uint32_t)_Response[2]));

		// Calculate Response
		if (_Command.Data_Type == 0) {
			
			_Data_SUM = _Data_RAW;
			
		} else {

			// Handle Bits
			for (uint8_t i = 0; i < 23; i++) {

				if (bitRead(_Data_RAW, i) == true) _Data_SUM += pow(2, (i - _Command.Data_Type));

			}
			if (bitRead(_Data_RAW, 23) == true) _Data_SUM += -1 * pow(2, (23 - _Command.Data_Type));

		}

	}

	// Close UART Connection
	Energy_Serial.end();

	// End Function
	return(_Data_SUM);

}
void MAX78630::_Clear_Buffer(void) {

	// Clear UART Buffer
	Energy_Serial.flush(); while(Energy_Serial.available() > 0) Energy_Serial.read(); delay(5);

}
void MAX78630::VT100_Base(void) {

	VT100.setTextColor(VT_WHITE); 

	VT100.setCursor(1, 1); Serial.print("|");
	for (size_t i = 2; i < 126; i++) {VT100.setCursor(1, i); Serial.print(F("-"));}
	VT100.setCursor(1, 126); Serial.print("|");

	VT100.setCursor(2, 1); Serial.print(F("|"));
	VT100.setCursor(2, 126); Serial.print(F("|"));

	VT100.setCursor(3, 1); Serial.print("|");
	for (size_t i = 2; i < 126; i++) {VT100.setCursor(3, i); Serial.print(F("-"));}
	VT100.setCursor(3, 126); Serial.print("|");

	VT100.setCursor(2, 51); Serial.print("3 Phase Power Analayser");

	for (size_t i = 2; i < 126; i++) {VT100.setCursor(5, i); Serial.print(F("-"));}

	for (size_t i = 3; i < 22; i++) {
		VT100.setCursor(i, 1); Serial.print(F("|"));
		VT100.setCursor(i, 126); Serial.print(F("|"));		
	}

	for (size_t i = 3; i < 22; i++) {
		VT100.setCursor(i, 40); Serial.print(F("|"));
	}

	VT100.setCursor(4, 3); Serial.print("Voltage Measurements");

	VT100.setCursor(6, 3); Serial.print("R Phase Instant Voltage..[        V]");
	VT100.setCursor(7, 3); Serial.print("S Phase Instant Voltage..[        V]");
	VT100.setCursor(8, 3); Serial.print("T Phase Instant Voltage..[        V]");
	for (size_t i = 2; i < 40; i++) {VT100.setCursor(9, i); Serial.print(F("-"));}
	VT100.setCursor(10, 3); Serial.print("R Phase RMS Voltage......[        V]");
	VT100.setCursor(11, 3); Serial.print("S Phase RMS Voltage......[        V]");
	VT100.setCursor(12, 3); Serial.print("T Phase RMS Voltage......[        V]");
	for (size_t i = 2; i < 40; i++) {VT100.setCursor(13, i); Serial.print(F("-"));}
	VT100.setCursor(14, 3); Serial.print("R Phase Fund Voltage.....[        V]");
	VT100.setCursor(15, 3); Serial.print("S Phase Fund Voltage.....[        V]");
	VT100.setCursor(16, 3); Serial.print("T Phase Fund Voltage.....[        V]");
	for (size_t i = 2; i < 40; i++) {VT100.setCursor(17, i); Serial.print(F("-"));}
	VT100.setCursor(18, 3); Serial.print("R Phase Harm Voltage.....[        V]");
	VT100.setCursor(19, 3); Serial.print("S Phase Harm Voltage.....[        V]");
	VT100.setCursor(20, 3); Serial.print("T Phase Harm Voltage.....[        V]");
	for (size_t i = 2; i < 40; i++) {VT100.setCursor(21, i); Serial.print(F("-"));}

	VT100.setCursor(4, 42); Serial.print("Current Measurements");

	VT100.setCursor(6, 43); Serial.print("R Phase Instant Current..[        A]");
	VT100.setCursor(7, 43); Serial.print("S Phase Instant Current..[        A]");
	VT100.setCursor(8, 43); Serial.print("T Phase Instant Current..[        A]");
	for (size_t i = 41; i < 80; i++) {VT100.setCursor(9, i); Serial.print(F("-"));}
	VT100.setCursor(10, 43); Serial.print("R Phase RMS Current......[        A]");
	VT100.setCursor(11, 43); Serial.print("S Phase RMS Current......[        A]");
	VT100.setCursor(12, 43); Serial.print("T Phase RMS Current......[        A]");
	for (size_t i = 41; i < 80; i++) {VT100.setCursor(13, i); Serial.print(F("-"));}
	VT100.setCursor(14, 43); Serial.print("R Phase Fund Current.....[        A]");
	VT100.setCursor(15, 43); Serial.print("S Phase Fund Current.....[        A]");
	VT100.setCursor(16, 43); Serial.print("T Phase Fund Current.....[        A]");
	for (size_t i = 41; i < 80; i++) {VT100.setCursor(17, i); Serial.print(F("-"));}
	VT100.setCursor(18, 43); Serial.print("R Phase Harm Current.....[        A]");
	VT100.setCursor(19, 43); Serial.print("S Phase Harm Current.....[        A]");
	VT100.setCursor(20, 43); Serial.print("T Phase Harm Current.....[        A]");
	for (size_t i = 41; i < 80; i++) {VT100.setCursor(21, i); Serial.print(F("-"));}



/*
	//VT100.setCursor(52, 77); UART_Terminal.print(F("RTC Interval :"));
	VT100.setCursor(52, 104); UART_Terminal.print(F("Send Counter :"));

	VT100.setTextColor(VT_YELLOW); 
	VT100.setCursor(54, 3); UART_Terminal.print(F("[ ] Environment Update / [ ] Battery Update / [ ] Start Pump / [ ] Stop Pump / [ ] Reset Device / [ ] Clear Console"));
	VT100.setTextColor(VT_MAGENTA); 
	VT100.setCursor(54, 4); UART_Terminal.print(F("1"));
	VT100.setCursor(54, 29); UART_Terminal.print(F("2"));
	VT100.setCursor(54, 50); UART_Terminal.print(F("I"));
	VT100.setCursor(54, 67); UART_Terminal.print(F("O"));
	VT100.setCursor(54, 83); UART_Terminal.print(F("R"));
	VT100.setCursor(54, 102); UART_Terminal.print(F("C"));
*/
}

// Define Library Class
MAX78630 Energy_Analayser;

// 1903