// Device Defaults
#define __MAX78630_Firmware__			            (uint32_t)0x0004D912

// Config Defaults
#ifndef __MAX78630_Config_VScale__
    #define __MAX78630_Config_VScale__			    (uint16_t)667
#endif
#ifndef __MAX78630_Config_IScale__
    #define __MAX78630_Config_IScale__			    (uint16_t)7
#endif
#ifndef __MAX78630_Config_BucketH__
    #define __MAX78630_Config_BucketH__			    (uint32_t)0x000821
#endif
#ifndef __MAX78630_Config_BucketL__
    #define __MAX78630_Config_BucketL__			    (uint32_t)0xD0F4C2
#endif
#ifndef __MAX78630_Config_Status_Sticky__
    #define __MAX78630_Config_Status_Sticky__	    (bool)false
#endif

// Limit Defaults

// Min Voltage
#ifndef __MAX78630_Limit_Voltage_Min__
    #define __MAX78630_Limit_Voltage_Min__		    (float)190
#endif
#ifndef __MAX78630_Limit_Voltage_Min_Diff__
    #define __MAX78630_Limit_Voltage_Min_Diff__		(float)10
#endif

// Max Voltage
#ifndef __MAX78630_Limit_Voltage_Max__
    #define __MAX78630_Limit_Voltage_Max__		    (float)253
#endif
#ifndef __MAX78630_Limit_Voltage_Max_Diff__
    #define __MAX78630_Limit_Voltage_Max_Diff__		(float)10
#endif

// Max Current
#ifndef __MAX78630_Limit_Current_Max__
    #define __MAX78630_Limit_Current_Max__		    (float)5
#endif
#ifndef __MAX78630_Limit_Current_Max_Diff__
    #define __MAX78630_Limit_Current_Max_Diff__	    (float)0.5
#endif

// Min Frequency
#ifndef __MAX78630_Limit_Frequency_Min__
    #define __MAX78630_Limit_Frequency_Min__	    (float)47
#endif
#ifndef __MAX78630_Limit_Frequency_Min_Diff__
    #define __MAX78630_Limit_Frequency_Min_Diff__	(float)1
#endif

// Max Frequency
#ifndef __MAX78630_Limit_Frequency_Max__
    #define __MAX78630_Limit_Frequency_Max__	    (float)52
#endif
#ifndef __MAX78630_Limit_Frequency_Max_Diff__
    #define __MAX78630_Limit_Frequency_Max_Diff__	(float)1
#endif

// Min Temperature
#ifndef __MAX78630_Limit_Temperature_Min__
    #define __MAX78630_Limit_Temperature_Min__	    (float)10
#endif
#ifndef __MAX78630_Limit_Temperature_Min_Diff__
    #define __MAX78630_Limit_Temperature_Min_Diff__	(float)2
#endif

// Max Temperature
#ifndef __MAX78630_Limit_Temperature_Max__
    #define __MAX78630_Limit_Temperature_Max__	    (float)35
#endif
#ifndef __MAX78630_Limit_Temperature_Max_Diff__
    #define __MAX78630_Limit_Temperature_Max_Diff__	(float)2
#endif

// Max Voltage Imbalance
#ifndef __MAX78630_Limit_VImb_Max__
    #define __MAX78630_Limit_VImb_Max__			    (float)0.06
#endif
#ifndef __MAX78630_Limit_VImb_Max_Diff__
    #define __MAX78630_Limit_VImb_Max_Diff__		(float)0.01
#endif

// Max Current Imbalance
#ifndef __MAX78630_Limit_IImb_Max__
    #define __MAX78630_Limit_IImb_Max__			    (float)0.06
#endif
#ifndef __MAX78630_Limit_IImb_Max_Diff__
    #define __MAX78630_Limit_IImb_Max_Diff__		(float)0.01
#endif

// Max Voltage Sag
#ifndef __MAX78630_Limit_VSag_Lim__
    #define __MAX78630_Limit_VSag_Lim__			    (float)0.9
#endif

// Min Max Monitor Settings

// Monitor 1
#ifndef __MAX78630_Monitor_1_Type__
    #define __MAX78630_Monitor_1_Type__		        (uint8_t)0x30   // 0x30 = VRMS_R
#endif

// Monitor 2
#ifndef __MAX78630_Monitor_2_Type__
    #define __MAX78630_Monitor_2_Type__		        (uint8_t)0x31   // 0x31 = VRMS_S
#endif

// Monitor 3
#ifndef __MAX78630_Monitor_3_Type__
    #define __MAX78630_Monitor_3_Type__		        (uint8_t)0x32   // 0x32 = VRMS_T
#endif

// Monitor 4
#ifndef __MAX78630_Monitor_4_Type__
    #define __MAX78630_Monitor_4_Type__		        (uint8_t)0x47   // 0x47 = IRMS_R
#endif

// Monitor 5
#ifndef __MAX78630_Monitor_5_Type__
    #define __MAX78630_Monitor_5_Type__		        (uint8_t)0x48   // 0x48 = IRMS_S
#endif

// Monitor 6
#ifndef __MAX78630_Monitor_6_Type__
    #define __MAX78630_Monitor_6_Type__		        (uint8_t)0x49   // 0x49 = IRMS_T
#endif

// Monitor 7
#ifndef __MAX78630_Monitor_7_Type__
    #define __MAX78630_Monitor_7_Type__		        (uint8_t)0x7A   // 0x7A = PF_T
#endif

// Monitor 8
#ifndef __MAX78630_Monitor_8_Type__
    #define __MAX78630_Monitor_8_Type__		        (uint8_t)0x80   // 0x80 = FREQ
#endif