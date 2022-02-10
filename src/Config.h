#ifndef __MAX78630_CONFIG__
#define __MAX78630_CONFIG__

// Define Serial Port
#define Energy_Serial		       	Serial2
#define Energy_Serial_Baud          38400

// Define Version
#define Version				        "04.00.04"

// Define Default Read Count
#define Read_Count                  3
#define Read_Average_Type           1

// Limit Definations
#define _V_Scale					667
#define _C_Scale					7
#define _V_RMS_Min					220
#define _V_RMS_Max					250
#define _Fq_Min						48
#define _Fq_Max						51
#define _Temp_Min					20
#define _Temp_Max					40
#define _C_Max						5
#define _V_Max_Imb					0.06
#define _C_Max_Imb					0.06
#define _PF_Min	    				0.8

// Wh Bucket Defination
#define _BUCKET_LOW                 0xD0F4C2
#define _BUCKET_HIGH                0x000821

#endif
