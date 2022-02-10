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
#include <Config.h>
#endif

// Define Registers
#ifndef __MAX78630_DEFINATION__
#include <Defination.h>
#endif

// Define VT100 Terminal
#ifndef VT100_H_
#include <VT100.h>
#endif


class MAX78630 {

	public:

		bool Begin(void);

		// Scale Functions
		uint16_t VScale(uint32_t _VScale);
		uint16_t IScale(uint32_t _VScale);
		uint8_t Harmonic(uint32_t _Harmonic);
		
		// Limit Functions
		bool Set_Limit(uint8_t _Limit_ID, float _Value);
		float Get_Limit(uint8_t _Limit_ID);

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

		// Temperature Functions
		float IC_Temperature(void);
		float Firmware(void);

		// Voltage Measurements
		float Voltage_RMS(char Phase);
		float Voltage_Instantaneous(char Phase);
		float Voltage_Fundamental(const char _Phase);
		float Voltage_Harmonic(const char _Phase);
		float Voltage_SAG_Limit(void);

		// Frequency Measurements
		float Frequency(void);

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

		// Limit Functions
		void Control_Limits(void);
		void Control_Clear(void);

		void VT100_Base(void);

		// Scale Variables
		float 	_VScale	= 0;
		float 	_IScale	= 0;

		// Set Global Limit Object
		Limits Limit {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};

	private:

		double _Register_Pointer_Read(Register _Command);
		void _Clear_Buffer(void);
		bool _Register_Pointer_Set(Register _Command, uint32_t _Data);
		uint32_t _FtoS(double _Variable, uint8_t _Data_Type);

};

extern MAX78630 Energy_Analayser;

#endif /* defined(__MAX78630__) */
