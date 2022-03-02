#ifndef __MAX78630_DEFINATION__
#define __MAX78630_DEFINATION__

// --------------------------------------------------------------------------------------------------------------------------
// Master Packets
// --------------------------------------------------------------------------------------------------------------------------
// Master packets always start with the 1-byte header (0xAA) for synchronization purposes. The master 
// then sends the byte count of the entire packet (up to 255 byte packets) followed by the payload 
// (up to 253 bytes) and a 1-byte modulo-256 checksum of all packet bytes for data integrity checking.
//
// [Header 0xAA] - [Byte Count] - [Payload] - [CheckSum]
//
// The payload can contain either a single command or multiple commands if the target is already selected. 
// It can also include device addresses, register addresses, and data. All multi-byte payloads are sent 
// and received least-significant-byte first.
// 
// |-------------|--------------------------|--------------------------------------------------|
// | Command     | Parameters               | Description                                      |
// |-------------|--------------------------|--------------------------------------------------|
// | 0x00 - 0x7F |                          | Invalid                                          |
// | 0x80 - 0x9F |                          | Not Used                                         |
// |    0xA0     |                          | Clear Address                                    |
// |    0xA1     | [Byte Low]               | Set Read/Write address bits [7:0]                |
// |    0xA2     | [Byte High]              | Set Read/Write address bits [15:8]               |
// |    0xA3     | [Byte High] - [Byte Low] | Set Read/Write address bits [15:0]               |
// | 0xA4 - 0xAF |                          | Reserved for larger address targets              |
// | 0xB0 - 0xBF |                          | Not Used                                         |
// |    0xC0     |                          | De-select Target (target will Acknowledge)       |
// | 0xC1 - 0xCE |                          | Select target 1 to 14 (target will Acknowledge)  |
// |    0xCF     | [Byte]                   | Select target 0 to 255 (target will Acknowledge) |
// |    0xD0     | [Data..]                 | Write bytes set by remainder of Byte Count       |
// | 0xD1 - 0xDF | [Data..]                 | Write 1 to 15 bytes                              |
// |    0xE0     | [Byte]                   | Read 0 to 255 bytes                              |
// | 0xE1 - 0xEF |                          | Read 1 to 15 bytes                               |
// | 0xF0 - 0xFF |                          | Not Used                                         |
// |-------------|--------------------------|--------------------------------------------------|
//
// Users only need to implement commands they actually need or intend to use. For example, only one address 
// command is required – either 0xA1 for systems with 8 address bits or less or 0xA3 for systems with 9 to 
// 16 address bits. Likewise, only one write, read, or select target command needs to be implemented. Select 
// Target is not needed in systems with only one target.
//
// Command Payload Examples
//
// Device Selection
// [0xCF] - [SSI ID]
//
// Register Address Pointer Selection
// [0xA3] - [Register Address (2 Bytes)]
//
// Small Read Command (3 bytes)
// [0xE3]
//
// Large Read Command (30 bytes)
// [0xE0] - [0x1E]
//
// Small Write Command (3 bytes)
// [0xD3] - [3 Bytes of Data]
//
// Large Write Command (30 bytes)
// [0x21] - [0xD0] - [30 Bytes of Data]
//
// --------------------------------------------------------------------------------------------------------------------------

// Pack Byte Definitions
#define Pack_Header			(uint8_t)0xAA
#define Pack_Device_Select	(uint8_t)0xCF
#define Pack_Register		(uint8_t)0xA3
#define Pack_Small_Read		(uint8_t)0xE3
#define Pack_Large_Read		(uint8_t)0xE0
#define Pack_Small_Write	(uint8_t)0xD3
#define Pack_Lager_Write	(uint8_t)0x21

enum STATUS_Bits {
	RESET 			= 0,
	XSTATE			= 1,
	I_IMBAL			= 2,
	V_IMBAL			= 3,
	VA_SAG			= 4,
	VB_SAG			= 5,
	VC_SAG			= 6,
	OV_IRMSA		= 7,
	OV_IRMSB		= 8,
	OV_IRMSC		= 9,
	UN_PFA			= 10,
	UN_PFB			= 11,
	UN_PFC			= 12,
	UN_VRMSA		= 13,
	OV_VRMSA		= 14,
	UN_VRMSB		= 15,
	OV_VRMSB		= 16,
	UN_VRMSC		= 17,
	OV_VRMSC		= 18,
	UN_TEMP			= 19,
	OV_TEMP			= 20,
	UN_FREQ			= 21,
	OV_FREQ			= 22,
	DRDY			= 23
};

struct Register {
	const uint8_t High_Address;
	const uint8_t Low_Address;
	const uint8_t Data_Type;
	const bool NonVolatile;
};

struct System_Limits {
	bool Current_Imbalance;
	bool Voltage_Imbalance;
	bool Sag_VR;
	bool Sag_VS;
	bool Sag_VT;
	bool Current_Over_Limit_VR;
	bool Current_Over_Limit_VS;
	bool Current_Over_Limit_VT;
	bool Power_Factor_Under_Limit_VR;
	bool Power_Factor_Under_Limit_VS;
	bool Power_Factor_Under_Limit_VT;
	bool Voltage_Under_Limit_VR;
	bool Voltage_Under_Limit_VS;
	bool Voltage_Under_Limit_VT;
	bool Voltage_Over_Limit_VR;
	bool Voltage_Over_Limit_VS;
	bool Voltage_Over_Limit_VT;
	bool Temperature_Under_Limit;
	bool Temperature_Over_Limit;
	bool Frequency_Under_Limit;
	bool Frequency_Over_Limit;
};

#endif
