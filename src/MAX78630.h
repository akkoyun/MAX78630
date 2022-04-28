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

// Phase Type
#define __Phase_R__					(uint8_t)1
#define __Phase_S__					(uint8_t)2
#define __Phase_T__					(uint8_t)3
#define __Phase_Avg__				(uint8_t)4

// Scale Variables
#define __VSCALE__ 					667
#define __ISCALE__ 					7

// Measurement Type
#define __RMS__ 					(uint8_t)1
#define __Instant__ 				(uint8_t)2
#define __Fundamental__ 			(uint8_t)3
#define __Harmonic__ 				(uint8_t)4
#define __Peak__ 					(uint8_t)5
#define __Active__ 					(uint8_t)6
#define __ReActive__ 				(uint8_t)7
#define __Apparent__ 				(uint8_t)8
#define __Fund_ReActive__ 			(uint8_t)9
#define __Harm_ReActive__ 			(uint8_t)10
#define __Fund_VA__ 				(uint8_t)11
#define __Active_Recieved__ 		(uint8_t)12
#define __Active_Delivered__ 		(uint8_t)13
#define __ReActive_Recieved__ 		(uint8_t)14
#define __ReActive_Delivered__ 		(uint8_t)15

// Limit Type
#define __VRMS_MIN__ 				(uint8_t)1
#define __VRMS_MAX__ 				(uint8_t)2
#define __T_MIN__ 					(uint8_t)3
#define __T_MAX__ 					(uint8_t)4
#define __F_MIN__ 					(uint8_t)5
#define __F_MAX__ 					(uint8_t)6
#define __IRMS_MAX__ 				(uint8_t)7
#define __PF_MIN__ 					(uint8_t)8
#define __VIMB_MAX__ 				(uint8_t)9
#define __IIMB_MAX__ 				(uint8_t)10


class MAX78630 {

	private:

		// Stream Object Definition
		Stream *_Energy_Serial;

		// Device Register Structure
		struct Register {
			const uint8_t High_Address;
			const uint8_t Low_Address;
			const uint8_t Data_Type;
		};

		// Hardware Functions
		uint32_t Get_Device_Address(void);
		uint32_t Get_System_Stat(void);
		uint32_t Get_Firmware(void);

		// Data Refresh Functions	
		uint32_t Get_Frame(void);
		uint32_t Get_Cycle(void);
		uint32_t Get_Divisor(void);
		uint32_t Get_Samples(void);

		// Communication Functions
		uint32_t Read_Baud(void);
		bool Write_Baud(uint32_t _Baud);

		// Scale Functions
		bool Write_VScale(uint32_t _Voltage_Scale);
		bool Write_IScale(uint32_t _Current_Scale);
		bool Set_Scale(uint32_t _VScale, uint32_t _IScale);

		// Calibration Functions
		bool Voltage_Calibration(const uint8_t _Phase, const float _Gain, const float _Offset);
		bool Current_Calibration(const uint8_t _Phase, const float _Gain, const float _Offset);
		bool Temperature_Calibration(const float _Gain, const float _Offset);
		float Voltage_HPF_COEF(float _COEF);
		float Current_HPF_COEF(float _COEF);
		float Current_RMS_Offset(char Phase, float _Offset);
		float Active_Power_Offset(char Phase, float _Offset);
		float ReActive_Power_Offset(char Phase, float _Offset);

		// Bucket Functions
		bool Write_Bucket(uint32_t _Bucket_H, uint32_t _Bucket_L);

		// Limit Functions
		bool Write_Limit(uint8_t _Limit_ID, float _Value);

		// Stream Functions
		void _Clear_Buffer(void);

		// Register Functions
		double _Register_Pointer_Read(Register _Command);
		bool _Register_Pointer_Set(Register _Command, uint32_t _Data);
		uint32_t _FtoS(double _Variable, uint8_t _Data_Type);

	public:

		// Object Declaration Functions
		bool Begin(Stream &_Serial);

		// Measurement Functions
		float Voltage(const uint8_t _Phase, const uint8_t _Type);
		float Current(const uint8_t _Phase, const uint8_t _Type);
		float Power(const uint8_t _Phase, const uint8_t _Type);
		float Energy(const uint8_t _Phase, const uint8_t _Type);
		float Frequency(void);
		float Power_Factor(uint8_t _Phase);
		float IC_Temperature(void);

		// Set Functions
		uint8_t Set_Harmonic(uint32_t _Harmonic);

		// Limit Functions
		uint8_t Control_Limits(void);
		bool Set_Voltage_Limits(const float _VMin, const float _VMax);
		float Read_Voltage_Min_Limit(void);
		float Read_Voltage_Max_Limit(void);
		bool Set_Frequency_Limits(const float _FQMin, const float _FQMax);
		float Read_Frequency_Min_Limit(void);
		float Read_Frequency_Max_Limit(void);
		bool Set_Imbalance_Limits(const float _VImb, const float _IImb);
		float Read_Voltage_Imbalance_Max_Limit(void);
		float Read_Current_Imbalance_Max_Limit(void);
		float Voltage_SAG_Limit(void);

		// Phase Compensation
		float Phase_Compensation(const char _Phase);

		// Energy Functions
		void Active_Energy_Reset(char Phase);

		// Min Max Tracking Functions
		bool Set_Min_Max_Address(uint8_t _MM_ADDR, uint32_t _Mask);
		float Get_Min_Value(uint8_t _MM_ADDR);
		float Get_Max_Value(uint8_t _MM_ADDR);
		bool Clear_Min_Value(uint8_t _MM_ADDR);
		bool Clear_Max_Value(uint8_t _MM_ADDR);

		// TODO: Working on functions
		bool Alarm(void);
		bool DIO(void);

};

#endif /* defined(__MAX78630__) */