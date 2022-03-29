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

// Scale Variables
#define __VSCALE__ 667
#define __ISCALE__ 7

// Measurement Type
#define __RMS__ 1
#define __Instant__ 2
#define __Fundamental__ 3
#define __Harmonic__ 4
#define __Peak__ 5
#define __Active__ 6
#define __ReActive__ 7
#define __Apparent__ 8
#define __Fund_ReActive__ 9
#define __Harm_ReActive__ 10
#define __Fund_VA__ 11
#define __Active_Recieved__ 12
#define __Active_Delivered__ 13
#define __ReActive_Recieved__ 14
#define __ReActive_Delivered__ 15

// Limit Type
#define __VRMS_MIN__ 1
#define __VRMS_MAX__ 2
#define __T_MIN__ 3
#define __T_MAX__ 4
#define __F_MIN__ 5
#define __F_MAX__ 6
#define __IRMS_MAX__ 7
#define __PF_MIN__ 8
#define __VIMB_MAX__ 9
#define __IIMB_MAX__ 10


class MAX78630 {

	public:

		// Object Declaration Functions
		bool Begin(Stream &_Serial);

		// Hardware Functions
		uint32_t Get_Device_Address(void);
		uint32_t Get_System_Stat(void);
		uint32_t Get_Firmware(void);

		// Scale Functions
		uint16_t Get_VScale(void);
		uint16_t Get_IScale(void);
		bool Set_Scale(uint32_t _VScale, uint32_t _IScale);

		// Measurement Functions
		float Voltage(const uint8_t _Phase, const uint8_t _Type);
		float Current(const char _Phase, const uint8_t _Type);
		float Power(const char _Phase, const uint8_t _Type);
		float Energy(const char _Phase, const uint8_t _Type);
		float Frequency(void);
		float Power_Factor(char Phase);

		// Limit Functions
		bool Set_Voltage_Limits(float _VMin, float _VMax);
		float Read_Voltage_Min_Limit(void);
		float Read_Voltage_Max_Limit(void);
		bool Set_Frequency_Limits(float _FQMin, float _FQMax);
		float Read_Frequency_Min_Limit(void);
		float Read_Frequency_Max_Limit(void);
		bool Set_Imbalance_Limits(float _VImb, float _IImb);
		float Read_Voltage_Imbalance_Max_Limit(void);
		float Read_Current_Imbalance_Max_Limit(void);
		uint8_t Control_Limits(void);










		// Scale Functions
		uint8_t Set_Harmonic(uint32_t _Harmonic);

		// Bucket Functions
		bool Set_Bucket(void);
		uint32_t Get_Bucket_LOW(void);
		uint32_t Get_Bucket_HIGH(void);

		// Limit Functions
		float Limit(uint8_t _Limit_ID, float _Value);

		// Data Refresh Functions	
		uint32_t Get_Frame(void);
		uint32_t Get_Cycle(void);
		uint32_t Get_Divisor(void);
		uint32_t Get_Samples(void);


		float Voltage_HPF_COEF(float _COEF);
		float Current_HPF_COEF(float _COEF);
		float Current_RMS_Offset(char Phase, float _Offset);
		float Active_Power_Offset(char Phase, float _Offset);
		float ReActive_Power_Offset(char Phase, float _Offset);

		// Temperature Functions
		float IC_Temperature(void);

		// Voltage Measurements
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


	private:

		// Stream Object Definition
		Stream *_Energy_Serial;

		// Device Register Structure
		struct Register {
			const uint8_t High_Address;
			const uint8_t Low_Address;
			const uint8_t Data_Type;
		};

		// Scale Functions
		bool Write_VScale(uint32_t _Voltage_Scale);
		bool Write_IScale(uint32_t _Current_Scale);

		// Communication Functions
		uint32_t Read_Baud(void);
		bool Write_Baud(uint32_t _Baud);

		// Calibration Functions
		bool Voltage_Calibration(const char _Phase, const float _Gain, const float _Offset);
		bool Current_Calibration(const char _Phase, const float _Gain, const float _Offset);
		bool Temperature_Calibration(const float _Gain, const float _Offset);

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

};

extern MAX78630 Energy_Analayser;

#endif /* defined(__MAX78630__) */