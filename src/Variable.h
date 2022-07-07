// Define Arduino Library
#ifndef __Arduino__
	#include <Arduino.h>
#endif

/**
 * @brief Measurement Structure update
 */
#define Measurement_Frequency
#define Measurement_Voltage_R
#define Measurement_Voltage_S
#define Measurement_Voltage_T
#define Measurement_Current_R
#define Measurement_Current_S
#define Measurement_Current_T
//#define Measurement_PowerFactor_R
//#define Measurement_PowerFactor_S
//#define Measurement_PowerFactor_T
#define Measurement_PowerFactor_A
#define Measurement_Energy_Active
#define Measurement_Energy_ReActive

/**
 * @brief Limit Defaults Definition.
 */
#define Limit_Voltage_Min       (float)192
#define Limit_Voltage_Max       (float)253
#define Limit_Frequency_Min     (float)47
#define Limit_Frequency_Max     (float)52
#define Limit_Voltage_IMB       (float)0.06
#define Limit_Current_IMB		(float)0.06
#define Bucket_High				(uint32_t)0x000821
#define Bucket_Low				(uint32_t)0xD0F4C2

