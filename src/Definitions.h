// Command Codes
#define __MAX78630_HEADER__				(uint8_t)0xAA
#define __MAX78630_CLEAR_ADRS__			(uint8_t)0xA0
#define __MAX78630_RW_ADRS_LO_BYTE__	(uint8_t)0xA1
#define __MAX78630_RW_ADRS_HI_BYTE__	(uint8_t)0xA2
#define __MAX78630_RW_ADRS__			(uint8_t)0xA3
#define __MAX78630_DE_SELECT_DEVICE__	(uint8_t)0xC0
#define __MAX78630_SELECT_DEVICE__		(uint8_t)0xCF
#define __MAX78630_WRITE_BYTES__		(uint8_t)0xD0
#define __MAX78630_WRITE_3_BYTES__		(uint8_t)0xD3
#define __MAX78630_READ_BYTES__			(uint8_t)0xE0
#define __MAX78630_READ_3_BYTES__		(uint8_t)0xE3

// Response Codes
#define __MAX78630_ACK_DATA__			(uint8_t)0xAA
#define __MAX78630_AUTO_REPORT__		(uint8_t)0xAE
#define __MAX78630_ACK_NO_DATA__		(uint8_t)0xAD
#define __MAX78630_NACK__				(uint8_t)0xB0
#define __MAX78630_BAD_CMD__			(uint8_t)0xBC
#define __MAX78630_CHK_SUM_BAD__		(uint8_t)0xBD
#define __MAX78630_BUFF_OVRFLW__		(uint8_t)0xBF

// Payload Size Codes
#define __MAX78630_1_BYTE__				(uint8_t)0x01
#define __MAX78630_2_BYTES__			(uint8_t)0x02
#define __MAX78630_3_BYTES__			(uint8_t)0x03
#define __MAX78630_4_BYTES__			(uint8_t)0x04
#define __MAX78630_5_BYTES__			(uint8_t)0x05
#define __MAX78630_6_BYTES__			(uint8_t)0x06
#define __MAX78630_7_BYTES__			(uint8_t)0x07
#define __MAX78630_8_BYTES__			(uint8_t)0x08
#define __MAX78630_9_BYTES__			(uint8_t)0x09
#define __MAX78630_10_BYTES__			(uint8_t)0x0A

// Function Definitions
#define __MAX78630_CLEAR__ 				(uint8_t)0
#define __MAX78630_GET__ 				(uint8_t)1
#define __MAX78630_SET__ 				(uint8_t)2

// Phase Type
#define __Phase_R__						(uint8_t)1
#define __Phase_S__						(uint8_t)2
#define __Phase_T__						(uint8_t)3
#define __Phase_Avg__					(uint8_t)4
#define __Phase_Any__					(uint8_t)5

// Measurement Type
#define __RMS__ 						(uint8_t)1
#define __Instant__ 					(uint8_t)2
#define __Fundamental__ 				(uint8_t)3
#define __Harmonic__ 					(uint8_t)4
#define __Peak__ 						(uint8_t)5
#define __Active__ 						(uint8_t)6
#define __ReActive__ 					(uint8_t)7
#define __Apparent__ 					(uint8_t)8
#define __Fund_ReActive__ 				(uint8_t)9
#define __Harm_ReActive__ 				(uint8_t)10
#define __Fund_VA__ 					(uint8_t)11
#define __Active_Received__ 			(uint8_t)12
#define __Active_Delivered__ 			(uint8_t)13
#define __ReActive_Received__ 			(uint8_t)14
#define __ReActive_Delivered__ 			(uint8_t)15
#define __Energy_Reset__ 				(uint8_t)16
#define __Phase_Compensation__ 			(uint8_t)17

// Calibration Type
#define __Voltage__ 					(uint8_t)1
#define __Current__ 					(uint8_t)2
#define __Temperature__ 				(uint8_t)3
#define __Active_Power__ 				(uint8_t)4
#define __ReActive_Power__ 				(uint8_t)5

// Limit Type
#define __VRMS_MIN__ 					(uint8_t)1
#define __VRMS_MAX__ 					(uint8_t)2
#define __T_MIN__ 						(uint8_t)3
#define __T_MAX__ 						(uint8_t)4
#define __F_MIN__ 						(uint8_t)5
#define __F_MAX__ 						(uint8_t)6
#define __IRMS_MAX__ 					(uint8_t)7
#define __PF_MIN__ 						(uint8_t)8
#define __VIMB_MAX__ 					(uint8_t)9
#define __IIMB_MAX__ 					(uint8_t)10
#define __VSAG_LIM__ 					(uint8_t)11

// Mask Definitions
#define __MASK_AL1__ 					(uint8_t)0x01
#define __MASK_AL2__ 					(uint8_t)0x02

// Alarm Type Definitions
#define __ALARM_LOW_VOLTAGE__	 		(uint16_t)1
#define __ALARM_HIGH_VOLTAGE__			(uint16_t)2
#define __ALARM_HIGH_CURRENT__ 			(uint16_t)3
#define __ALARM_LOW_FREQUENCY__ 		(uint16_t)4
#define __ALARM_HIGH_FREQUENCY__		(uint16_t)5
#define __ALARM_VOLTAGE_IMBAL__			(uint16_t)6
#define __ALARM_CURRENT_IMBAL__ 		(uint16_t)7
#define __ALARM_LOW_TEMPERATURE__		(uint16_t)8
#define __ALARM_HIGH_TEMPERATURE__ 		(uint16_t)9
#define __ALARM_LOW_POWERFACTOR__		(uint16_t)10

// Defect Bit Definitions
#define __BIT_OV_FREQ__ 				(uint8_t)22
#define __BIT_UN_FREQ__ 				(uint8_t)21
#define __BIT_OV_TEMP__ 				(uint8_t)20
#define __BIT_UN_TEMP__ 				(uint8_t)19
#define __BIT_OV_VRMSC__ 				(uint8_t)18
#define __BIT_UN_VRMSC__ 				(uint8_t)17
#define __BIT_OV_VRMSB__ 				(uint8_t)16
#define __BIT_UN_VRMSB__ 				(uint8_t)15
#define __BIT_OV_VRMSA__ 				(uint8_t)14
#define __BIT_UN_VRMSA__ 				(uint8_t)13
#define __BIT_UN_PFC__ 					(uint8_t)12
#define __BIT_UN_PFB__ 					(uint8_t)11
#define __BIT_UN_PFA__ 					(uint8_t)10
#define __BIT_OV_IRMSC__ 				(uint8_t)9
#define __BIT_OV_IRMSB__ 				(uint8_t)8
#define __BIT_OV_IRMSA__ 				(uint8_t)7
#define __BIT_VC_SAG__ 		   			(uint8_t)6
#define __BIT_VB_SAG__ 		   			(uint8_t)5
#define __BIT_VA_SAG__ 		   			(uint8_t)4
#define __BIT_V_IMBAL__ 				(uint8_t)3
#define __BIT_I_IMBAL__ 				(uint8_t)2

// Monitor Address Definitions
#define __MAX78630_MONITOR_1__ 			(uint8_t)1
#define __MAX78630_MONITOR_2__ 			(uint8_t)2
#define __MAX78630_MONITOR_3__ 			(uint8_t)3
#define __MAX78630_MONITOR_4__ 			(uint8_t)4
#define __MAX78630_MONITOR_5__ 			(uint8_t)5
#define __MAX78630_MONITOR_6__ 			(uint8_t)6
#define __MAX78630_MONITOR_7__ 			(uint8_t)7
#define __MAX78630_MONITOR_8__ 			(uint8_t)8

// Alarm Mask Definitions
#define __MAX78630_ALARM_1__ 			(uint8_t)1
#define __MAX78630_ALARM_2__ 			(uint8_t)2