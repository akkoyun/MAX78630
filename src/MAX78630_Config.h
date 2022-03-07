#ifndef __MAX78630_CONFIG__
#define __MAX78630_CONFIG__

// Define Version
#define MAX78630_Version	        "04.02.10"

// Define Default Read Count
#define Read_Count                  3
#define Read_Average_Type           1

// Limit Definition
#define _V_Scale					667
#define _C_Scale					7
#define _V_RMS_Min					192
#define _V_RMS_Max					253
#define _Fq_Min						47
#define _Fq_Max						52
#define _Temp_Min					10
#define _Temp_Max					40
#define _C_Max						5
#define _V_Max_Imb					0.06
#define _C_Max_Imb					0.06
#define _PF_Min	    				0.8

// Wh Bucket Definition
#define _BUCKET_LOW                 0xD0F4C2
#define _BUCKET_HIGH                0x000821

#endif
