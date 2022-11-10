// Phase Type
#define __Phase_R__					(uint8_t)1
#define __Phase_S__					(uint8_t)2
#define __Phase_T__					(uint8_t)3
#define __Phase_Avg__				(uint8_t)4
#define __Phase_Any__				(uint8_t)5

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
#define __Active_Received__ 		(uint8_t)12
#define __Active_Delivered__ 		(uint8_t)13
#define __ReActive_Received__ 		(uint8_t)14
#define __ReActive_Delivered__ 		(uint8_t)15
#define __Energy_Reset__ 			(uint8_t)16
#define __Phase_Compensation__ 		(uint8_t)16

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
#define __VSAG_LIM__ 				(uint8_t)11

// Defect Bit Definitions
#define __OV_FREQ__ 				(uint8_t)22
#define __UN_FREQ__ 				(uint8_t)21
#define __OV_TEMP__ 				(uint8_t)20
#define __UN_TEMP__ 				(uint8_t)19
#define __OV_VRMSC__ 				(uint8_t)18
#define __UN_VRMSC__ 				(uint8_t)17
#define __OV_VRMSB__ 				(uint8_t)16
#define __UN_VRMSB__ 				(uint8_t)15
#define __OV_VRMSA__ 				(uint8_t)14
#define __UN_VRMSA__ 				(uint8_t)13
#define __UN_PFC__ 		    		(uint8_t)12
#define __UN_PFB__ 		    		(uint8_t)11
#define __UN_PFA__ 		    		(uint8_t)10
#define __OV_IRMSC__ 				(uint8_t)9
#define __OV_IRMSB__ 				(uint8_t)8
#define __OV_IRMSA__ 				(uint8_t)7
#define __VC_SAG__ 		    		(uint8_t)6
#define __VB_SAG__ 		    		(uint8_t)5
#define __VA_SAG__ 		    		(uint8_t)4
#define __V_IMBAL__ 				(uint8_t)3
#define __I_IMBAL__ 				(uint8_t)2

/**
 * @brief IC Register Address Structure
 */
struct Register {
    const uint8_t High_Address;
    const uint8_t Low_Address;
    const uint8_t Data_Type;
};
