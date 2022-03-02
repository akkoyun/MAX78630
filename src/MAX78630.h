/* *******************************************************************************
 *  Copyright (C) 2014-2022 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: MAX78630 Energy Analayser
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *
 *********************************************************************************/

#ifndef __MAX78630__
#define __MAX78630__

// Define Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Define Library Structures
#ifndef __MAX78630_CONFIG__
#include <MAX78630_Config.h>
#endif

// Define Registers
#ifndef __MAX78630_DEFINATION__
#include <MAX78630_Defination.h>
#endif

class MAX78630 {

	public:

		// Scale Variables
		float 	_VScale	= 0;
		float 	_IScale	= 0;

		// Set Global Limit Object
		System_Limits Limit {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};

		// Object Declaration Functions
		bool Begin(Stream &_Serial);

		// Hardware Functions
		uint32_t Get_Baud(void);
		bool Set_Baud(uint32_t _Baud);
		uint32_t Get_Device_Address(void);
		uint32_t Get_System_Stat(void);
		uint32_t Get_Firmware(void);

		// Scale Functions
		uint16_t VScale(uint32_t _VScale);
		uint16_t IScale(uint32_t _VScale);
		uint8_t Harmonic(uint32_t _Harmonic);
		uint64_t Bucket(bool _Set, uint32_t _Bucket_L, uint32_t _Bucket_H);

		// Bucket Functions
		bool Set_Bucket(void);
		uint32_t Get_Bucket_LOW(void);
		uint32_t Get_Bucket_HIGH(void);

		// Limit Functions
		bool Set_Limit(uint8_t _Limit_ID, float _Value);
		float Get_Limit(uint8_t _Limit_ID);

		// Data Refresh Functions	
		uint32_t Get_Frame(void);
		uint32_t Get_Cycle(void);
		uint32_t Get_Divisor(void);
		uint32_t Get_Samples(void);

		// Calibration Functions
		float Get_Voltage_Gain(char Phase);
		float Get_Voltage_Offset(char Phase);
		float Get_Voltage_RMS_Offset(char Phase);
		float Get_Current_Gain(char Phase);
		float Get_Current_Offset(char Phase);
		float Get_Temperature_Gain(void);
		float Get_Temperature_Offset(void);
		float Get_Active_Power_Offset(char Phase);
		float Get_ReActive_Power_Offset(char Phase);
		float Get_Voltage_HPF_COEF(void);
		float Get_Current_HPF_COEF(void);

		// Temperature Functions
		float IC_Temperature(void);

		// Voltage Measurements
		float Voltage_RMS(char Phase);
		float Voltage_Instantaneous(char Phase);
		float Voltage_Fundamental(const char _Phase);
		float Voltage_Harmonic(const char _Phase);
		float Voltage_SAG_Limit(void);

		// Frequency Measurements
		float Frequency(void);

		// Phase Compensation
		float Phase_Compensation(const char _Phase);

		// Current Measurements
		float Current_RMS(char Phase);
		float Current_Instantaneous(char Phase);
		float Current_Peak(char Phase);
		float Current_Fundamental(char Phase);
		float Current_Harmonic(char Phase);

		// Power Functions
		float Active_Power(char Phase);
		float ReActive_Power(char Phase);
		float Apparent_Power(char Phase);
		float Fundamental_Power(char Phase);
		float Harmonic_Power(char Phase);
		float Power_Factor(char Phase);
		float Fundamental_ReActive_Power(char Phase);
		float Harmonic_ReActive_Power(char Phase);
		float Fundamental_VA_Power(char Phase);

		// Energy Functions
		float Active_Energy_Recieved(char Phase);
		float Active_Energy_Delivered(char Phase);
		float ReActive_Energy_Recieved(char Phase);
		float ReActive_Energy_Delivered(char Phase);
		void Active_Energy_Reset(char Phase);

		// Min Max Tracking Functions
		bool Set_Min_Max_Address(uint8_t _MM_ADDR, uint32_t _Mask);
		float Get_Min_Value(uint8_t _MM_ADDR);
		float Get_Max_Value(uint8_t _MM_ADDR);

		// TODO: Working on functions
		bool Alarm(void);
		bool DIO(void);

		// Limit Functions
		void Control_Limits(void);
		void Control_Clear(void);

	private:

		// Stream Object Defination
		Stream *_Energy_Serial;

		// Stream Functions
		void _Clear_Buffer(void);

		// Register Functions
		double _Register_Pointer_Read(Register _Command);
		bool _Register_Pointer_Set(Register _Command, uint32_t _Data);
		uint32_t _FtoS(double _Variable, uint8_t _Data_Type);

};

extern MAX78630 Energy_Analayser;

#endif /* defined(__MAX78630__) */