/**
 * @brief Library Scope
 */
// #define __MAX78630_Scope_Utils__
// #define __MAX78630_Scope_Measure_Detail__

/**
 * @brief Measure Parameter Defaults
 */
#define __MAX78630_Measure_Voltage__
#define __MAX78630_Measure_Current__
#define __MAX78630_Measure_Frequency__
#define __MAX78630_Measure_PowerFactor__
#define __MAX78630_Measure_Power__
#define __MAX78630_Measure_Energy__
#define __MAX78630_Measure_Temperature__
#define __MAX78630_Measure_LimitDefect__
#define __MAX78630_Measure_MinMax__
#define __MAX78630_Measure_Calibration__

/**
 * @brief Config Defaults
 */
#define __MAX78630_Config_VScale__			(uint16_t)667
#define __MAX78630_Config_IScale__			(uint16_t)7
#define __MAX78630_Config_BucketH__			(uint32_t)0x000821
#define __MAX78630_Config_BucketL__			(uint32_t)0xD0F4C2
#define __MAX78630_Config_Status_Sticky__	(bool)false

/**
 * @brief Limit Defaults
 */
#define __MAX78630_Limit_Voltage_Min__		(float)190
#define __MAX78630_Limit_Voltage_Max__		(float)253
#define __MAX78630_Limit_Current_Max__		(float)5
#define __MAX78630_Limit_Frequency_Min__	(float)47
#define __MAX78630_Limit_Frequency_Max__	(float)52
#define __MAX78630_Limit_Temperature_Min__	(float)10
#define __MAX78630_Limit_Temperature_Max__	(float)40
#define __MAX78630_Limit_VImb_Max__			(float)0.06
#define __MAX78630_Limit_IImb_Max__			(float)0.06

