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

//		MAX78630(void);
//		~MAX78630(void);

		bool Begin(void);

		float IC_Temperature(void);
		float Firmware(void);

		// Voltage Measurements
		float Voltage_RMS(char Phase);
		float Voltage_Instantaneous(char Phase);
		float Voltage_Fundamental(char Phase);
		float Voltage_Harmonic(char Phase);
		float Voltage_V_Target(void);
		float Voltage_RMS_Min(void);
		float Voltage_RMS_Max(void);
		float Voltage_SAG_Limit(void);

		// Current Measurements
		float Current_RMS(char Phase);
		float Current_Instantaneous(char Phase);
		float Current_Fundamental(char Phase);
		float Current_Harmonic(char Phase);
		float Current_I_Target(void);
		float Current_RMS_Max(void);

		float Phase_Compensation(char Phase);

		bool Set_VScale(uint32_t _VScale);
		bool Set_IScale(uint32_t _VScale);

		void VT100_Base(void);

	private:

		double 		_Register_Pointer_Read(Register _Command);
		void		_Clear_Buffer(void);

		uint32_t 	_VScale	= 0;
		uint32_t 	_IScale	= 0;

};

extern MAX78630 Energy_Analayser;

#endif /* defined(__MAX78630__) */
