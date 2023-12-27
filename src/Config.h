// Device Defaults
#define __MAX78630_Firmware__			    (uint32_t)0x0004D912

// Measure Parameter Defaults
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

// Config Defaults
#ifndef __MAX78630_Config_VScale__
    #define __MAX78630_Config_VScale__			(uint16_t)667
#endif
#ifndef __MAX78630_Config_IScale__
    #define __MAX78630_Config_IScale__			(uint16_t)7
#endif
#ifndef __MAX78630_Config_BucketH__
    #define __MAX78630_Config_BucketH__			(uint32_t)0x000821
#endif
#ifndef __MAX78630_Config_BucketL__
    #define __MAX78630_Config_BucketL__			(uint32_t)0xD0F4C2
#endif
#ifndef __MAX78630_Config_Status_Sticky__
    #define __MAX78630_Config_Status_Sticky__	(bool)false
#endif

// Limit Defaults
#ifndef __MAX78630_Limit_Voltage_Min__
    #define __MAX78630_Limit_Voltage_Min__		(float)190
#endif
#ifndef __MAX78630_Limit_Voltage_Max__
    #define __MAX78630_Limit_Voltage_Max__		(float)253
#endif
#ifndef __MAX78630_Limit_Current_Max__
    #define __MAX78630_Limit_Current_Max__		(float)5
#endif
#ifndef __MAX78630_Limit_Frequency_Min__
    #define __MAX78630_Limit_Frequency_Min__	(float)47
#endif
#ifndef __MAX78630_Limit_Frequency_Max__
    #define __MAX78630_Limit_Frequency_Max__	(float)52
#endif
#ifndef __MAX78630_Limit_Temperature_Min__
    #define __MAX78630_Limit_Temperature_Min__	(float)10
#endif
#ifndef __MAX78630_Limit_Temperature_Max__
    #define __MAX78630_Limit_Temperature_Max__	(float)40
#endif
#ifndef __MAX78630_Limit_VImb_Max__
    #define __MAX78630_Limit_VImb_Max__			(float)0.06
#endif
#ifndef __MAX78630_Limit_IImb_Max__
    #define __MAX78630_Limit_IImb_Max__			(float)0.06
#endif
#ifndef __MAX78630_Limit_VSag_Lim__
    #define __MAX78630_Limit_VSag_Lim__			(float)0.9
#endif

