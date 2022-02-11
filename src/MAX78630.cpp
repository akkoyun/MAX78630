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
Register CONFIG			{0x00, 0x06, 0, true};		// Selects input configuration

// Calibration Registers
Register VSAG_INT		{0x00, 0x7E, 0, true};		// Voltage sag detect interval
Register CALCYCS		{0x00, 0x39, 0, true};		// Number of calibration cycles to average

// Target Registers
Register V_TARGET		{0x00, 0xAE, 23, true};		// Calibration Target for Voltages
Register I_TARGET		{0x00, 0xF6, 23, true};		// Calibration Target for Currents
Register T_TARGET		{0x01, 0x77, 10, true};		// Temperature calibration target

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

	// VScale Set Command - 667
	VScale(_V_Scale);

	// IScale Set Command - 7
	IScale(_C_Scale);

	// Bucket Set Command
	Bucket(true, _BUCKET_HIGH, _BUCKET_LOW);

	// Set Limit Parameters
	Set_Limit(1, _Temp_Max);
	Set_Limit(2, _Temp_Min);
	Set_Limit(3, _Fq_Max);
	Set_Limit(4, _Fq_Min);
	Set_Limit(5, (_V_RMS_Max));
	Set_Limit(6, (_V_RMS_Min));
	Set_Limit(7, _C_Max);
	Set_Limit(8, _PF_Min);
	Set_Limit(9, _V_Max_Imb);
	Set_Limit(10, _C_Max_Imb);

	// Control for Limits
	Control_Limits();

	// Close UART Connection
	Energy_Serial.end();
	
}

// Device Functions
uint32_t MAX78630::Get_Baud(void) {

	// Define Objects
	Register BAUD			{0x00, 0x51, 0, true};		// Baud rate for UART interface

	// Declare Variable
	uint32_t _Result = 0;

	// Decide Command
	_Result = _Register_Pointer_Read(BAUD);
	
	// End Function
	return(_Result);

}
bool MAX78630::Set_Baud(uint32_t _Baud) {

	// Define Objects
	Register BAUD			{0x00, 0x51, 0, true};		// Baud rate for UART interface

	// Declare Variable
	uint32_t _Result = 0;

	// Set Command
	_Result = _Register_Pointer_Set(BAUD, _Baud);
	
	// End Function
	return(_Result);

}
uint32_t MAX78630::Get_Device_Address(void) {

	// Define Objects
	Register DEVADDR		{0x00, 0x4E, 0, true};		// High order address bits for I2C and UART interfaces

	// Declare Variable
	uint32_t _Result = 0;

	// Decide Command
	_Result = _Register_Pointer_Read(DEVADDR);
	
	// End Function
	return(_Result);

}
uint32_t MAX78630::Get_System_Stat(void) {

	// Define Objects
	Register SYSSTAT		{0x02, 0x43, 0, false};		// Bit 23 is a sticky register with status of any SPI Errors

	// Declare Variable
	uint32_t _Result = 0;

	// Decide Command
	_Result = _Register_Pointer_Read(SYSSTAT);
	
	// End Function
	return(_Result);

}
uint32_t MAX78630::Get_Firmware(void) {

	// Define Objects
	Register FW_VERSION 	{0x00, 0x03, 0, false};		// Hardware and firmware version

	// Declare Variable
	uint32_t _Result = 0;

	// Decide Command
	_Result = _Register_Pointer_Read(FW_VERSION);
	
	// End Function
	return(_Result);

}

// Set Scale Registers
uint16_t MAX78630::VScale(uint32_t _Voltage_Scale) {

	// Define Objects
	Register VSCALE			{0x01, 0x44, 0, true};		// Voltage Scale

	// Decide Action
	if (_Voltage_Scale == 0) {
		
		// Read Register
		_VScale = _Register_Pointer_Read(VSCALE);

	} else {

		// Set Register
		if (_Register_Pointer_Set(VSCALE, _Voltage_Scale)) {
			
			// Set Variable
			_VScale = _Voltage_Scale;

		} else {

			// Set Variable
			_VScale = 0;

		}

	}
	
	// End Function
	return(_VScale);

}
uint16_t MAX78630::IScale(uint32_t _Current_Scale) {

	// Define Objects
	Register IFSCALE		{0x01, 0x41, 0, true};		// Current Scale

	// Decide Action
	if (_Current_Scale == 0) {
		
		// Read Register
		_IScale = _Register_Pointer_Read(IFSCALE);

	} else {

		// Set Register
		if (_Register_Pointer_Set(IFSCALE, _Current_Scale)) {
			
			// Set Variable
			_IScale = _Current_Scale;
			
		} else {

			// Set Variable
			_IScale = 0;

		}

	}
	
	// End Function
	return(_IScale);

}
uint8_t MAX78630::Harmonic(uint32_t _Harmonic) {

	// Define Objects
	Register HARM			{0x00, 0x4B, 0, true};		// Harmonic Selector, default: 1 

	// Declare Variable
	uint32_t _Result = 0;

	// Decide Action
	if (_Harmonic == 0) {
		
		// Read Register
		_Result = _Register_Pointer_Read(HARM);

	} else {

		// Set Register
		if (_Register_Pointer_Set(HARM, _Harmonic)) {
			
			// Set Variable
			_Result = _Harmonic;
			
		} else {

			// Set Variable
			_Result = 255;

		}

	}
	
	// End Function
	return(_Result);

}
uint64_t MAX78630::Bucket(bool _Set, uint32_t _Bucket_H, uint32_t _Bucket_L) {

	// Define Objects
	Register BUCKET_LOW		{0x01, 0xD1, 0, true};		// Energy Bucket Size – Low word
	Register BUCKET_HIGH	{0x01, 0xD4, 0, true};		// Energy Bucket Size – High word

	// Control for Set
	if (_Set == true) {

		// Declare Variable
		bool _Result_LOW = false;
		bool _Result_HIGH = false;

		// Set Command
		_Result_LOW = _Register_Pointer_Set(BUCKET_LOW, _Bucket_L);
		_Result_HIGH = _Register_Pointer_Set(BUCKET_HIGH, _Bucket_H);

		// Handle Response
		if (_Result_LOW & _Result_HIGH) return(0xFF);
		
	} else {

		// Declare Variable
		uint32_t _Result_LOW = 0x00;
		uint32_t _Result_HIGH = 0x00;

		// Decide Command
		_Result_LOW = _Register_Pointer_Read(BUCKET_LOW);
		_Result_HIGH = _Register_Pointer_Read(BUCKET_HIGH);

		// Combine Function
		uint64_t _Result = (((uint32_t)_Result_HIGH << 24) & ((uint32_t)_Result_LOW));

	}

}

// Set Limit Registers
bool MAX78630::Set_Limit(uint8_t _Limit_ID, float _Value) {

	// Define Objects
	Register VRMS_MIN		{0x00, 0xB1, 23, true};		// Voltage lower alarm limit
	Register VRMS_MAX		{0x00, 0xB4, 23, true};		// Voltage upper alarm limit
	Register T_MIN			{0x01, 0x7A, 10, true};		// Temperature Alarm Lower Limit
	Register T_MAX			{0x01, 0x7D, 10, true};		// Temperature Alarm Upper Limit
	Register F_MIN			{0x01, 0x83, 16, true};		// Frequency Alarm Lower Limit
	Register F_MAX			{0x01, 0x86, 16, true};		// Frequency Alarm Upper Limit
	Register IRMS_MAX		{0x00, 0xF3, 23, true};		// Current upper alarm limit
	Register PF_MIN			{0x01, 0x71, 22, true};		// Power Factor lower alarm limit
	Register V_IMB_MAX		{0x00, 0x81, 23, true};		// Voltage imbalance alarm limit
	Register I_IMB_MAX		{0x00, 0x84, 23, true};		// Current imbalance alarm limit

	// Declare Data Variable
	uint32_t _Data = 0x00;

	// Decide Command
	if (_Limit_ID == 1) _Data = _FtoS(_Value, T_MAX.Data_Type);
	if (_Limit_ID == 2) _Data = _FtoS(_Value, T_MIN.Data_Type);
	if (_Limit_ID == 3) _Data = _FtoS(_Value, F_MAX.Data_Type);
	if (_Limit_ID == 4) _Data = _FtoS(_Value, F_MIN.Data_Type);
	if (_Limit_ID == 5) _Data = _FtoS(_Value / _VScale, VRMS_MAX.Data_Type);
	if (_Limit_ID == 6) _Data = _FtoS(_Value / _VScale, VRMS_MIN.Data_Type);
	if (_Limit_ID == 7) _Data = _FtoS(_Value / _IScale, IRMS_MAX.Data_Type);
	if (_Limit_ID == 8) _Data = _FtoS(_Value, PF_MIN.Data_Type);
	if (_Limit_ID == 9) _Data = _FtoS(_Value / 100, V_IMB_MAX.Data_Type);
	if (_Limit_ID == 10) _Data = _FtoS(_Value / 100, I_IMB_MAX.Data_Type);
	
	// Define Result Variable
	bool _Result = false;

	// Decide Command
	if (_Limit_ID == 1) _Result = _Register_Pointer_Set(T_MAX, _Data);
	if (_Limit_ID == 2) _Result = _Register_Pointer_Set(T_MIN, _Data);
	if (_Limit_ID == 3) _Result = _Register_Pointer_Set(F_MAX, _Data);
	if (_Limit_ID == 4) _Result = _Register_Pointer_Set(F_MIN, _Data);
	if (_Limit_ID == 5) _Result = _Register_Pointer_Set(VRMS_MAX, _Data);
	if (_Limit_ID == 6) _Result = _Register_Pointer_Set(VRMS_MIN, _Data);
	if (_Limit_ID == 7) _Result = _Register_Pointer_Set(IRMS_MAX, _Data);
	if (_Limit_ID == 8) _Result = _Register_Pointer_Set(PF_MIN, _Data);
	if (_Limit_ID == 9) _Result = _Register_Pointer_Set(V_IMB_MAX, _Data);
	if (_Limit_ID == 10) _Result = _Register_Pointer_Set(I_IMB_MAX, _Data);
	
	// End Function
	return(_Result);

}
float MAX78630::Get_Limit(uint8_t _Limit_ID) {

	// Define Objects
	Register VRMS_MIN		{0x00, 0xB1, 23, true};		// Voltage lower alarm limit
	Register VRMS_MAX		{0x00, 0xB4, 23, true};		// Voltage upper alarm limit
	Register T_MIN			{0x01, 0x7A, 10, true};		// Temperature Alarm Lower Limit
	Register T_MAX			{0x01, 0x7D, 10, true};		// Temperature Alarm Upper Limit
	Register F_MIN			{0x01, 0x83, 16, true};		// Frequency Alarm Lower Limit
	Register F_MAX			{0x01, 0x86, 16, true};		// Frequency Alarm Upper Limit
	Register IRMS_MAX		{0x00, 0xF3, 23, true};		// Current upper alarm limit
	Register PF_MIN			{0x01, 0x71, 22, true};		// Power Factor lower alarm limit
	Register V_IMB_MAX		{0x00, 0x81, 23, true};		// Voltage imbalance alarm limit
	Register I_IMB_MAX		{0x00, 0x84, 23, true};		// Current imbalance alarm limit

	// Declare Variable
	float _Result = 0;

	// Decide Command
	if (_Limit_ID == 1) _Result = _Register_Pointer_Read(T_MAX);
	if (_Limit_ID == 2) _Result = _Register_Pointer_Read(T_MIN);
	if (_Limit_ID == 3) _Result = _Register_Pointer_Read(F_MAX);
	if (_Limit_ID == 4) _Result = _Register_Pointer_Read(F_MIN);
	if (_Limit_ID == 5) _Result = _Register_Pointer_Read(VRMS_MAX) * _VScale;
	if (_Limit_ID == 6) _Result = _Register_Pointer_Read(VRMS_MIN) * _VScale;
	if (_Limit_ID == 7) _Result = _Register_Pointer_Read(IRMS_MAX) * _IScale;
	if (_Limit_ID == 8) _Result = _Register_Pointer_Read(PF_MIN);
	if (_Limit_ID == 9) _Result = _Register_Pointer_Read(V_IMB_MAX) * 100;
	if (_Limit_ID == 10) _Result = _Register_Pointer_Read(I_IMB_MAX) * 100;
	
	// End Function
	return(_Result);

}

// Data Refresh Functions
uint32_t MAX78630::Get_Frame(void) {

	// Define Objects
	Register FRAME 			{0x00, 0x12, 0, false};		// Low-rate sample counter

	// Declare Variable
	uint32_t _Result = 0;

	// Decide Command
	_Result = _Register_Pointer_Read(FRAME);
	
	// End Function
	return(_Result);

}
uint32_t MAX78630::Get_Cycle(void) {

	// Define Objects
	Register CYCLE 			{0x00, 0x0F, 0, false};		// High-rate sample counter

	// Declare Variable
	uint32_t _Result = 0;

	// Decide Command
	_Result = _Register_Pointer_Read(CYCLE);
	
	// End Function
	return(_Result);

}
uint32_t MAX78630::Get_Divisor(void) {

	// Define Objects
	Register DIVISOR 		{0x00, 0x0C, 0, false};		// Actual samples in previous accumulation interval

	// Declare Variable
	uint32_t _Result = 0;

	// Decide Command
	_Result = _Register_Pointer_Read(DIVISOR);
	
	// End Function
	return(_Result);

}
uint32_t MAX78630::Get_Samples(void) {

	// Define Objects
	Register SAMPLES 		{0x00, 0x09, 0, true};		// Minimum high-rate samples per accumulation interval

	// Declare Variable
	uint32_t _Result = 0;

	// Decide Command
	_Result = _Register_Pointer_Read(SAMPLES);
	
	// End Function
	return(_Result);

}

// Calibration Functions
float MAX78630::Get_Voltage_Gain(char Phase) {

	// Define Objects
	Register V1_GAIN		{0x00, 0x5D, 21, true};		// Voltage Gain Calibration
	Register V2_GAIN		{0x00, 0x60, 21, true};		// Voltage Gain Calibration
	Register V3_GAIN		{0x00, 0x63, 21, true};		// Voltage Gain Calibration

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(V1_GAIN); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(V2_GAIN); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(V3_GAIN); // Measure Phase T
	
	// End Function
	return(_Result);

}
float MAX78630::Get_Current_Gain(char Phase) {

	// Define Objects
	Register I1_GAIN		{0x00, 0x54, 21, true};		// Current Gain Calibration
	Register I2_GAIN		{0x00, 0x57, 21, true};		// Current Gain Calibration
	Register I3_GAIN		{0x00, 0x5A, 21, true};		// Current Gain Calibration

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(I1_GAIN); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(I2_GAIN); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(I3_GAIN); // Measure Phase T
	
	// End Function
	return(_Result);

}
float MAX78630::Get_Temperature_Gain(void) {

	// Define Objects
	Register T_GAIN			{0x00, 0x78, 0, true};		// Temperature Slope Calibration

	// Declare Variable
	float _Result = 0;

	_Result = _Register_Pointer_Read(T_GAIN); // Measure Phase R
	
	// End Function
	return(_Result);

}
float MAX78630::Get_Voltage_Offset(char Phase) {

	// Define Objects
	Register V1_OFFS		{0x00, 0x6F, 23, true};		// Voltage Offset Calibration
	Register V2_OFFS		{0x00, 0x72, 23, true};		// Voltage Offset Calibration
	Register V3_OFFS		{0x00, 0x75, 23, true};		// Voltage Offset Calibration

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(V1_OFFS); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(V2_OFFS); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(V3_OFFS); // Measure Phase T
	
	// End Function
	return(_Result);

}
float MAX78630::Get_Current_Offset(char Phase) {

	// Define Objects
	Register I1_OFFS		{0x00, 0x66, 23, true};		// Current Offset Calibration
	Register I2_OFFS		{0x00, 0x69, 23, true};		// Current Offset Calibration
	Register I3_OFFS		{0x00, 0x6C, 23, true};		// Current Offset Calibration

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(I1_OFFS); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(I2_OFFS); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(I3_OFFS); // Measure Phase T
	
	// End Function
	return(_Result);

}
float MAX78630::Get_Temperature_Offset(void) {

	// Define Objects
	Register T_OFFS			{0x00, 0x7B, 0, true};		// Temperature Offset Calibration

	// Declare Variable
	float _Result = 0;

	_Result = _Register_Pointer_Read(T_OFFS); // Measure Phase R
	
	// End Function
	return(_Result);

}
float MAX78630::Get_Voltage_RMS_Offset(char Phase) {

	// Define Objects
	Register IARMS_OFF		{0x00, 0xC3, 23, true};		// RMS Current dynamic offset adjust A
	Register IBRMS_OFF		{0x00, 0xC6, 23, true};		// RMS Current dynamic offset adjust B
	Register ICRMS_OFF		{0x00, 0xC9, 23, true};		// RMS Current dynamic offset adjust C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(IARMS_OFF); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(IBRMS_OFF); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(ICRMS_OFF); // Measure Phase T
	
	// End Function
	return(_Result);

}
float MAX78630::Get_Active_Power_Offset(char Phase) {

	// Define Objects
	Register PA_OFFS		{0x01, 0x14, 23, true};		// Active Power dynamic offset adjust A
	Register PB_OFFS		{0x01, 0x17, 23, true};		// Active Power dynamic offset adjust B
	Register PC_OFFS		{0x01, 0x1A, 23, true};		// Active Power dynamic offset adjust C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(PA_OFFS); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(PB_OFFS); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(PC_OFFS); // Measure Phase T
	
	// End Function
	return(_Result);

}
float MAX78630::Get_ReActive_Power_Offset(char Phase) {

	// Define Objects
	Register QA_OFFS		{0x01, 0x0B, 23, true};		// Reactive Power dynamic offset adjust A
	Register QB_OFFS		{0x01, 0x0E, 23, true};		// Reactive Power dynamic offset adjust B
	Register QC_OFFS		{0x01, 0x11, 23, true};		// Reactive Power dynamic offset adjust C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(QA_OFFS); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(QB_OFFS); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(QC_OFFS); // Measure Phase T
	
	// End Function
	return(_Result);

}
float MAX78630::Get_Voltage_HPF_COEF(void) {

	// Define Objects
	Register HPF_COEF_V		{0x00, 0x3F, 23, true};		// Voltage input HPF coefficient

	// Declare Variable
	float _Result = 0;

	_Result = _Register_Pointer_Read(HPF_COEF_V); // Measure Phase R
	
	// End Function
	return(_Result);

}
float MAX78630::Get_Current_HPF_COEF(void) {

	// Define Objects
	Register HPF_COEF_I		{0x00, 0x3C, 23, true};		// Current input HPF coefficient

	// Declare Variable
	float _Result = 0;

	_Result = _Register_Pointer_Read(HPF_COEF_I); // Measure Phase R
	
	// End Function
	return(_Result);

}

// Voltage Measurements
float MAX78630::Voltage_RMS(char Phase) {

	// Define Objects
	Register VA_RMS			{0x00, 0x90, 23, false};	// RMS Voltage A
	Register VB_RMS			{0x00, 0x93, 23, false};	// RMS Voltage B
	Register VC_RMS			{0x00, 0x96, 23, false};	// RMS Voltage C
	Register VT_RMS			{0x00, 0x99, 23, false};	// RMS Voltage average

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

	// Define Objects
	Register VA				{0x00, 0x87, 23, false};	// Instantaneous Voltage A
	Register VB				{0x00, 0x8A, 23, false};	// Instantaneous Voltage B
	Register VC				{0x00, 0x8D, 23, false};	// Instantaneous Voltage C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(VA); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(VB); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(VC); // Measure Phase T
	
	// End Function
	return(_Result * _VScale);

}
float MAX78630::Voltage_Fundamental(const char _Phase) {

	// Define Objects
	Register VFUND_A		{0x00, 0x9C, 23, false};	// Fundamental Voltage A
	Register VFUND_B		{0x00, 0x9F, 23, false};	// Fundamental Voltage B
	Register VFUND_C		{0x00, 0xA2, 23, false};	// Fundamental Voltage C

	// Declare Variable
	float _Result = 0;

	// Get Measured Data
	if (_Phase == 'R') _Result = _Register_Pointer_Read(VFUND_A); // Measure Phase R
	if (_Phase == 'S') _Result = _Register_Pointer_Read(VFUND_B); // Measure Phase S
	if (_Phase == 'T') _Result = _Register_Pointer_Read(VFUND_C); // Measure Phase T

	// End Function
	return(_Result * _VScale);

}
float MAX78630::Voltage_Harmonic(const char _Phase) {

	// Define Objects
	Register VHARM_A		{0x00, 0xA5, 23, false};	// Harmonic Voltage A
	Register VHARM_B		{0x00, 0xA8, 23, false};	// Harmonic Voltage B
	Register VHARM_C		{0x00, 0xAB, 23, false};	// Harmonic Voltage C

	// Declare Variable
	float _Result = 0;

	// Get Measured Data
	if (_Phase == 'R') _Result = _Register_Pointer_Read(VHARM_A); // Measure Phase R
	if (_Phase == 'S') _Result = _Register_Pointer_Read(VHARM_B); // Measure Phase S
	if (_Phase == 'T') _Result = _Register_Pointer_Read(VHARM_C); // Measure Phase T
	
	// End Function
	return(_Result * _VScale);

}
float MAX78630::Voltage_SAG_Limit(void) {

	// Define Objects
	Register VSAG_LIM		{0x00, 0xB7, 23, true};		// RMS Voltage Sag threshold

	// Declare Variable
	float _Result = 0;

	_Result = _Register_Pointer_Read(VSAG_LIM); // Measure Phase R
	
	// End Function
	return(_Result * _VScale);

}

// Phase Compansation
float MAX78630::Phase_Compansation(const char _Phase) {

	// Define Objects
	Register PHASECOMP1		{0x00, 0x42, 21, true};		// Phase compensation A
	Register PHASECOMP2		{0x00, 0x45, 21, true};		// Phase compensation B
	Register PHASECOMP3		{0x00, 0x48, 21, true};		// Phase compensation C

	// Declare Variable
	float _Result = 0;

	// Get Measured Data
	if (_Phase == 'R') _Result = _Register_Pointer_Read(PHASECOMP1); // Measure Phase R
	if (_Phase == 'S') _Result = _Register_Pointer_Read(PHASECOMP2); // Measure Phase S
	if (_Phase == 'T') _Result = _Register_Pointer_Read(PHASECOMP3); // Measure Phase T
	
	// End Function
	return(_Result);

}

// Current Measurements
float MAX78630::Current_RMS(char Phase) {

	// Define Objects
	Register IA_RMS			{0x00, 0xD5, 23, false};	// RMS Current A
	Register IB_RMS			{0x00, 0xD8, 23, false};	// RMS Current B
	Register IC_RMS			{0x00, 0xDB, 23, false};	// RMS Current C
	Register IT_RMS			{0x00, 0xDE, 23, false};	// RMS Current average

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

	// Define Objects
	Register IA				{0x00, 0xBA, 23, false};	// Instantaneous Current A
	Register IB				{0x00, 0xBD, 23, false};	// Instantaneous Current B
	Register IC				{0x00, 0xC0, 23, false};	// Instantaneous Current C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(IA); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(IB); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(IC); // Measure Phase T
	
	// End Function
	return(_Result * _IScale);

}
float MAX78630::Current_Peak(char Phase) {

	// Define Objects
	Register IA_PEAK		{0x00, 0xCC, 23, false};	// Peak Current A
	Register IB_PEAK		{0x00, 0xCF, 23, false};	// Peak Current B
	Register IC_PEAK		{0x00, 0xD2, 23, false};	// Peak Current C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(IA_PEAK); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(IB_PEAK); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(IC_PEAK); // Measure Phase T
	
	// End Function
	return(_Result * _IScale);

}
float MAX78630::Current_Fundamental(char Phase) {

	// Define Objects
	Register IFUND_A		{0x00, 0xE1, 23, false};	// Fundamental Current A
	Register IFUND_B		{0x00, 0xE4, 23, false};	// Fundamental Current B
	Register IFUND_C		{0x00, 0xE7, 23, false};	// Fundamental Current C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(IFUND_A); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(IFUND_B); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(IFUND_C); // Measure Phase T
	
	// End Function
	return(_Result * _IScale);

}
float MAX78630::Current_Harmonic(char Phase) {

	// Define Objects
	Register IHARM_A		{0x00, 0xEA, 23, false};	// Harmonic Current A
	Register IHARM_B		{0x00, 0xED, 23, false};	// Harmonic Current B
	Register IHARM_C		{0x00, 0xF0, 23, false};	// Harmonic Current C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(IHARM_A); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(IHARM_B); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(IHARM_C); // Measure Phase T
	
	// End Function
	return(_Result * _IScale);

}

// Power Measurements
float MAX78630::Active_Power(char Phase) {

	// Define Objects
	Register WATT_A			{0x01, 0x1D, 23, false};	// Active Power A
	Register WATT_B			{0x01, 0x20, 23, false};	// Active Power B
	Register WATT_C			{0x01, 0x23, 23, false};	// Active Power C
	Register WATT_T			{0x01, 0x38, 23, false};	// Active Power average

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

	// Define Objects
	Register VAR_A			{0x01, 0x26, 23, false};	// Reactive Power A
	Register VAR_B			{0x01, 0x29, 23, false};	// Reactive Power B
	Register VAR_C			{0x01, 0x2C, 23, false};	// Reactive Power C
	Register VAR_T			{0x01, 0x3B, 23, false};	// Reactive Power average

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

	// Define Objects
	Register VA_A			{0x01, 0x2F, 23, false};	// Apparent Power A
	Register VA_B			{0x01, 0x32, 23, false};	// Apparent Power B
	Register VA_C			{0x01, 0x35, 23, false};	// Apparent Power C
	Register VA_T			{0x01, 0x3E, 23, false};	// Apparent Power average

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

	// Define Objects
	Register PFUND_A		{0x01, 0x4A, 23, false};	// Fundamental Power A
	Register PFUND_B		{0x01, 0x4D, 23, false};	// Fundamental Power B
	Register PFUND_C		{0x01, 0x50, 23, false};	// Fundamental Power C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(PFUND_A); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(PFUND_B); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(PFUND_C); // Measure Phase T
	
	// End Function
	return(_Result * _IScale * _VScale);
	
}
float MAX78630::Harmonic_Power(char Phase) {

	// Define Objects
	Register PHARM_A		{0x01, 0x53, 23, false};	// Harmonic Power A
	Register PHARM_B		{0x01, 0x56, 23, false};	// Harmonic Power B
	Register PHARM_C		{0x01, 0x59, 23, false};	// Harmonic Power C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(PHARM_A); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(PHARM_B); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(PHARM_C); // Measure Phase T
	
	// End Function
	return(_Result * _IScale * _VScale);
	
}
float MAX78630::Power_Factor(char Phase) {

	// Define Objects
	Register PFA			{0x01, 0x65, 22, false};	// Power Factor A
	Register PFB			{0x01, 0x68, 22, false};	// Power Factor B
	Register PFC			{0x01, 0x6B, 22, false};	// Power Factor C
	Register PF_T			{0x01, 0x6E, 22, false};	// T otal Power Factor

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(PFA); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(PFB); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(PFC); // Measure Phase T
	if (Phase == 'A') _Result = _Register_Pointer_Read(PF_T); // Measure Phase Average
	
	// End Function
	return(_Result);
	
}
float MAX78630::Fundamental_ReActive_Power(char Phase) {

	// Define Objects
	Register QFUND_A		{0x00, 0xF9, 23, false};	// Fundamental Reactive Power A
	Register QFUND_B		{0x00, 0xFC, 23, false};	// Fundamental Reactive Power B
	Register QFUND_C		{0x00, 0xFF, 23, false};	// Fundamental Reactive Power C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(QFUND_A); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(QFUND_B); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(QFUND_C); // Measure Phase T
	
	// End Function
	return(_Result * _IScale * _VScale);
	
}
float MAX78630::Harmonic_ReActive_Power(char Phase) {

	// Define Objects
	Register QHARM_A		{0x01, 0x02, 23, false};	// Harmonic Reactive Power A
	Register QHARM_B		{0x01, 0x05, 23, false};	// Harmonic Reactive Power B
	Register QHARM_C		{0x01, 0x08, 23, false};	// Harmonic Reactive Power C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(QHARM_A); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(QHARM_B); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(QHARM_C); // Measure Phase T
	
	// End Function
	return(_Result * _IScale * _VScale);
	
}
float MAX78630::Fundamental_VA_Power(char Phase) {

	// Define Objects
	Register VAFUNDA		{0x01, 0x5C, 23, false};	// Fundamental Volt Amperes A
	Register VAFUNDB		{0x01, 0x5F, 23, false};	// Fundamental Volt Amperes B
	Register VAFUNDC		{0x01, 0x62, 23, false};	// Fundamental Volt Amperes C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(VAFUNDA); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(VAFUNDB); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(VAFUNDC); // Measure Phase T
	
	// End Function
	return(_Result * _IScale * _VScale);
	
}

// Energy Measurements
float MAX78630::Active_Energy_Recieved(char Phase) {

	// Define Objects
	Register WHA_POS		{0x01, 0xDD, 0, false};		// Received Active Energy Counter A
	Register WHB_POS		{0x01, 0xEF, 0, false};		// Received Active Energy Counter B
	Register WHC_POS		{0x02, 0x01, 0, false};		// Received Active Energy Counter C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(WHA_POS); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(WHB_POS); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(WHC_POS); // Measure Phase T
	
	// End Function
	return(_Result);
	
}
float MAX78630::Active_Energy_Delivered(char Phase) {

	// Define Objects
	Register WHA_NEG		{0x01, 0xE6, 0, false};		// Delivered Active Energy Counter A
	Register WHB_NEG		{0x01, 0xF8, 0, false};		// Delivered Active Energy Counter B
	Register WHC_NEG		{0x02, 0x0A, 0, false};		// Delivered Active Energy Counter C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(WHA_NEG); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(WHB_NEG); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(WHC_NEG); // Measure Phase T
	
	// End Function
	return(_Result);
	
}
float MAX78630::ReActive_Energy_Recieved(char Phase) {

	// Define Objects
	Register VARHA_POS		{0x02, 0x13, 0, false};		// Reactive Energy Leading Counter A
	Register VARHB_POS		{0x02, 0x25, 0, false};		// Reactive Energy Leading Counter B
	Register VARHC_POS		{0x02, 0x37, 0, false};		// Reactive Energy Leading Counter C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(VARHA_POS); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(VARHB_POS); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(VARHC_POS); // Measure Phase T
	
	// End Function
	return(_Result);
	
}
float MAX78630::ReActive_Energy_Delivered(char Phase) {

	// Define Objects
	Register VARHA_NEG		{0x02, 0x1C, 0, false};		// Reactive Energy Lagging Counter A
	Register VARHB_NEG		{0x02, 0x2E, 0, false};		// Reactive Energy Lagging Counter B
	Register VARHC_NEG		{0x02, 0x40, 0, false};		// Reactive Energy Lagging Counter C

	// Declare Variable
	float _Result = 0;

	if (Phase == 'R') _Result = _Register_Pointer_Read(VARHA_NEG); // Measure Phase R
	if (Phase == 'S') _Result = _Register_Pointer_Read(VARHB_NEG); // Measure Phase S
	if (Phase == 'T') _Result = _Register_Pointer_Read(VARHC_NEG); // Measure Phase T
	
	// End Function
	return(_Result);
	
}

// Temperature Measurements
float MAX78630::IC_Temperature(void) {

	// Define Objects
	Register TEMPC			{0x01, 0x74, 10, false};	// Chip Temperature

	// Declare Variable
	float _Result = 0;

	// Get Data
	_Result = _Register_Pointer_Read(TEMPC);

	// End Function
	return(_Result);
	
}

// Frequency Measurements
float MAX78630::Frequency(void) {

	// Define Objects
	Register FREQ			{0x01, 0x80, 16, false};	// Line Frequency

	// Declare Variable
	float _Result = 0;

	// Get Data
	_Result = _Register_Pointer_Read(FREQ);

	// End Function
	return(_Result);
	
}

// Min Max Tracking Functions
bool MAX78630::Set_Min_Max_Address(uint8_t _MM_ADDR, uint32_t _Mask) {

	// Recorded MIN/MAX Registers
	Register MMADDR0		{0x01, 0xB9, 0, true};		// Min/Max Monitor address 1
	Register MMADDR1		{0x01, 0xBC, 0, true};		// Min/Max Monitor address 2
	Register MMADDR2		{0x01, 0xBF, 0, true};		// Min/Max Monitor address 3
	Register MMADDR3		{0x01, 0xC2, 0, true};		// Min/Max Monitor address 4
	Register MMADDR4		{0x01, 0xC5, 0, true};		// Min/Max Monitor address 5
	Register MMADDR5		{0x01, 0xC8, 0, true};		// Min/Max Monitor address 6
	Register MMADDR6		{0x01, 0xCB, 0, true};		// Min/Max Monitor address 7
	Register MMADDR7		{0x01, 0xCE, 0, true};		// Min/Max Monitor address 8

	// Define Result Variable
	bool _Result = false;

	// Decide Command
	if (_MM_ADDR == 1) _Result = _Register_Pointer_Set(MMADDR0, _Mask);
	if (_MM_ADDR == 2) _Result = _Register_Pointer_Set(MMADDR1, _Mask);
	if (_MM_ADDR == 3) _Result = _Register_Pointer_Set(MMADDR2, _Mask);
	if (_MM_ADDR == 4) _Result = _Register_Pointer_Set(MMADDR3, _Mask);
	if (_MM_ADDR == 5) _Result = _Register_Pointer_Set(MMADDR4, _Mask);
	if (_MM_ADDR == 6) _Result = _Register_Pointer_Set(MMADDR5, _Mask);
	if (_MM_ADDR == 7) _Result = _Register_Pointer_Set(MMADDR6, _Mask);
	if (_MM_ADDR == 8) _Result = _Register_Pointer_Set(MMADDR7, _Mask);

	// End Function
	return(_Result);

}
float MAX78630::Get_Min_Value(uint8_t _MM_ADDR) {

	// Define Objects
	Register MIN0			{0x01, 0x89, 0, false};		// Minimum Recorded Value 1
	Register MIN1			{0x01, 0x8C, 0, false};		// Minimum Recorded Value 2
	Register MIN2			{0x01, 0x8F, 0, false};		// Minimum Recorded Value 3
	Register MIN3			{0x01, 0x92, 0, false};		// Minimum Recorded Value 4
	Register MIN4			{0x01, 0x95, 0, false};		// Minimum Recorded Value 5
	Register MIN5			{0x01, 0x98, 0, false};		// Minimum Recorded Value 6
	Register MIN6			{0x01, 0x9B, 0, false};		// Minimum Recorded Value 7
	Register MIN7			{0x01, 0x9E, 0, false};		// Minimum Recorded Value 8

	// Declare Variable
	float _Result = 0;

	if (_MM_ADDR == 1) _Result = _Register_Pointer_Read(MIN0); // Measure Phase R
	if (_MM_ADDR == 2) _Result = _Register_Pointer_Read(MIN1); // Measure Phase R
	if (_MM_ADDR == 3) _Result = _Register_Pointer_Read(MIN2); // Measure Phase R
	if (_MM_ADDR == 4) _Result = _Register_Pointer_Read(MIN3); // Measure Phase R
	if (_MM_ADDR == 5) _Result = _Register_Pointer_Read(MIN4); // Measure Phase R
	if (_MM_ADDR == 6) _Result = _Register_Pointer_Read(MIN5); // Measure Phase R
	if (_MM_ADDR == 7) _Result = _Register_Pointer_Read(MIN6); // Measure Phase R
	if (_MM_ADDR == 8) _Result = _Register_Pointer_Read(MIN7); // Measure Phase R

	// End Function
	return(_Result);

}
float MAX78630::Get_Max_Value(uint8_t _MM_ADDR) {

	// Define Objects
	Register MAX0			{0x01, 0xA1, 0, false};		// Maximum Recorded Value 1
	Register MAX1			{0x01, 0xA4, 0, false};		// Maximum Recorded Value 2
	Register MAX2			{0x01, 0xA7, 0, false};		// Maximum Recorded Value 3
	Register MAX3			{0x01, 0xAA, 0, false};		// Maximum Recorded Value 4
	Register MAX4			{0x01, 0xAD, 0, false};		// Maximum Recorded Value 5
	Register MAX5			{0x01, 0xB0, 0, false};		// Maximum Recorded Value 6
	Register MAX6			{0x01, 0xB3, 0, false};		// Maximum Recorded Value 7
	Register MAX7			{0x01, 0xB6, 0, false};		// Maximum Recorded Value 8

	// Declare Variable
	float _Result = 0;

	if (_MM_ADDR == 1) _Result = _Register_Pointer_Read(MAX0); // Measure Phase R
	if (_MM_ADDR == 2) _Result = _Register_Pointer_Read(MAX1); // Measure Phase R
	if (_MM_ADDR == 3) _Result = _Register_Pointer_Read(MAX2); // Measure Phase R
	if (_MM_ADDR == 4) _Result = _Register_Pointer_Read(MAX3); // Measure Phase R
	if (_MM_ADDR == 5) _Result = _Register_Pointer_Read(MAX4); // Measure Phase R
	if (_MM_ADDR == 6) _Result = _Register_Pointer_Read(MAX5); // Measure Phase R
	if (_MM_ADDR == 7) _Result = _Register_Pointer_Read(MAX6); // Measure Phase R
	if (_MM_ADDR == 8) _Result = _Register_Pointer_Read(MAX7); // Measure Phase R

	// End Function
	return(_Result);

}

// TODO: Working on functions
bool MAX78630::Alarm(void) {

	// Alarm Registers
	Register STATUS_SET		{0x00, 0x1B, 0, false};		// Used to set/force alarm/status bits
	Register MASK1			{0x00, 0x1E, 0, true};		// Alarm/status mask for AL1 output pin
	Register MASK2			{0x00, 0x21, 0, true};		// Alarm/status mask for AL2 output pin
	Register MASK3			{0x00, 0x24, 0, true};		// Alarm/status mask for AL3 output pin
	Register MASK4			{0x00, 0x27, 0, true};		// Alarm/status mask for AL4 output pin
	Register MASK5			{0x00, 0x2A, 0, true};		// Alarm/status mask for AL5 output pin
	Register STICKY			{0x00, 0x2D, 0, true};		// Alarm/status bits to hold until cleared by host

}
bool MAX78630::DIO(void) {

	// DIO Registers
	Register DIO_STATE		{0x00, 0x30, 0, false};		// State of DIO pins
	Register DIO_DIR		{0x00, 0x33, 0, true};		// Direction of DIO pins. 1=Input ; 0=Output
	Register DIO_POL		{0x00, 0x36, 0, true};		// Polarity of DIO pins. 1=Active High ; 0=Active Low


}

// Control Functions
void MAX78630::Control_Limits(void) {

	// Define Objects
	Register STATUS			{0x00, 0x15, 0, false};		// Alarm and device status bits

	// Read Status Register
	uint32_t _Status = _Register_Pointer_Read(STATUS);

	// Clear Variables
	Limit.Current_Imbalance = false;
	Limit.Voltage_Imbalance = false;
	Limit.Sag_VR = false;
	Limit.Sag_VS = false;
	Limit.Sag_VT = false;
	Limit.Current_Over_Limit_VR = false;
	Limit.Current_Over_Limit_VS = false;
	Limit.Current_Over_Limit_VT = false;
	Limit.Power_Factor_Under_Limit_VR = false;
	Limit.Power_Factor_Under_Limit_VS = false;
	Limit.Power_Factor_Under_Limit_VT = false;
	Limit.Voltage_Under_Limit_VR = false;
	Limit.Voltage_Over_Limit_VR = false;
	Limit.Voltage_Under_Limit_VS = false;
	Limit.Voltage_Over_Limit_VS = false;
	Limit.Voltage_Under_Limit_VT = false;
	Limit.Voltage_Over_Limit_VT = false;
	Limit.Temperature_Under_Limit = false;
	Limit.Temperature_Over_Limit = false;
	Limit.Frequency_Under_Limit = false;
	Limit.Frequency_Over_Limit = false;

	// Read Bit
	if (bitRead(_Status, I_IMBAL)) Limit.Current_Imbalance = true;
	if (bitRead(_Status, V_IMBAL)) Limit.Voltage_Imbalance = true;
	if (bitRead(_Status, VA_SAG)) Limit.Sag_VR = true;
	if (bitRead(_Status, VB_SAG)) Limit.Sag_VS = true;
	if (bitRead(_Status, VC_SAG)) Limit.Sag_VT = true;
	if (bitRead(_Status, OV_IRMSA)) Limit.Current_Over_Limit_VR = true;
	if (bitRead(_Status, OV_IRMSB)) Limit.Current_Over_Limit_VS = true;
	if (bitRead(_Status, OV_IRMSC)) Limit.Current_Over_Limit_VT = true;
	if (bitRead(_Status, UN_PFA)) Limit.Power_Factor_Under_Limit_VR = true;
	if (bitRead(_Status, UN_PFB)) Limit.Power_Factor_Under_Limit_VS = true;
	if (bitRead(_Status, UN_PFC)) Limit.Power_Factor_Under_Limit_VT = true;
	if (bitRead(_Status, UN_VRMSA)) Limit.Voltage_Under_Limit_VR = true;
	if (bitRead(_Status, OV_VRMSA)) Limit.Voltage_Over_Limit_VR = true;
	if (bitRead(_Status, UN_VRMSB)) Limit.Voltage_Under_Limit_VS = true;
	if (bitRead(_Status, OV_VRMSB)) Limit.Voltage_Over_Limit_VS = true;
	if (bitRead(_Status, UN_VRMSC)) Limit.Voltage_Under_Limit_VT = true;
	if (bitRead(_Status, OV_VRMSC)) Limit.Voltage_Over_Limit_VT = true;
	if (bitRead(_Status, UN_TEMP)) Limit.Temperature_Under_Limit = true;
	if (bitRead(_Status, OV_TEMP)) Limit.Temperature_Over_Limit = true;
	if (bitRead(_Status, UN_FREQ)) Limit.Frequency_Under_Limit = true;
	if (bitRead(_Status, OV_FREQ)) Limit.Frequency_Over_Limit = true;

	// Clear Control Bits
	Control_Clear();

}
void MAX78630::Control_Clear(void) {

	// Define Objects
	Register STATUS			{0x00, 0x15, 0, false};		// Alarm and device status bits
//	Register STATUS_CLEAR	{0x00, 0x18, 0, false};		// Used to reset alarm/status bits

	// Read Status Register
	_Register_Pointer_Set(STATUS, 0x00);
	
}

// Library Specified Functions
bool MAX78630::_Register_Pointer_Set(Register _Command, uint32_t _Data) {

	// Start Serial Connection
	Energy_Serial.begin(Energy_Serial_Baud);

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

	// Declare Variable
	uint8_t _Response[1];
	memset(_Response, '\0', 1);
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

	// Close UART Connection
	Energy_Serial.end();

	// End Function
	if (_Response[0 == 0xAD]) return(true);

	// End Function
	return(false);

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
uint32_t MAX78630::_FtoS(double _Variable, uint8_t _Data_Type) {

	// Declare Data Variable
	uint32_t _Data = 0x00;

	// Convert Loop
	for (uint8_t i = 0; i < 24; i++) {
		
		// Calculate Bit Multiplier
		double _Bit_Multiplier = pow(2, (-_Data_Type - (i - 23)));

		// Control for Bit
		if (_Variable >= _Bit_Multiplier) {
			
			// Set Bit Control
			bitSet(_Data, 23 - i);

			// Handle Data
			_Variable -= _Bit_Multiplier;

		} else {

			// Set Bit Control
			bitClear(_Data, 23 - i);

		}

	}

	// End Function
	return(_Data);

}
void MAX78630::_Clear_Buffer(void) {

	// Clear UART Buffer
	Energy_Serial.flush(); while(Energy_Serial.available() > 0) Energy_Serial.read(); delay(5);

}

// Define Library Class
MAX78630 Energy_Analayser;

// 1903