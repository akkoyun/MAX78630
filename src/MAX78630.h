#ifndef __MAX78630__
#define __MAX78630__

	// Define Arduino Library
	#ifndef __Arduino__
		#include <Arduino.h>
	#endif

	// Include External Files
	#include "Definitions.h"
	#include "Config.h"

	// MAX78630 Class
	class MAX78630 {

		// Private Context
		private:

			// Serial Stream Pointer
			Stream * Serial_Energy;

			// Device Structure Definition
			struct Device_Struct {
				bool Found = false;
				uint32_t Status = 0;
			} Device;

			// IC Register Address Structure
			struct Register {
				const uint8_t High_Address;
				const uint8_t Low_Address;
				const uint8_t Data_Type;
			};

			// Clear Serial Buffer Function
			void Clear_Buffer(void) {

				// Flush UART Buffer
				Serial_Energy->flush();
				
				// Clear UART Buffer
				while(Serial_Energy->available() > 0) {
					
					// Read Serial Char
					Serial_Energy->read(); 
					
					// Stream Delay
					delay(5);

				}

			}

			// Register Read Function.
			double Register_Pointer_Read(Register _Command) {

				/*
					MAX78630 Serial Communication Read Values Structure
					---------------------------------------------------
					Read Request : [1]-[2]-[3]-[4]-[5]-[6]-[7]
					---------------------------------------------------
					1. byte is the IC address byte (0xAA)
					2. byte is the total sended byte (0x07)
					3. byte is the package payload type (0xA3)
					4. byte is the request (RMS) byte (2)
					5. byte is the request (RMS) byte (1)
					6. byte is the requested byte count (0xE3)
					7. byte is the CRC correction byte (CHK)
				*/

				// Clear Serial Buffer
				this->Clear_Buffer();

				// Command Send Delay
				delay(5);

				// Calculate CheckSum
				uint8_t _Request_CheckSum = 0x100 - ((0xAA + 0x07 + 0xA3 + _Command.Low_Address + _Command.High_Address + 0xE3) % 256);

				// Send Command
				Serial_Energy->write(0xAA);
				Serial_Energy->write(0x07);
				Serial_Energy->write(0xA3);
				Serial_Energy->write(_Command.Low_Address);
				Serial_Energy->write(_Command.High_Address);
				Serial_Energy->write(0xE3);
				Serial_Energy->write(_Request_CheckSum);

				// Command Send Delay
				delay(20);

				// Declare Variable
				uint8_t _Response[6];
				memset(_Response, '\0', 6);
				uint8_t _Response_Order = 0;

				// Read UART Response
				while(Serial_Energy->available() > 0) {

					// Read Serial Char
					_Response[_Response_Order] = Serial_Energy->read();
					
					// Increase Read Order
					_Response_Order++;
					
					// Stream Delay
					delay(5);
					
				}

				// Calculate Response CheckSum
				uint8_t _Response_CheckSum = 0x100 - ((_Response[0] + _Response[1] + _Response[2] + _Response[3] + _Response[4]) % 256);

				// Declare Raw Data Variable
				uint32_t _Data_RAW = 0;
				double _Data_SUM = 0;

				// Control Received Data
				if (_Response[0] == 0xAA and _Response[1] == 0x06 and _Response[5] == _Response_CheckSum) {

					// Combine Read Bytes
					_Data_RAW = ((uint32_t)(_Response[4]) << 16 | ((uint32_t)_Response[3]) << 8 | ((uint32_t)_Response[2]));

					// Calculate Response
					if (_Command.Data_Type == 0) {

						// Handle Response (int)	
						_Data_SUM = _Data_RAW;
						
					} else {

						// Handle Bits
						for (uint8_t i = 0; i < 23; i++) if (bitRead(_Data_RAW, i) == true) _Data_SUM += pow(2, (i - _Command.Data_Type));
						if (bitRead(_Data_RAW, 23) == true) _Data_SUM += (-1 * pow(2, (23 - _Command.Data_Type)));

					}

				}

				// End Function
				return(_Data_SUM);
				
			}

			// Write Register Function
			bool Register_Pointer_Set(Register _Command, const uint32_t _Data) {

				// Clear Buffer
				this->Clear_Buffer();

				// Convert Parameters
				uint8_t _Parameter1 = _Data;
				uint8_t _Parameter2 = _Data >> 8;
				uint8_t _Parameter3 = _Data >> 16;

				// Send Command
				Serial_Energy->write(0xAA);
				Serial_Energy->write(0x0A);
				Serial_Energy->write(0xA3);
				Serial_Energy->write(_Command.Low_Address);
				Serial_Energy->write(_Command.High_Address);
				Serial_Energy->write(0xD3);
				Serial_Energy->write(_Parameter1);
				Serial_Energy->write(_Parameter2);
				Serial_Energy->write(_Parameter3);
				Serial_Energy->write(0x100 - ((0xAA + 0x0A + 0xA3 + _Command.Low_Address + _Command.High_Address + 0xD3 + _Parameter1 + _Parameter2 + _Parameter3) % 256));

				// Command Delay
				delay(20);

				// End Function
				if (Serial_Energy->read() == 0xAD) return(true);

				// End Function
				return(false);
				
			}

			// Select IC Function
			bool Select_IC(uint8_t _Address) {

				// Command Send Delay
				delay(5);

				// Send Command
				Serial_Energy->write(0xAA);											// Header (0xAA)
				Serial_Energy->write(0x04);											// Total Sended Byte (0x04)
				Serial_Energy->write(_Address);										// IC Address (0xC1, 0xC2, 0xC3, 0xC4)
				Serial_Energy->write(0x100 - ((0xAA + 0x04 + _Address) % 256));		// CheckSum (0x__)

				// Command Send Delay
				delay(20);

				// End Function
				if (Serial_Energy->read() == 0xAD) return(true);

				// End Function
				return(false);

			}

			// Set Voltage Scale Function
			bool Set_VScale(const uint32_t _Voltage_Scale) {

				// Define Register
				Register VSCALE {0x01, 0x44, 0}; // Scratch register (see Scaling Registers section)

				// Set Register
				return(this->Register_Pointer_Set(VSCALE, _Voltage_Scale));

			}

			// Set Current Scale Function
			bool Set_IScale(const uint32_t _Current_Scale) {

				// Define Register
				Register IFSCALE {0x01, 0x41, 0}; // Scratch register (see Scaling Registers section)

				// Set Register
				return(this->Register_Pointer_Set(IFSCALE, _Current_Scale));

			}

			// Bucket Size Set Function.
			bool Set_Bucket(const uint32_t _Bucket_H, const uint32_t _Bucket_L) {

				// Define Register
				Register BUCKET_HIGH {0x01,	0xD4, 0}; // Energy Bucket Size – High word
				Register BUCKET_LOW {0x01, 0xD1, 0}; // Energy Bucket Size – Low word

				// End Function
				return(this->Register_Pointer_Set(BUCKET_LOW, _Bucket_L) && this->Register_Pointer_Set(BUCKET_HIGH, _Bucket_H));

			}

			// Set Min-Max Control Address
			bool Set_Min_Max_Address(const uint8_t _MM_ADDR, const uint32_t _Mask) {

				// Decide Address
				switch (_MM_ADDR) {

					case __MAX78630_MONITOR_1__: {

						// Define Register
						Register MMADDR0 {0x01, 0xB9, 0}; // Min/Max Monitor address 1

						// Get Register
						return(this->Register_Pointer_Set(MMADDR0, _Mask));

						// End Case
						break;

					}
					case __MAX78630_MONITOR_2__: {

						// Define Register
						Register MMADDR1 {0x01, 0xBC, 0}; // Min/Max Monitor address 2

						// Get Register
						return(this->Register_Pointer_Set(MMADDR1, _Mask));

						// End Case
						break;

					}
					case __MAX78630_MONITOR_3__: {

						// Define Register
						Register MMADDR2 {0x01, 0xBF, 0}; // Min/Max Monitor address 3

						// Get Register
						return(this->Register_Pointer_Set(MMADDR2, _Mask));

						// End Case
						break;

					}
					case __MAX78630_MONITOR_4__: {

						// Define Register
						Register MMADDR3 {0x01, 0xC2, 0}; // Min/Max Monitor address 4

						// Get Register
						return(this->Register_Pointer_Set(MMADDR3, _Mask));

						// End Case
						break;

					}
					case __MAX78630_MONITOR_5__: {

						// Define Register
						Register MMADDR4 {0x01, 0xC5, 0}; // Min/Max Monitor address 5

						// Get Register
						return(this->Register_Pointer_Set(MMADDR4, _Mask));

						// End Case
						break;

					}
					case __MAX78630_MONITOR_6__: {

						// Define Register
						Register MMADDR5 {0x01, 0xC8, 0}; // Min/Max Monitor address 6

						// Get Register
						return(this->Register_Pointer_Set(MMADDR5, _Mask));

						// End Case
						break;

					}
					case __MAX78630_MONITOR_7__: {

						// Define Register
						Register MMADDR6 {0x01, 0xCB, 0}; // Min/Max Monitor address 7

						// Get Register
						return(this->Register_Pointer_Set(MMADDR6, _Mask));

						// End Case
						break;

					}
					case __MAX78630_MONITOR_8__: {

						// Define Register
						Register MMADDR7 {0x01, 0xCE, 0}; // Min/Max Monitor address 8

						// Get Register
						return(this->Register_Pointer_Set(MMADDR7, _Mask));

						// End Case
						break;

					}

				}

				// End Function
				return(false);

			}

			// Clear Min Value From Address
			bool Clear_Min_Value(const uint8_t _MM_ADDR) {

				// Decide Address
				switch (_MM_ADDR) {

					case 1: {

						// Define Register
						Register MIN0 {0x01, 0x89, 23}; // Minimum Recorded Value 1

						// Clear Register
						Register_Pointer_Set(MIN0, 0x00);

						// End Function
						return(true);

						// End Case
						break;

					}
					case 2: {

						// Define Register
						Register MIN1 {0x01, 0x8C, 23}; // Minimum Recorded Value 2

						// Clear Register
						Register_Pointer_Set(MIN1, 0x00);

						// End Function
						return(true);

						// End Case
						break;

					}
					case 3: {

						// Define Register
						Register MIN2 {0x01, 0x8F, 23}; // Minimum Recorded Value 3

						// Clear Register
						Register_Pointer_Set(MIN2, 0x00);

						// End Function
						return(true);

						// End Case
						break;

					}
					case 4: {

						// Define Register
						Register MIN3 {0x01, 0x92, 23}; // Minimum Recorded Value 4

						// Clear Register
						Register_Pointer_Set(MIN3, 0x00);

						// End Function
						return(true);

						// End Case
						break;

					}
					case 5: {

						// Define Register
						Register MIN4 {0x01, 0x95, 23}; // Minimum Recorded Value 5

						// Clear Register
						Register_Pointer_Set(MIN4, 0x00);

						// End Function
						return(true);

						// End Case
						break;

					}
					case 6: {

						// Define Register
						Register MIN5 {0x01, 0x98, 23}; // Minimum Recorded Value 6

						// Clear Register
						Register_Pointer_Set(MIN5, 0x00);

						// End Function
						return(true);

						// End Case
						break;

					}
					case 7: {

						// Define Register
						Register MIN6 {0x01, 0x9B, 0}; // Minimum Recorded Value 7

						// Clear Register
						Register_Pointer_Set(MIN6, 0x00);

						// End Function
						return(true);

						// End Case
						break;

					}
					case 8: {

						// Define Register
						Register MIN7 {0x01, 0x9E, 0}; // Minimum Recorded Value 8

						// Clear Register
						Register_Pointer_Set(MIN7, 0x00);

						// End Function
						return(true);

						// End Case
						break;

					}

				}

				// End Function
				return(false);

			}

			// Clear Max Value From Address
			bool Clear_Max_Value(const uint8_t _MM_ADDR) {

				// Decide Address
				switch (_MM_ADDR) {

					case 1: {

						// Define Register
						Register MAX0 {0x01, 0xA1, 23}; // Maximum Recorded Value 1

						// Clear Register
						Register_Pointer_Set(MAX0, 0x00);

						// End Function
						return(true);

						// End Case
						break;

					}
					case 2: {

						// Define Register
						Register MAX1 {0x01, 0xA4, 23}; // Maximum Recorded Value 2

						// Clear Register
						Register_Pointer_Set(MAX1, 0x00);

						// End Function
						return(true);

						// End Case
						break;

					}
					case 3: {

						// Define Register
						Register MAX2 {0x01, 0xA7, 23}; // Maximum Recorded Value 3

						// Clear Register
						Register_Pointer_Set(MAX2, 0x00);

						// End Function
						return(true);

						// End Case
						break;

					}
					case 4: {

						// Define Register
						Register MAX3 {0x01, 0xAA, 23}; // Maximum Recorded Value 4

						// Clear Register
						Register_Pointer_Set(MAX3, 0x00);

						// End Function
						return(true);

						// End Case
						break;

					}
					case 5: {

						// Define Register
						Register MAX4 {0x01, 0xAD, 23}; // Maximum Recorded Value 5

						// Clear Register
						Register_Pointer_Set(MAX4, 0x00);

						// End Function
						return(true);

						// End Case
						break;

					}
					case 6: {

						// Define Register
						Register MAX5 {0x01, 0xB0, 23}; // Maximum Recorded Value 6

						// Clear Register
						Register_Pointer_Set(MAX5, 0x00);

						// End Function
						return(true);

						// End Case
						break;

					}
					case 7: {

						// Define Register
						Register MAX6 {0x01, 0xB3, 0}; // Maximum Recorded Value 7

						// Clear Register
						Register_Pointer_Set(MAX6, 0x00);

						// End Function
						return(true);

						// End Case
						break;

					}
					case 8: {

						// Define Register
						Register MAX7 {0x01, 0xB6, 0}; // Maximum Recorded Value 8

						// Clear Register
						Register_Pointer_Set(MAX7, 0x00);

						// End Function
						return(true);

						// End Case
						break;

					}

				}

				// End Function
				return(false);

			}

			// Set Sticky Function
			bool Set_Sticky(const bool _Sticky) {

				// Define Register
				Register STICKY {0x00, 0x2D, 0}; // Alarm/status bits to hold until cleared by host

				// Get Current Register
				uint32_t _Current_Sticky = this->Register_Pointer_Read(STICKY);

				// Return Result
				if (!_Sticky) return(this->Register_Pointer_Set(STICKY, (_Current_Sticky & 0xFF800003)));
				if (_Sticky) return(this->Register_Pointer_Set(STICKY, (_Current_Sticky | 0x7FFFFC)));

				// End Function
				return(false);

			}

			// Set Alarm Mask
			bool Set_Alarm_Mask(const uint8_t _Mask_Addres, const uint8_t _Mask, const bool _Status) {

				// Decide Address
				switch (_Mask_Addres) {

					case __MAX78630_ALARM_1__: {

						// Define Register
						Register MASK1 {0x00, 0x1E, 0}; // Alarm Mask address 1

						// Read MASK1 Register
						uint32_t _MASK1_Current = this->Register_Pointer_Read(MASK1);

						// Set Mask bit
						if (_Status) {

							// Set Bit
							bitSet(_MASK1_Current, _Mask);

						} else {

							// Clear Bit
							bitClear(_MASK1_Current, _Mask);

						}

						// Write MASK1 Register
						return(this->Register_Pointer_Set(MASK1, _MASK1_Current));

						// End Case
						break;

					}
					case __MAX78630_ALARM_2__: {

						// Define Register
						Register MASK2 {0x00, 0x21, 0}; // Alarm Mask address 2

						// Read MASK2 Register
						uint32_t _MASK2_Current = this->Register_Pointer_Read(MASK2);

						// Set Mask bit
						if (_Status) {

							// Set Bit
							bitSet(_MASK2_Current, _Mask);

						} else {

							// Clear Bit
							bitClear(_MASK2_Current, _Mask);

						}

						// Get Register
						return(this->Register_Pointer_Set(MASK2, _MASK2_Current));

						// End Case
						break;

					}

				}

				// End Function
				return(false);

			}

			// Data Converter Function
			uint32_t FtoS(double _Variable, uint8_t _Data_Type) {

				// Declare Data Variable
				uint32_t _Data = 0x00;

				// Convert Loop
				for (uint8_t i = 0; i < 24; i++) {
					
					// Calculate Bit Multiplier
					double _Bit_Multiplier = pow(2, (-_Data_Type - (i - 23)));

					// Control for Bit
					if (_Variable >= _Bit_Multiplier) {
						
						// Set Bit Control
						bitSet(_Data, 23 - i);

						// Handle Data
						_Variable -= _Bit_Multiplier;

					} else {

						// Set Bit Control
						bitClear(_Data, 23 - i);

					}

				}

				// End Function
				return(_Data);

			}

		// Public Context
		public:

			// Limit Variables
			struct Limit_Struct {
				float V_Min = __MAX78630_Limit_Voltage_Min__;
				float V_Max = __MAX78630_Limit_Voltage_Max__;
				float I_Max = __MAX78630_Limit_Current_Max__;
				float FQ_Min = __MAX78630_Limit_Frequency_Min__;
				float FQ_Max = __MAX78630_Limit_Frequency_Max__;
				float VImb_Max = __MAX78630_Limit_VImb_Max__;
				float IImb_Max = __MAX78630_Limit_IImb_Max__;
				float T_Min = __MAX78630_Limit_Temperature_Min__;
				float T_Max = __MAX78630_Limit_Temperature_Max__;
			} Limit;

			// Constructor
			MAX78630(Stream &_Serial) {
				
				// Set Serial Port
				Serial_Energy = & _Serial;

			}

			// Begin and Set Start Parameters.
			bool Begin(const uint8_t _Address = 4) {

				// Clear Serial Buffer
				this->Clear_Buffer();

				// Declare Device Address
				uint8_t _Device_Address = 0xC0;

				// Set Device Address
				if (_Address == 0) _Device_Address = 0xC0;
				if (_Address == 1) _Device_Address = 0xC1;
				if (_Address == 2) _Device_Address = 0xC2;
				if (_Address == 3) _Device_Address = 0xC3;
				if (_Address == 4) _Device_Address = 0xC4;

				// Control Connection
				if (this->Select_IC(_Device_Address)) {

					// Control Firmware
					if (this->Get_Firmware() == __MAX78630_Firmware__) {

						// Get Status
						this->Get_Status();

						// Set Device Found
						this->Device.Found = true;

					} else {

						// Set Device Found
						this->Device.Found = false;

					}

				} else {

					// Set Device Found
					this->Device.Found = false;

				}

				// End Function
				return(this->Device.Found);

			}

			// Get Device Firmware Function
			uint32_t Get_Firmware(void) {

				// Define Register
				Register FW_VERSION {0x00, 0x03, 0}; // Hardware and firmware version

				// Return Register
				return(this->Register_Pointer_Read(FW_VERSION));

				// Default Firmware: 0x0004D912

			}

			// Setup Device Function
			bool Setup(const bool _Scale = true, const bool _Bucket = true, const bool _Limit = true, const bool _MinMax = true, const bool _Alarm = true) {

				// Control for Device
				if (this->Device.Found) {

					// Set Scale
					if (_Scale) {

						// Set Voltage Scale
						this->Set_VScale(__MAX78630_Config_VScale__);

						// Set Current Scale
						this->Set_IScale(__MAX78630_Config_IScale__);

					}

					// Set Bucket
					if (_Bucket) {

						// Set Bucket Size
						this->Set_Bucket(__MAX78630_Config_BucketH__, __MAX78630_Config_BucketL__);

					}

					// Set Limit
					if (_Limit) {

						// Set Voltage Min Limit
						this->Set_Limit(__VRMS_MIN__, this->Limit.V_Min);

						// Set Voltage Max Limit
						this->Set_Limit(__VRMS_MAX__, this->Limit.V_Max);

						// Set Current Max Limit
						this->Set_Limit(__IRMS_MAX__, this->Limit.I_Max);

						// Set Frequency Min Limit
						this->Set_Limit(__F_MIN__, this->Limit.FQ_Min);

						// Set Frequency Max Limit
						this->Set_Limit(__F_MAX__, this->Limit.FQ_Max);

						// Set Voltage Imbalance Max Limit
						this->Set_Limit(__VIMB_MAX__, this->Limit.VImb_Max);

						// Set Current Imbalance Max Limit
						this->Set_Limit(__IIMB_MAX__, this->Limit.IImb_Max);

						// Set Temperature Min Limit
						this->Set_Limit(__T_MIN__, this->Limit.T_Min);

						// Set Temperature Max Limit
						this->Set_Limit(__T_MAX__, this->Limit.T_Max);

					}

					// Set Min Max
					if (_MinMax) {

						// Set Min/Max Monitor Address 1
						this->Set_Min_Max_Address(__MAX78630_MONITOR_1__, __MAX78630_Monitor_1_Type__);
						this->Clear_Min_Value(__MAX78630_MONITOR_1__);
						this->Clear_Max_Value(__MAX78630_MONITOR_1__);

						// Set Min/Max Monitor Address 2
						this->Set_Min_Max_Address(__MAX78630_MONITOR_2__, __MAX78630_Monitor_2_Type__);
						this->Clear_Min_Value(__MAX78630_MONITOR_2__);
						this->Clear_Max_Value(__MAX78630_MONITOR_2__);

						// Set Min/Max Monitor Address 3
						this->Set_Min_Max_Address(__MAX78630_MONITOR_3__, __MAX78630_Monitor_3_Type__);
						this->Clear_Min_Value(__MAX78630_MONITOR_3__);
						this->Clear_Max_Value(__MAX78630_MONITOR_3__);

						// Set Min/Max Monitor Address 4
						this->Set_Min_Max_Address(__MAX78630_MONITOR_4__, __MAX78630_Monitor_4_Type__);
						this->Clear_Min_Value(__MAX78630_MONITOR_4__);
						this->Clear_Max_Value(__MAX78630_MONITOR_4__);

						// Set Min/Max Monitor Address 5
						this->Set_Min_Max_Address(__MAX78630_MONITOR_5__, __MAX78630_Monitor_5_Type__);
						this->Clear_Min_Value(__MAX78630_MONITOR_5__);
						this->Clear_Max_Value(__MAX78630_MONITOR_5__);

						// Set Min/Max Monitor Address 6
						this->Set_Min_Max_Address(__MAX78630_MONITOR_6__, __MAX78630_Monitor_6_Type__);
						this->Clear_Min_Value(__MAX78630_MONITOR_6__);
						this->Clear_Max_Value(__MAX78630_MONITOR_6__);

						// Set Min/Max Monitor Address 7
						this->Set_Min_Max_Address(__MAX78630_MONITOR_7__, __MAX78630_Monitor_7_Type__);
						this->Clear_Min_Value(__MAX78630_MONITOR_7__);
						this->Clear_Max_Value(__MAX78630_MONITOR_7__);

						// Set Min/Max Monitor Address 8
						this->Set_Min_Max_Address(__MAX78630_MONITOR_8__, __MAX78630_Monitor_8_Type__);
						this->Clear_Min_Value(__MAX78630_MONITOR_8__);
						this->Clear_Max_Value(__MAX78630_MONITOR_8__);

					}

					// Set Alarm
					if (_Alarm) {

						// Set Sticky
						this->Set_Sticky(__MAX78630_Config_Status_Sticky__);

						// Set High Voltage Alarm --> AL1
						this->Set_Alarm_Mask(__MAX78630_ALARM_1__, __BIT_OV_VRMSA__, true);
						this->Set_Alarm_Mask(__MAX78630_ALARM_1__, __BIT_OV_VRMSB__, true);
						this->Set_Alarm_Mask(__MAX78630_ALARM_1__, __BIT_OV_VRMSC__, true);

						// Set Low Voltage Alarm --> AL1
						this->Set_Alarm_Mask(__MAX78630_ALARM_1__, __BIT_UN_VRMSA__, true);
						this->Set_Alarm_Mask(__MAX78630_ALARM_1__, __BIT_UN_VRMSB__, true);
						this->Set_Alarm_Mask(__MAX78630_ALARM_1__, __BIT_UN_VRMSC__, true);

						// Set High Frequency Alarm --> AL1
						this->Set_Alarm_Mask(__MAX78630_ALARM_1__, __BIT_OV_FREQ__, true);

						// Set Low Frequency Alarm --> AL1
						this->Set_Alarm_Mask(__MAX78630_ALARM_1__, __BIT_UN_FREQ__, true);

						// Set Voltage Imbalance Alarm --> AL2
						this->Set_Alarm_Mask(__MAX78630_ALARM_2__, __BIT_V_IMBAL__, true);

						// Set Current Imbalance Alarm --> AL2
						this->Set_Alarm_Mask(__MAX78630_ALARM_2__, __BIT_I_IMBAL__, true);

						// Set Low Power Factor Alarm --> AL2
						this->Set_Alarm_Mask(__MAX78630_ALARM_2__, __BIT_UN_PFA__, true);
						this->Set_Alarm_Mask(__MAX78630_ALARM_2__, __BIT_UN_PFB__, true);
						this->Set_Alarm_Mask(__MAX78630_ALARM_2__, __BIT_UN_PFC__, true);

						// Set High Temperature Alarm --> AL2
						this->Set_Alarm_Mask(__MAX78630_ALARM_2__, __BIT_OV_TEMP__, true);

						// Set Low Temperature Alarm --> AL2
						this->Set_Alarm_Mask(__MAX78630_ALARM_2__, __BIT_UN_TEMP__, true);

						// Clear Status
						this->Clear_Status();

					}

					// End Function
					return(true);

				} else {

					// End Function
					return(false);

				}

			}

			// Limit Set Function.
			bool Set_Limit(const uint8_t _Limit_Type, const float _Limit_Value) {

				// Decide Type
				switch (_Limit_Type) {

					case __VRMS_MIN__: {

						// Define Register
						Register VRMS_MIN {0x00, 0xB1, 23};	// Voltage lower alarm limit. Positive values only

						// Get Register
						return(this->Register_Pointer_Set(VRMS_MIN, this->FtoS(_Limit_Value / __MAX78630_Config_VScale__, VRMS_MIN.Data_Type)));

						// End Case
						break;

					}
					case __VRMS_MAX__: {

						// Define Register
						Register VRMS_MAX {0x00, 0xB4, 23};	// Voltage upper alarm limit. Positive values only

						// Get Register
						return(this->Register_Pointer_Set(VRMS_MAX, this->FtoS(_Limit_Value / __MAX78630_Config_VScale__, VRMS_MAX.Data_Type)));

						// End Case
						break;

					}
					case __IRMS_MAX__: {

						// Define Register
						Register IRMS_MAX {0x00, 0xF3, 23}; // Current upper alarm limit. Positive values only

						// Get Register
						return(this->Register_Pointer_Set(IRMS_MAX, this->FtoS(_Limit_Value / __MAX78630_Config_IScale__, IRMS_MAX.Data_Type)));

						// End Case
						break;

					}
					case __F_MIN__: {

						// Define Register
						Register F_MIN {0x01, 0x83, 16}; // Frequency Alarm Lower Limit

						// Get Register
						return(this->Register_Pointer_Set(F_MIN, this->FtoS(_Limit_Value, F_MIN.Data_Type)));

						// End Case
						break;

					}
					case __F_MAX__: {

						// Define Register
						Register F_MAX {0x01, 0x86, 16}; // Frequency Alarm Upper Limit

						// Get Register
						return(this->Register_Pointer_Set(F_MAX, this->FtoS(_Limit_Value, F_MAX.Data_Type)));

						// End Case
						break;

					}
					case __T_MIN__: {

						// Define Register
						Register T_MIN {0x01, 0x7A, 10}; // Temperature Alarm Lower Limit

						// Get Register
						return(this->Register_Pointer_Set(T_MIN, this->FtoS(_Limit_Value, T_MIN.Data_Type)));

						// End Case
						break;

					}
					case __T_MAX__: {

						// Define Register
						Register T_MAX {0x01, 0x7D, 10}; // Temperature Alarm Upper Limit

						// Get Register
						return(this->Register_Pointer_Set(T_MAX, this->FtoS(_Limit_Value, T_MAX.Data_Type)));

						// End Case
						break;

					}
					case __PF_MIN__: {

						// Define Register
						Register PF_MIN {0x01, 0x71, 22}; // Power Factor lower alarm limit

						// Get Register
						return(this->Register_Pointer_Set(PF_MIN, this->FtoS(_Limit_Value, PF_MIN.Data_Type)));

						// End Case
						break;

					}
					case __VSAG_LIM__: {

						// Define Register
						Register VSAG_LIM {0x00, 0xB7, 23}; // RMS Voltage Sag threshold. Positive values only

						// Get Register
						return(this->Register_Pointer_Set(VSAG_LIM, this->FtoS(_Limit_Value / __MAX78630_Config_VScale__, VSAG_LIM.Data_Type)));

						// End Case
						break;

					}
					case __VIMB_MAX__: {

						// Define Register
						Register V_IMB_MAX {0x00, 0x81, 23}; // Voltage imbalance alarm limit. Positive values only

						// Get Register
						return(this->Register_Pointer_Set(V_IMB_MAX, this->FtoS(_Limit_Value, V_IMB_MAX.Data_Type)));

						// End Case
						break;

					}
					case __IIMB_MAX__: {

						// Define Register
						Register I_IMB_MAX {0x00, 0x84, 23}; // Current imbalance alarm limit. Positive values only

						// Get Register
						return(this->Register_Pointer_Set(I_IMB_MAX, this->FtoS(_Limit_Value, I_IMB_MAX.Data_Type)));

						// End Case
						break;

					}
					default: {

						// End Function
						return(false);

					}

				}

				// End Function
				return(false);

			}

			/* IC Utility Functions */

			// Get Device Address Function.
			uint32_t Get_Device_Address(void) {

				// Define Register
				Register DEVADDR {0x00,	0x4E, 0}; // High order address bits for I2C and UART interfaces

				// Return Register
				return(this->Register_Pointer_Read(DEVADDR));

			}

			// Get Communication Baud Function.
			uint32_t Get_Baud(void) {

				// Define Register
				Register BAUD {0x00, 0x51, 0}; // Baud rate for UART interface

				// Return Register
				return(this->Register_Pointer_Read(BAUD));

			}

			// Set Baud Function
			bool Set_Baud(const uint32_t _Baud) {

				// Define Register
				Register BAUD {0x00, 0x51, 0}; // Baud rate for UART interface

				// Return Result
				return(this->Register_Pointer_Set(BAUD, _Baud));

			}

			// Get System Status Function.
			uint32_t Get_System_Stat(void) {

				// Define Register
				Register SYSSTAT {0x02, 0x43, 0}; // Bit 23 is a sticky register with status of any SPI Errors

				// Return Register
				return(this->Register_Pointer_Read(SYSSTAT));

			}

			// Get Device Frame Rate Function.
			uint32_t Get_Frame(void) {

				// Define Register
				Register FRAME {0x00, 0x12, 0}; // Low-rate sample counter

				// Return Register
				return(this->Register_Pointer_Read(FRAME));

			}

			// Get Sample Counter Function
			uint32_t Get_Cycle(void) {

				// Define Register
				Register CYCLE {0x00, 0x0F, 0}; // High-rate sample counter

				// Return Register
				return(this->Register_Pointer_Read(CYCLE));

			}

			// Get Divisor Value Function
			uint32_t Get_Divisor(void) {

				// Define Register
				Register DIVISOR {0x00, 0x0C, 0}; // Actual samples in previous accumulation interval

				// Return Register
				return(this->Register_Pointer_Read(DIVISOR));

			}

			// Get Samples per Accumulation Interval Function.
			uint32_t Get_Samples(void) {

				// Define Register
				Register SAMPLES {0x00, 0x09, 0}; // Minimum high-rate samples per accumulation interval

				// Return Register
				return(this->Register_Pointer_Read(SAMPLES));

			}

			/* Scale Config Functions */

			// Get VSCALE Function.
			uint32_t Get_VSCale(void) {

				// Define Register
				Register VSCALE {0x01, 0x44, 0}; // Scratch register (see Scaling Registers section)

				// Return Register
				return(this->Register_Pointer_Read(VSCALE));

			}

			// Get ISCALE Function.
			uint32_t Get_ISCale(void) {

				// Define Register
				Register IFSCALE {0x01, 0x41, 0}; // Scratch register (see Scaling Registers section)

				// Return Register
				return(this->Register_Pointer_Read(IFSCALE));

			}

			// Bucket Size Get Function.
			uint32_t Get_Bucket_H(void) {

				// Define Register
				Register BUCKET_HIGH {0x01, 0xD4, 0}; // Energy Bucket Size – High word

				// Set Command
				return(this->Register_Pointer_Read(BUCKET_HIGH));

			}

			// Bucket Size Get Function.
			uint32_t Get_Bucket_L(void) {

				// Define Register
				Register BUCKET_LOW {0x01, 0xD1, 0}; // Energy Bucket Size – Low word

				// Set Command
				return(this->Register_Pointer_Read(BUCKET_LOW));

			}

			/* Min Max Functions */

			// Get Min Value From Address
			float Get_Min_Value(const uint8_t _MM_ADDR) {

				// Decide Address
				switch (_MM_ADDR) {

					case __MAX78630_MONITOR_1__: {

						// Define Register
						Register MIN0 {0x01, 0x89, 23}; // Minimum Recorded Value 1

						// Get Register
						return(this->Register_Pointer_Read(MIN0) * __MAX78630_Config_VScale__);

						// End Case
						break;

					}
					case __MAX78630_MONITOR_2__: {

						// Define Register
						Register MIN1 {0x01, 0x8C, 23}; // Minimum Recorded Value 2

						// Get Register
						return(this->Register_Pointer_Read(MIN1) * __MAX78630_Config_VScale__);

						// End Case
						break;

					}
					case __MAX78630_MONITOR_3__: {

						// Define Register
						Register MIN2 {0x01, 0x8F, 23}; // Minimum Recorded Value 3

						// Get Register
						return(this->Register_Pointer_Read(MIN2) * __MAX78630_Config_VScale__);

						// End Case
						break;

					}
					case __MAX78630_MONITOR_4__: {

						// Define Register
						Register MIN3 {0x01, 0x92, 23}; // Minimum Recorded Value 4

						// Get Register
						return(this->Register_Pointer_Read(MIN3) * __MAX78630_Config_IScale__);

						// End Case
						break;

					}
					case __MAX78630_MONITOR_5__: {

						// Define Register
						Register MIN4 {0x01, 0x95, 23}; // Minimum Recorded Value 5

						// Get Register
						return(this->Register_Pointer_Read(MIN4) * __MAX78630_Config_IScale__);

						// End Case
						break;

					}
					case __MAX78630_MONITOR_6__: {

						// Define Register
						Register MIN5 {0x01, 0x98, 23}; // Minimum Recorded Value 6

						// Get Register
						return(this->Register_Pointer_Read(MIN5) * __MAX78630_Config_IScale__);

						// End Case
						break;

					}
					case __MAX78630_MONITOR_7__: {

						// Define Register
						Register MIN6 {0x01, 0x9B, 0}; // Minimum Recorded Value 7

						// Get Register
						return(this->Register_Pointer_Read(MIN6));

						// End Case
						break;

					}
					case __MAX78630_MONITOR_8__: {

						// Define Register
						Register MIN7 {0x01, 0x9E, 0}; // Minimum Recorded Value 8

						// Get Register
						return(this->Register_Pointer_Read(MIN7));

						// End Case
						break;

					}

				}

				// End Function
				return(-101);

			}

			// Get Max Value From Address
			float Get_Max_Value(const uint8_t _MM_ADDR) {

				// Decide Address
				switch (_MM_ADDR) {

					case __MAX78630_MONITOR_1__: {

						// Define Register
						Register MAX0 {0x01, 0xA1, 23}; // Maximum Recorded Value 1

						// Get Register
						return(this->Register_Pointer_Read(MAX0) * __MAX78630_Config_VScale__);

						// End Case
						break;

					}
					case __MAX78630_MONITOR_2__: {

						// Define Register
						Register MAX1 {0x01, 0xA4, 23}; // Maximum Recorded Value 2

						// Get Register
						return(this->Register_Pointer_Read(MAX1) * __MAX78630_Config_VScale__);

						// End Case
						break;

					}
					case __MAX78630_MONITOR_3__: {

						// Define Register
						Register MAX2 {0x01, 0xA7, 23}; // Maximum Recorded Value 3

						// Get Register
						return(this->Register_Pointer_Read(MAX2) * __MAX78630_Config_VScale__);

						// End Case
						break;

					}
					case __MAX78630_MONITOR_4__: {

						// Define Register
						Register MAX3 {0x01, 0xAA, 23}; // Maximum Recorded Value 4

						// Get Register
						return(this->Register_Pointer_Read(MAX3) * __MAX78630_Config_IScale__);

						// End Case
						break;

					}
					case __MAX78630_MONITOR_5__: {

						// Define Register
						Register MAX4 {0x01, 0xAD, 23}; // Maximum Recorded Value 5

						// Get Register
						return(this->Register_Pointer_Read(MAX4) * __MAX78630_Config_IScale__);

						// End Case
						break;

					}
					case __MAX78630_MONITOR_6__: {

						// Define Register
						Register MAX5 {0x01, 0xB0, 23}; // Maximum Recorded Value 6

						// Get Register
						return(this->Register_Pointer_Read(MAX5) * __MAX78630_Config_IScale__);

						// End Case
						break;

					}
					case __MAX78630_MONITOR_7__: {

						// Define Register
						Register MAX6 {0x01, 0xB3, 0}; // Maximum Recorded Value 7

						// Get Register
						return(this->Register_Pointer_Read(MAX6));

						// End Case
						break;

					}
					case __MAX78630_MONITOR_8__: {

						// Define Register
						Register MAX7 {0x01, 0xB6, 0}; // Maximum Recorded Value 8

						// Get Register
						return(this->Register_Pointer_Read(MAX7));

						// End Case
						break;

					}

				}

				// End Function
				return(-101);

			}

			/* Calibration Functions */

			// Voltage Calibration Function
			bool Voltage_Calibration(const uint8_t _Phase, const float _Gain, const float _Offset) {

				Register V1_GAIN 		{0x00, 		0x5D, 		21};		// Voltage Gain Calibration. Positive values only
				Register V2_GAIN 		{0x00, 		0x60, 		21};		// Voltage Gain Calibration. Positive values only
				Register V3_GAIN 		{0x00, 		0x63, 		21};		// Voltage Gain Calibration. Positive values only
				Register V1_OFFS 		{0x00, 		0x6F, 		23};		// Voltage Offset Calibration
				Register V2_OFFS 		{0x00, 		0x72, 		23};		// Voltage Offset Calibration
				Register V3_OFFS 		{0x00, 		0x75, 		23};		// Voltage Offset Calibration

				// Control Input Parameters
				if (_Phase > 3) return(false);

				// Phase R Calibration
				if (_Phase == __Phase_R__) {

					bool _Result_Gain = this->Register_Pointer_Set(V1_GAIN, this->FtoS(_Gain, V1_GAIN.Data_Type)); // Write Gain
					bool _Result_Offset = this->Register_Pointer_Set(V1_OFFS, this->FtoS(_Offset, V1_GAIN.Data_Type)); // Write Offset

					// End Function
					return(_Result_Gain and _Result_Offset);

				}

				// Phase S Calibration
				if (_Phase == __Phase_S__) {

					bool _Result_Gain = this->Register_Pointer_Set(V2_GAIN, this->FtoS(_Gain, V2_GAIN.Data_Type)); // Write Gain
					bool _Result_Offset = this->Register_Pointer_Set(V2_OFFS, this->FtoS(_Offset, V2_GAIN.Data_Type)); // Write Offset

					// End Function
					return(_Result_Gain and _Result_Offset);

				}

				// Phase T Calibration
				if (_Phase == __Phase_T__) {

					bool _Result_Gain = this->Register_Pointer_Set(V3_GAIN, this->FtoS(_Gain, V3_GAIN.Data_Type)); // Write Gain
					bool _Result_Offset = this->Register_Pointer_Set(V3_OFFS, this->FtoS(_Offset, V3_GAIN.Data_Type)); // Write Offset

					// End Function
					return(_Result_Gain and _Result_Offset);

				}

				// End Function
				return(false);

			}

			// Current Calibration Function
			bool Current_Calibration(const uint8_t _Phase, const float _Gain, const float _Offset) {

				Register I1_GAIN 		{0x00, 		0x54, 		21};		// Current Gain Calibration.Positive values only
				Register I2_GAIN 		{0x00, 		0x57, 		21};		// Current Gain Calibration.Positive values only
				Register I3_GAIN 		{0x00, 		0x5A, 		21};		// Current Gain Calibration.Positive values only
				Register I1_OFFS 		{0x00, 		0x66, 		23};		// Current Offset Calibration
				Register I2_OFFS 		{0x00, 		0x69, 		23};		// Current Offset Calibration
				Register I3_OFFS 		{0x00, 		0x6C, 		23};		// Current Offset Calibration

				// Control Input Parameters
				if (_Phase > 3) return(false);

				// Phase R Calibration
				if (_Phase == __Phase_R__) {

					bool _Result_Gain = this->Register_Pointer_Set(I1_GAIN, this->FtoS(_Gain, I1_GAIN.Data_Type)); // Write Gain
					bool _Result_Offset = this->Register_Pointer_Set(I1_OFFS, this->FtoS(_Offset, I1_GAIN.Data_Type)); // Write Offset

					// End Function
					return(_Result_Gain and _Result_Offset);

				}

				// Phase S Calibration
				if (_Phase == __Phase_S__) {

					bool _Result_Gain = this->Register_Pointer_Set(I2_GAIN, this->FtoS(_Gain, I2_GAIN.Data_Type)); // Write Gain
					bool _Result_Offset = this->Register_Pointer_Set(I2_OFFS, this->FtoS(_Offset, I2_GAIN.Data_Type)); // Write Offset

					// End Function
					return(_Result_Gain and _Result_Offset);

				}

				// Phase T Calibration
				if (_Phase == __Phase_T__) {

					bool _Result_Gain = this->Register_Pointer_Set(I3_GAIN, this->FtoS(_Gain, I3_GAIN.Data_Type)); // Write Gain
					bool _Result_Offset = this->Register_Pointer_Set(I3_OFFS, this->FtoS(_Offset, I3_GAIN.Data_Type)); // Write Offset

					// End Function
					return(_Result_Gain and _Result_Offset);

				}

				// End Function
				return(false);

			}

			// Temperature Calibration Function
			bool Temperature_Calibration(const float _Gain, const float _Offset) {

				Register T_GAIN 		{0x00, 		0x78, 		0};			// Temperature Slope Calibration
				Register T_OFFS 		{0x00, 		0x7B, 		0};			// Temperature Offset Calibration

				bool _Result_Gain = this->Register_Pointer_Set(T_GAIN, this->FtoS(_Gain, T_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = this->Register_Pointer_Set(T_OFFS, this->FtoS(_Offset, T_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// Voltage High Pass Filter Coefficient Function.
			float Voltage_HPF_COEF(float _COEF) {

				Register HPF_COEF_V 	{0x00, 		0x3F, 		23};		// Voltage input HPF coefficient. Positive values only

				// Declare Variable
				float _Result = 0;

				// Decide Workflow
				if (_COEF == -999) {

					// Read Register
					_Result = this->Register_Pointer_Read(HPF_COEF_V); // Measure Phase R

				} else {

					// Set Register
					_Result = this->Register_Pointer_Set(HPF_COEF_V, this->FtoS(_COEF, 23)); // Measure Phase R

				}
				
				// End Function
				return(_Result);

			}

			// Current High Pass Filter Coefficient Function.
			float Current_HPF_COEF(float _COEF) {

				Register HPF_COEF_I 	{0x00, 		0x3C, 		23};		// Current input HPF coefficient. Positive values only

				// Declare Variable
				float _Result = 0;

				// Decide Workflow
				if (_COEF == -999) {

					// Read Register
					_Result = this->Register_Pointer_Read(HPF_COEF_I); // Measure Phase R

				} else {

					// Set Register
					_Result = this->Register_Pointer_Set(HPF_COEF_I, this->FtoS(_COEF, 23)); // Measure Phase R

				}
				
				// End Function
				return(_Result);

			}

			// Current RMS Offset Set Function.
			float Current_RMS_Offset(char Phase, float _Offset) {

				Register IARMS_OFF 		{0x00, 		0xC3, 		23};		// RMS Current dynamic offset adjust. Positive values only
				Register IBRMS_OFF 		{0x00, 		0xC6, 		23};		// RMS Current dynamic offset adjust. Positive values only
				Register ICRMS_OFF 		{0x00, 		0xC9, 		23};		// RMS Current dynamic offset adjust. Positive values only

				// Declare Variable
				float _Result = 0;

				// Decide Workflow
				if (_Offset == -999) {

					// Read Register
					if (Phase == 'R') _Result = this->Register_Pointer_Read(IARMS_OFF); // Measure Phase R
					if (Phase == 'S') _Result = this->Register_Pointer_Read(IBRMS_OFF); // Measure Phase S
					if (Phase == 'T') _Result = this->Register_Pointer_Read(ICRMS_OFF); // Measure Phase T

				} else {

					// Set Register
					if (Phase == 'R') _Result = this->Register_Pointer_Set(IARMS_OFF, this->FtoS(_Offset, 23)); // Measure Phase R
					if (Phase == 'S') _Result = this->Register_Pointer_Set(IBRMS_OFF, this->FtoS(_Offset, 23)); // Measure Phase S
					if (Phase == 'T') _Result = this->Register_Pointer_Set(ICRMS_OFF, this->FtoS(_Offset, 23)); // Measure Phase T

				}

				// End Function
				return(_Result);

			}

			// Active Power Offset Set Function.
			float Active_Power_Offset(char Phase, float _Offset) {

				Register PA_OFFS 		{0x01, 		0x14, 		23};		// Active Power dynamic offset adjust. Positive values only
				Register PB_OFFS 		{0x01, 		0x17, 		23};		// Active Power dynamic offset adjust. Positive values only
				Register PC_OFFS 		{0x01, 		0x1A, 		23};		// Active Power dynamic offset adjust. Positive values only

				// Declare Variable
				float _Result = 0;

				// Decide Workflow
				if (_Offset == -999) {

					// Read Register
					if (Phase == 'R') _Result = this->Register_Pointer_Read(PA_OFFS); // Measure Phase R
					if (Phase == 'S') _Result = this->Register_Pointer_Read(PB_OFFS); // Measure Phase S
					if (Phase == 'T') _Result = this->Register_Pointer_Read(PC_OFFS); // Measure Phase T

				} else {

					// Set Register
					if (Phase == 'R') _Result = this->Register_Pointer_Set(PA_OFFS, this->FtoS(_Offset, 23)); // Measure Phase R
					if (Phase == 'S') _Result = this->Register_Pointer_Set(PB_OFFS, this->FtoS(_Offset, 23)); // Measure Phase S
					if (Phase == 'T') _Result = this->Register_Pointer_Set(PC_OFFS, this->FtoS(_Offset, 23)); // Measure Phase T

				}
				
				// End Function
				return(_Result);

			}

			// ReActive Power Offset Set Function.
			float ReActive_Power_Offset(char Phase, float _Offset) {

				Register QA_OFFS 		{0x01, 		0x0B, 		23};		// Reactive Power dynamic offset adjust. Positive values only
				Register QB_OFFS 		{0x01, 		0x0E, 		23};		// Reactive Power dynamic offset adjust. Positive values only
				Register QC_OFFS 		{0x01, 		0x11, 		23};		// Reactive Power dynamic offset adjust. Positive values only

				// Declare Variable
				float _Result = 0;

				// Decide Workflow
				if (_Offset == -999) {

					// Read Register
					if (Phase == 'R') _Result = this->Register_Pointer_Read(QA_OFFS); // Measure Phase R
					if (Phase == 'S') _Result = this->Register_Pointer_Read(QB_OFFS); // Measure Phase S
					if (Phase == 'T') _Result = this->Register_Pointer_Read(QC_OFFS); // Measure Phase T

				} else {

					// Set Register
					if (Phase == 'R') _Result = this->Register_Pointer_Set(QA_OFFS, this->FtoS(_Offset, 23)); // Measure Phase R
					if (Phase == 'S') _Result = this->Register_Pointer_Set(QB_OFFS, this->FtoS(_Offset, 23)); // Measure Phase S
					if (Phase == 'T') _Result = this->Register_Pointer_Set(QC_OFFS, this->FtoS(_Offset, 23)); // Measure Phase T

				}

				// End Function
				return(_Result);

			}

			/* Measurement Functions */

			// Set Harmonic Channel Function. TODO: Not Tested.
			uint8_t Set_Harmonic(uint32_t _Harmonic) {

				Register HARM {0x00, 0x4B, 0}; // Harmonic Selector, default: 1 (fundamental)

				// Declare Variable
				uint32_t _Result = 0;

				// Decide Action
				if (_Harmonic == 0) {
					
					// Read Register
					_Result = this->Register_Pointer_Read(HARM);

				} else {

					// Set Register
					if (Register_Pointer_Set(HARM, _Harmonic)) {
						
						// Set Variable
						_Result = _Harmonic;
						
					} else {

						// Set Variable
						_Result = 255;

					}

				}
				
				// End Function
				return(_Result);

			}

			// Voltage Measurement Function.
			float Voltage(const uint8_t _Phase, const uint8_t _Type) {

				// Decide Phase
				switch (_Phase) {

					// Phase R
					case __Phase_R__: {

						// Decide Type
						switch (_Type) {

							// RMS Voltage
							case __RMS__: {

								// Define Register
								Register VA_RMS {0x00, 0x90, 23}; // RMS Voltage

								// Get Register
								return(this->Register_Pointer_Read(VA_RMS) * __MAX78630_Config_VScale__);

								// End Case
								break;

							}

							// Instantaneous Voltage
							case __Instant__: {

								// Define Register
								Register VA {0x00, 0x87, 23}; // Instantaneous Voltage

								// Get Register
								return(this->Register_Pointer_Read(VA) * __MAX78630_Config_VScale__);

								// End Case
								break;

							}

							// Fundamental Voltage
							case __Fundamental__: {

								// Define Register
								Register VFUND_A {0x00, 0x9C, 23}; // Fundamental Voltage

								// Get Register
								return(this->Register_Pointer_Read(VFUND_A) * __MAX78630_Config_VScale__);

								// End Case
								break;

							}

							// Harmonic Voltage
							case __Harmonic__: {

								// Define Register
								Register VHARM_A {0x00, 0xA5, 23}; // Harmonic Voltage

								// Get Register
								return(this->Register_Pointer_Read(VHARM_A) * __MAX78630_Config_VScale__);

								// End Case
								break;
							}

						}

						// End Case
						break;

					}
					case __Phase_S__: {

						// Decide Type
						switch (_Type) {

							// RMS Voltage
							case __RMS__: {

								// Define Register
								Register VB_RMS {0x00, 0x93, 23}; // RMS Voltage

								// Get Register
								return(this->Register_Pointer_Read(VB_RMS) * __MAX78630_Config_VScale__);

								// End Case
								break;

							}

							// Instantaneous Voltage
							case __Instant__: {

								// Define Register
								Register VB {0x00, 0x8A, 23}; // Instantaneous Voltage

								// Get Register
								return(this->Register_Pointer_Read(VB) * __MAX78630_Config_VScale__);

								// End Case
								break;

							}

							// Fundamental Voltage
							case __Fundamental__: {

								// Define Register
								Register VFUND_B {0x00, 0x9F, 23}; // Fundamental Voltage

								// Get Register
								return(this->Register_Pointer_Read(VFUND_B) * __MAX78630_Config_VScale__);

								// End Case
								break;

							}

							// Harmonic Voltage
							case __Harmonic__: {

								// Define Register
								Register VHARM_B {0x00, 0xA8, 23}; // Harmonic Voltage

								// Get Register
								return(this->Register_Pointer_Read(VHARM_B) * __MAX78630_Config_VScale__);

								// End Case
								break;

							}

						}

						// End Case
						break;

					}
					case __Phase_T__: {

						// Decide Type
						switch (_Type) {

							// RMS Voltage
							case __RMS__: {

								// Define Register
								Register VC_RMS {0x00, 0x96, 23}; // RMS Voltage

								// Get Register
								return(this->Register_Pointer_Read(VC_RMS) * __MAX78630_Config_VScale__);

								// End Case
								break;

							}

							// Instantaneous Voltage
							case __Instant__: {

								// Define Register
								Register VC {0x00, 0x8D, 23}; // Instantaneous Voltage

								// Get Register
								return(this->Register_Pointer_Read(VC) * __MAX78630_Config_VScale__);

								// End Case
								break;

							}

							// Fundamental Voltage
							case __Fundamental__: {

								// Define Register
								Register VFUND_C {0x00, 0xA2, 23}; // Fundamental Voltage

								// Get Register
								return(this->Register_Pointer_Read(VFUND_C) * __MAX78630_Config_VScale__);

								// End Case
								break;

							}

							// Harmonic Voltage
							case __Harmonic__: {

								// Define Register
								Register VHARM_C {0x00, 0xAB, 23}; // Harmonic Voltage

								// Get Register
								return(this->Register_Pointer_Read(VHARM_C) * __MAX78630_Config_VScale__);

								// End Case
								break;

							}

						}

						// End Case
						break;

					}
					case __Phase_Avg__: {

						// Define Register
						Register VT_RMS {0x00, 0x99, 23}; // RMS Voltage average (Total / 3)

						// Get Register
						return(this->Register_Pointer_Read(VT_RMS) * __MAX78630_Config_VScale__);

						// End Case
						break;

					}

				}

				// End Function
				return(0);

			}

			// Current Measurement Function.
			float Current(const uint8_t _Phase, const uint8_t _Type) {

				// Decide Phase
				switch (_Phase) {

					// Phase R
					case __Phase_R__: {

						// Decide Type
						switch (_Type) {

							// RMS Current
							case __RMS__: {

								// Define Register
								Register IA_RMS {0x00, 0xD5, 23}; // RMS Current

								// Get Register
								return(this->Register_Pointer_Read(IA_RMS) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}

							// Instantaneous Current
							case __Instant__: {

								// Define Register
								Register IA {0x00, 0xBA, 23}; // Instantaneous Current

								// Get Register
								return(this->Register_Pointer_Read(IA) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}

							// Fundamental Current
							case __Fundamental__: {

								// Define Register
								Register IFUND_A {0x00, 0xE1, 23}; // Fundamental Current

								// Get Register
								return(this->Register_Pointer_Read(IFUND_A) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}

							// Harmonic Current
							case __Harmonic__: {

								// Define Register
								Register IHARM_A {0x00, 0xEA, 23}; // Harmonic Current

								// Get Register
								return(this->Register_Pointer_Read(IHARM_A) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}

							// Peak Current
							case __Peak__: {

								// Define Register
								Register IA_PEAK {0x00, 0xCC, 23}; // Peak Current

								// Get Register
								return(this->Register_Pointer_Read(IA_PEAK) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}

							// Phase Compensation
							case __Phase_Compensation__: {

								// Define Register
								Register PHASECOMP1 {0x00, 0x42, 21}; // Phase compensation (±4 samples) for AV1 input

								// Get Register
								return(this->Register_Pointer_Read(PHASECOMP1) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}

						}

						// End Case
						break;

					}

					// Phase S
					case __Phase_S__: {

						// Decide Type
						switch (_Type) {

							// RMS Current
							case __RMS__: {

								// Define Register
								Register IB_RMS {0x00, 0xD8, 23}; // RMS Current

								// Get Register
								return(this->Register_Pointer_Read(IB_RMS) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							
							// Instantaneous Current
							case __Instant__: {

								// Define Register
								Register IB {0x00, 0xBD, 23}; // Instantaneous Current

								// Get Register
								return(this->Register_Pointer_Read(IB) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							
							// Fundamental Current
							case __Fundamental__: {

								// Define Register
								Register IFUND_B {0x00, 0xE4, 23}; // Fundamental Current

								// Get Register
								return(this->Register_Pointer_Read(IFUND_B) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							
							// Harmonic Current
							case __Harmonic__: {

								// Define Register
								Register IHARM_B {0x00, 0xED, 23}; // Harmonic Current

								// Get Register
								return(this->Register_Pointer_Read(IHARM_B) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							
							// Peak Current
							case __Peak__: {

								// Define Register
								Register IB_PEAK {0x00, 0xCF, 23}; // Peak Current

								// Get Register
								return(this->Register_Pointer_Read(IB_PEAK) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							
							// Phase Compensation
							case __Phase_Compensation__: {

								// Define Register
								Register PHASECOMP2 {0x00, 0x45, 21}; // Phase compensation (±4 samples) for AV2 input

								// Get Register
								return(this->Register_Pointer_Read(PHASECOMP2) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}

						}

						// End Case
						break;

					}
					
					// Phase T
					case __Phase_T__: {

						// Decide Type
						switch (_Type) {

							// RMS Current
							case __RMS__: {

								// Define Register
								Register IC_RMS {0x00, 0xDB, 23}; // RMS Current

								// Get Register
								return(this->Register_Pointer_Read(IC_RMS) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							
							// Instantaneous Current
							case __Instant__: {

								// Define Register
								Register IC {0x00, 0xC0, 23}; // Instantaneous Current

								// Get Register
								return(this->Register_Pointer_Read(IC) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							
							// Fundamental Current
							case __Fundamental__: {

								// Define Register
								Register IFUND_C {0x00, 0xE7, 23}; // Fundamental Current

								// Get Register
								return(this->Register_Pointer_Read(IFUND_C) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							
							// Harmonic Current
							case __Harmonic__: {

								// Define Register
								Register IHARM_C {0x00, 0xF0, 23}; // Harmonic Current

								// Get Register
								return(this->Register_Pointer_Read(IHARM_C) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							
							// Peak Current
							case __Peak__: {

								// Define Register
								Register IC_PEAK {0x00, 0xD2, 23}; // Peak Current

								// Get Register
								return(this->Register_Pointer_Read(IC_PEAK) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							
							// Phase Compensation
							case __Phase_Compensation__: {

								// Define Register
								Register PHASECOMP3 {0x00, 0x48, 21}; // Phase compensation (±4 samples) for AV3 input

								// Get Register
								return(this->Register_Pointer_Read(PHASECOMP3) * __MAX78630_Config_IScale__);

								// End Case
								break;

							}

						}

						// End Case
						break;

					}
					
					// Phase Average
					case __Phase_Avg__: {

						// Define Register
						Register IT_RMS {0x00, 0xDE, 23}; // RMS Current average (Total / 3)

						// Get Register
						return(this->Register_Pointer_Read(IT_RMS) * __MAX78630_Config_IScale__);

						// End Case
						break;

					}

				}

				// End Function
				return(0);

			}

			// Frequency Measurement Function.
			float Frequency(void) {

				// Define Register
				Register FREQ {0x01, 0x80, 16}; // Line Frequency

				// Return Register
				return(this->Register_Pointer_Read(FREQ));

			}

			// Power Factor Measurement Function.
			float Power_Factor(const uint8_t _Phase) {

				// Decide Phase
				switch (_Phase) {

					case __Phase_R__: {

						// Define Register
						Register PFA {0x01, 0x65, 22}; // Power Factor

						// Return Register
						return(this->Register_Pointer_Read(PFA));

						// End Case
						break;

					}
					case __Phase_S__: {

						// Define Register
						Register PFB {0x01, 0x68, 22}; // Power Factor

						// Return Register
						return(this->Register_Pointer_Read(PFB));

						// End Case
						break;

					}
					case __Phase_T__: {

						// Define Register
						Register PFC {0x01, 0x6B, 22}; // Power Factor

						// Return Register
						return(this->Register_Pointer_Read(PFC));

						// End Case
						break;

					}
					case __Phase_Avg__: {

						// Define Register
						Register PF_T {0x01, 0x6E, 22}; // T otal Power Factor

						// Return Register
						return(this->Register_Pointer_Read(PF_T));

						// End Case
						break;

					}

				}

				// End Function
				return(0);

			}

			// Power Measurement Function.
			float Power(const uint8_t _Phase, const uint8_t _Type) {

				// Decide Phase
				switch (_Phase) {

					case __Phase_R__: {

						// Decide Type
						switch (_Type) {

							case __Active__: {

								// Define Register
								Register WATT_A {0x01, 0x1D, 23}; // Active Power

								// Get Register
								return(this->Register_Pointer_Read(WATT_A) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __ReActive__: {

								// Define Register
								Register VAR_A {0x01, 0x26, 23}; // Reactive Power

								// Get Register
								return(this->Register_Pointer_Read(VAR_A) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Apparent__: {

								// Define Register
								Register VA_A {0x01, 0x2F, 23};	// Apparent Power

								// Get Register
								return(this->Register_Pointer_Read(VA_A) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Fundamental__: {

								// Define Register
								Register PFUND_A {0x01, 0x4A, 23}; // Fundamental Power

								// Get Register
								return(this->Register_Pointer_Read(PFUND_A) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Harmonic__: {

								// Define Register
								Register PHARM_A {0x01, 0x53, 23}; // Harmonic Power

								// Get Register
								return(this->Register_Pointer_Read(PHARM_A) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Fund_ReActive__: {

								// Define Register
								Register QFUND_A {0x00, 0xF9, 23}; // Fundamental Reactive Power

								// Get Register
								return(this->Register_Pointer_Read(QFUND_A) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Harm_ReActive__: {

								// Define Register
								Register QHARM_A {0x01, 0x02, 23}; // Harmonic Reactive Power

								// Get Register
								return(this->Register_Pointer_Read(QHARM_A) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Fund_VA__: {

								// Define Register
								Register VAFUNDA {0x01, 0x5C, 23}; // Fundamental Volt Amperes

								// Get Register
								return(this->Register_Pointer_Read(VAFUNDA) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}

						}

						// End Case
						break;

					}
					case __Phase_S__: {

						// Decide Type
						switch (_Type) {

							case __Active__: {

								// Define Register
								Register WATT_B {0x01, 0x20, 23}; // Active Power

								// Get Register
								return(this->Register_Pointer_Read(WATT_B) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __ReActive__: {

								// Define Register
								Register VAR_B {0x01, 0x29, 23}; // Reactive Power

								// Get Register
								return(this->Register_Pointer_Read(VAR_B) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Apparent__: {

								// Define Register
								Register VA_B {0x01, 0x32, 23};	// Apparent Power

								// Get Register
								return(this->Register_Pointer_Read(VA_B) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Fundamental__: {

								// Define Register
								Register PFUND_B {0x01, 0x4D, 23}; // Fundamental Power

								// Get Register
								return(this->Register_Pointer_Read(PFUND_B) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Harmonic__: {

								// Define Register
								Register PHARM_B {0x01, 0x56, 23}; // Harmonic Power

								// Get Register
								return(this->Register_Pointer_Read(PHARM_B) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Fund_ReActive__: {

								// Define Register
								Register QFUND_B {0x00, 0xFC, 23}; // Fundamental Reactive Power

								// Get Register
								return(this->Register_Pointer_Read(QFUND_B) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Harm_ReActive__: {

								// Define Register
								Register QHARM_B {0x01, 0x05, 23}; // Harmonic Reactive Power

								// Get Register
								return(this->Register_Pointer_Read(QHARM_B) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Fund_VA__: {

								// Define Register
								Register VAFUNDB {0x01, 0x5F, 23}; // Fundamental Volt Amperes

								// Get Register
								return(this->Register_Pointer_Read(VAFUNDB) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}

						}

						// End Case
						break;

					}
					case __Phase_T__: {

						// Decide Type
						switch (_Type) {

							case __Active__: {

								// Define Register
								Register WATT_C {0x01, 0x23, 23}; // Active Power

								// Get Register
								return(this->Register_Pointer_Read(WATT_C) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __ReActive__: {

								// Define Register
								Register VAR_C {0x01, 0x2C, 23}; // Reactive Power

								// Get Register
								return(this->Register_Pointer_Read(VAR_C) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Apparent__: {

								// Define Register
								Register VA_C {0x01, 0x35, 23}; // Apparent Power

								// Get Register
								return(this->Register_Pointer_Read(VA_C) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Fundamental__: {

								// Define Register
								Register PFUND_C {0x01, 0x50, 23}; // Fundamental Power

								// Get Register
								return(this->Register_Pointer_Read(PFUND_C) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Harmonic__: {

								// Define Register
								Register PHARM_C {0x01, 0x59, 23}; // Harmonic Power

								// Get Register
								return(this->Register_Pointer_Read(PHARM_C) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Fund_ReActive__: {

								// Define Register
								Register QFUND_C {0x00, 0xFF, 23}; // Fundamental Reactive Power

								// Get Register
								return(this->Register_Pointer_Read(QFUND_C) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Harm_ReActive__: {

								// Define Register
								Register QHARM_C {0x01, 0x08, 23}; // Harmonic Reactive Power

								// Get Register
								return(this->Register_Pointer_Read(QHARM_C) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Fund_VA__: {

								// Define Register
								Register VAFUNDC {0x01, 0x62, 23}; // Fundamental Volt Amperes

								// Get Register
								return(this->Register_Pointer_Read(VAFUNDC) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}

						}

						// End Case
						break;

					}
					case __Phase_Avg__: {

						// Decide Type
						switch (_Type) {

							case __Active__: {

								// Define Register
								Register WATT_T {0x01, 0x38, 23}; // Active Power average (Total / 3)

								// Get Register
								return(this->Register_Pointer_Read(WATT_T) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __ReActive__: {

								// Define Register
								Register VAR_T {0x01, 0x3B, 23}; // Reactive Power average (Total / 3)

								// Get Register
								return(this->Register_Pointer_Read(VAR_T) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}
							case __Apparent__: {

								// Define Register
								Register VA_T {0x01, 0x3E, 23}; // Apparent Power average (Total / 3)

								// Get Register
								return(this->Register_Pointer_Read(VA_T) * __MAX78630_Config_VScale__ * __MAX78630_Config_IScale__);

								// End Case
								break;

							}

						}

						// End Case
						break;

					}

				}

				// End Function
				return(0);

			}

			// Energy Measurement Function.
			float Energy(const uint8_t _Phase, const uint8_t _Type) {

				// Decide Phase
				switch (_Phase) {

					case __Phase_R__: {

						// Decide Type
						switch (_Type) {

							case __Active_Received__: {

								// Define Register
								Register WHA_POS {0x01, 0xDD, 0}; // Received Active Energy Counter

								// Get Register
								return(this->Register_Pointer_Read(WHA_POS));

								// End Case
								break;

							}
							case __Active_Delivered__: {

								// Define Register
								Register WHA_NEG {0x01, 0xE6, 0}; // Delivered Active Energy Counter

								// Get Register
								return(this->Register_Pointer_Read(WHA_NEG));

								// End Case
								break;

							}
							case __ReActive_Received__: {

								// Define Register
								Register VARHA_POS {0x02, 0x13, 0};	// Reactive Energy Leading Counter

								// Get Register
								return(this->Register_Pointer_Read(VARHA_POS));

								// End Case
								break;

							}
							case __ReActive_Delivered__: {

								// Define Register
								Register VARHA_NEG {0x02, 0x1C, 0};	// Reactive Energy Lagging Counter

								// Get Register
								return(this->Register_Pointer_Read(VARHA_NEG));

								// End Case
								break;

							}
							case __Energy_Reset__: {

								// Define Register
								Register WHA_POS {0x01, 0xDD, 0}; // Received Active Energy Counter
								Register WHA_NEG {0x01, 0xE6, 0}; // Delivered Active Energy Counter
								Register VARHA_POS {0x02, 0x13, 0};	// Reactive Energy Leading Counter
								Register VARHA_NEG {0x02, 0x1C, 0};	// Reactive Energy Lagging Counter

								// Clear Register
								this->Register_Pointer_Set(WHA_POS, 0x00);
								this->Register_Pointer_Set(VARHA_POS, 0x00);
								this->Register_Pointer_Set(WHA_NEG, 0x00);
								this->Register_Pointer_Set(VARHA_NEG, 0x00);

								// End Function
								return(1);

								// End Case
								break;

							}

						}

						// End Case
						break;

					}
					case __Phase_S__: {

						// Decide Type
						switch (_Type) {

							case __Active_Received__: {

								// Define Register
								Register WHB_POS {0x01, 0xEF, 0}; // Received Active Energy Counter

								// Get Register
								return(this->Register_Pointer_Read(WHB_POS));

								// End Case
								break;

							}
							case __Active_Delivered__: {

								// Define Register
								Register WHB_NEG {0x01, 0xF8, 0}; // Delivered Active Energy Counter

								// Get Register
								return(this->Register_Pointer_Read(WHB_NEG));

								// End Case
								break;

							}
							case __ReActive_Received__: {

								// Define Register
								Register VARHB_POS {0x02, 0x25, 0};	// Reactive Energy Leading Counter

								// Get Register
								return(this->Register_Pointer_Read(VARHB_POS));

								// End Case
								break;

							}
							case __ReActive_Delivered__: {

								// Define Register
								Register VARHB_NEG {0x02, 0x2E, 0};	// Reactive Energy Lagging Counter

								// Get Register
								return(this->Register_Pointer_Read(VARHB_NEG));

								// End Case
								break;

							}
							case __Energy_Reset__: {

								// Define Register
								Register WHB_POS {0x01, 0xEF, 0}; // Received Active Energy Counter
								Register WHB_NEG {0x01, 0xF8, 0}; // Delivered Active Energy Counter
								Register VARHB_POS {0x02, 0x25, 0};	// Reactive Energy Leading Counter
								Register VARHB_NEG {0x02, 0x2E, 0};	// Reactive Energy Lagging Counter

								// Clear Register
								this->Register_Pointer_Set(WHB_POS, 0x00);
								this->Register_Pointer_Set(VARHB_POS, 0x00);
								this->Register_Pointer_Set(WHB_NEG, 0x00);
								this->Register_Pointer_Set(VARHB_NEG, 0x00);

								// End Function
								return(1);

								// End Case
								break;

							}

						}

						// End Case
						break;

					}
					case __Phase_T__: {

						// Decide Type
						switch (_Type) {

							case __Active_Received__: {

								// Define Register
								Register WHC_POS {0x02, 0x01, 0}; // Received Active Energy Counter

								// Get Register
								return(this->Register_Pointer_Read(WHC_POS));

								// End Case
								break;

							}
							case __Active_Delivered__: {

								// Define Register
								Register WHC_NEG {0x02, 0x0A, 0}; // Delivered Active Energy Counter

								// Get Register
								return(this->Register_Pointer_Read(WHC_NEG));

								// End Case
								break;

							}
							case __ReActive_Received__: {

								// Define Register
								Register VARHC_POS {0x02, 0x37, 0};	// Reactive Energy Leading Counter

								// Get Register
								return(this->Register_Pointer_Read(VARHC_POS));

								// End Case
								break;

							}
							case __ReActive_Delivered__: {

								// Define Register
								Register VARHC_NEG {0x02, 0x40, 0}; // Reactive Energy Lagging Counter

								// Get Register
								return(this->Register_Pointer_Read(VARHC_NEG));

								// End Case
								break;

							}
							case __Energy_Reset__: {

								// Define Register
								Register WHC_POS {0x02, 0x01, 0}; // Received Active Energy Counter
								Register WHC_NEG {0x02, 0x0A, 0}; // Delivered Active Energy Counter
								Register VARHC_POS {0x02, 0x37, 0};	// Reactive Energy Leading Counter
								Register VARHC_NEG {0x02, 0x40, 0}; // Reactive Energy Lagging Counter

								// Clear Register
								this->Register_Pointer_Set(WHC_POS, 0x00);
								this->Register_Pointer_Set(VARHC_POS, 0x00);
								this->Register_Pointer_Set(WHC_NEG, 0x00);
								this->Register_Pointer_Set(VARHC_NEG, 0x00);

								// End Function
								return(1);

								// End Case
								break;

							}

						}

						// End Case
						break;

					}

				}

				// End Function
				return(0);

			}

			// IC Temperature Measurement Function.
			float Temperature(void) {

				// Define Register
				Register TEMPC {0x01, 0x74, 10}; // Chip Temperature (Celsius°)

				// Return Register
				return(this->Register_Pointer_Read(TEMPC));

			}

			/* Limit and Defect Detect Functions */

			// Read Alarm Status
			bool Alarm(const uint8_t _Alarm_Type) {

				// End Function
				return(bitRead(this->Device.Status, _Alarm_Type));

			}

			// Over Voltage Alarm
			inline bool Alarm_OV(void) {

				// End Function
				return(bitRead(this->Device.Status, __BIT_OV_VRMSA__) or bitRead(this->Device.Status, __BIT_OV_VRMSB__) or bitRead(this->Device.Status, __BIT_OV_VRMSC__));

			}

			// Under Voltage Alarm
			inline bool Alarm_UV(void) {

				// End Function
				return(bitRead(this->Device.Status, __BIT_UN_VRMSA__) or bitRead(this->Device.Status, __BIT_UN_VRMSB__) or bitRead(this->Device.Status, __BIT_UN_VRMSC__));

			}

			// Over Current Alarm
			inline bool Alarm_OC(void) {

				// End Function
				return(bitRead(this->Device.Status, __BIT_OV_IRMSA__) or bitRead(this->Device.Status, __BIT_OV_IRMSB__) or bitRead(this->Device.Status, __BIT_OV_IRMSC__));

			}

			// Over Frequency Alarm
			inline bool Alarm_OFQ(void) {

				// End Function
				return(bitRead(this->Device.Status, __BIT_OV_FREQ__));

			}

			// Under Frequency Alarm
			inline bool Alarm_UFQ(void) {

				// End Function
				return(bitRead(this->Device.Status, __BIT_UN_FREQ__));

			}

			// Voltage Imbalance Alarm
			inline bool Alarm_VImb(void) {

				// End Function
				return(bitRead(this->Device.Status, __BIT_V_IMBAL__));

			}

			// Current Imbalance Alarm
			inline bool Alarm_IImb(void) {

				// End Function
				return(bitRead(this->Device.Status, __BIT_I_IMBAL__));

			}

			// Under Power Factor Alarm
			inline bool Alarm_UPF(void) {

				// End Function
				return(bitRead(this->Device.Status, __BIT_UN_PFA__) or bitRead(this->Device.Status, __BIT_UN_PFB__) or bitRead(this->Device.Status, __BIT_UN_PFC__));

			}

			// Over Temperature Alarm
			inline bool Alarm_OT(void) {

				// End Function
				return(bitRead(this->Device.Status, __BIT_OV_TEMP__));

			}

			// Under Temperature Alarm
			inline bool Alarm_UT(void) {

				// End Function
				return(bitRead(this->Device.Status, __BIT_UN_TEMP__));

			}

			// Get Status Value
			uint32_t Status(void) {

				// End Function
				return(this->Device.Status);

			}

			// Limit Parameters Control Function.
			void Get_Status(void) {

				// Define Register
				Register STATUS {0x00, 0x15, 0}; // Alarm and device status bits

				// Read Status Register
				this->Device.Status = this->Register_Pointer_Read(STATUS);

			}

			// Clear IC Status Register.
			void Clear_Status(void) {

				// Define Register
				Register STATUS {0x00, 0x15, 0}; // Alarm and device status bits

				// Read STATUS Register
				uint32_t _STATUS_Current = this->Register_Pointer_Read(STATUS);

				// Clear Alarm Bits
				_STATUS_Current &= 0x00800003;

				// Write STATUS Register
				this->Register_Pointer_Set(STATUS, _STATUS_Current);

			}

			// Adjust Limit Parameters Function.
			void Adjust_Limit(void) {

				// Update Status
				this->Get_Status();

				// Control for OV Temp
				if (bitRead(this->Device.Status, __BIT_OV_TEMP__)) {
					
					// Set Limit
					this->Set_Limit(__T_MAX__, (this->Limit.T_Max - __MAX78630_Limit_Temperature_Max_Diff__));
					
				} else {
					
					// Set Limit
					this->Set_Limit(__T_MAX__, this->Limit.T_Max);
					
				}

				// Control for OV Temp
				if (bitRead(this->Device.Status, __BIT_UN_TEMP__)) {
					
					// Set Limit
					this->Set_Limit(__T_MIN__, (this->Limit.T_Min + __MAX78630_Limit_Temperature_Min_Diff__));
					
				} else {
					
					// Set Limit
					this->Set_Limit(__T_MIN__, this->Limit.T_Min);
					
				}

				// Control for OV Voltage
				if (bitRead(this->Device.Status, __BIT_OV_VRMSA__) or bitRead(this->Device.Status, __BIT_OV_VRMSB__) or bitRead(this->Device.Status, __BIT_OV_VRMSC__)) {
					
					// Set Limit
					this->Set_Limit(__VRMS_MAX__, (this->Limit.V_Max - __MAX78630_Limit_Voltage_Max_Diff__));
					
				} else {
					
					// Set Limit
					this->Set_Limit(__VRMS_MAX__, this->Limit.V_Max);
					
				}

				// Control for UV Voltage
				if (bitRead(this->Device.Status, __BIT_UN_VRMSA__) or bitRead(this->Device.Status, __BIT_UN_VRMSB__) or bitRead(this->Device.Status, __BIT_UN_VRMSC__)) {
					
					// Set Limit
					this->Set_Limit(__VRMS_MIN__, (this->Limit.V_Min + __MAX78630_Limit_Voltage_Min_Diff__));
					
				} else {
					
					// Set Limit
					this->Set_Limit(__VRMS_MIN__, this->Limit.V_Min);
					
				}

				// Control for High Frequency
				if (bitRead(this->Device.Status, __BIT_OV_FREQ__)) {
					
					// Set Limit
					this->Set_Limit(__F_MAX__, (this->Limit.FQ_Max - __MAX78630_Limit_Frequency_Max_Diff__));
					
				} else {
					
					// Set Limit
					this->Set_Limit(__F_MAX__, this->Limit.FQ_Max);
					
				}
				
				// Control for Low Frequency
				if (bitRead(this->Device.Status, __BIT_UN_FREQ__)) {
					
					// Set Limit
					this->Set_Limit(__F_MIN__, (this->Limit.FQ_Min + __MAX78630_Limit_Frequency_Min_Diff__));
					
				} else {
					
					// Set Limit
					this->Set_Limit(__F_MIN__, this->Limit.FQ_Min);
					
				}

				// Control for Voltage Imbalance
				if (bitRead(this->Device.Status, __BIT_V_IMBAL__)) {
					
					// Set Limit
					this->Set_Limit(__VIMB_MAX__, (this->Limit.VImb_Max + __MAX78630_Limit_VImb_Max_Diff__));
					
				} else {
					
					// Set Limit
					this->Set_Limit(__VIMB_MAX__, this->Limit.VImb_Max);
					
				}

				// Control for Current Imbalance
				if (bitRead(this->Device.Status, __BIT_I_IMBAL__)) {
					
					// Set Limit
					this->Set_Limit(__IIMB_MAX__, (this->Limit.IImb_Max + __MAX78630_Limit_IImb_Max_Diff__));
					
				} else {
					
					// Set Limit
					this->Set_Limit(__IIMB_MAX__, this->Limit.IImb_Max);
					
				}

			}

			//Limit Read Function.
			float Get_Limit(const uint8_t _Limit_Type) {

				// Decide Type
				switch (_Limit_Type) {

					case __VRMS_MIN__: {

						// Define Register
						Register VRMS_MIN {0x00, 0xB1, 23};	// Voltage lower alarm limit. Positive values only

						// Get Register
						return(this->Register_Pointer_Read(VRMS_MIN) * __MAX78630_Config_VScale__);

						// End Case
						break;

					}
					case __VRMS_MAX__: {

						// Define Register
						Register VRMS_MAX {0x00, 0xB4, 23};	// Voltage upper alarm limit. Positive values only

						// Get Register
						return(this->Register_Pointer_Read(VRMS_MAX) * __MAX78630_Config_VScale__);

						// End Case
						break;

					}
					case __IRMS_MAX__: {

						// Define Register
						Register IRMS_MAX {0x00, 0xF3, 23}; // Current upper alarm limit. Positive values only

						// Get Register
						return(this->Register_Pointer_Read(IRMS_MAX) * __MAX78630_Config_IScale__);

						// End Case
						break;

					}
					case __F_MIN__: {

						// Define Register
						Register F_MIN {0x01, 0x83, 16}; // Frequency Alarm Lower Limit

						// Get Register
						return(this->Register_Pointer_Read(F_MIN));

						// End Case
						break;

					}
					case __F_MAX__: {

						// Define Register
						Register F_MAX {0x01, 0x86, 16}; // Frequency Alarm Upper Limit

						// Get Register
						return(this->Register_Pointer_Read(F_MAX));

						// End Case
						break;

					}
					case __T_MIN__: {

						// Define Register
						Register T_MIN {0x01, 0x7A, 10}; // Temperature Alarm Lower Limit

						// Get Register
						return(this->Register_Pointer_Read(T_MIN));

						// End Case
						break;

					}
					case __T_MAX__: {

						// Define Register
						Register T_MAX {0x01, 0x7D, 10}; // Temperature Alarm Upper Limit

						// Get Register
						return(this->Register_Pointer_Read(T_MAX));

						// End Case
						break;

					}
					case __PF_MIN__: {

						// Define Register
						Register PF_MIN {0x01, 0x71, 22}; // Power Factor lower alarm limit

						// Get Register
						return(this->Register_Pointer_Read(PF_MIN));

						// End Case
						break;

					}
					case __VSAG_LIM__: {

						// Define Register
						Register VSAG_LIM {0x00, 0xB7, 23}; // RMS Voltage Sag threshold. Positive values only

						// Get Register
						return(this->Register_Pointer_Read(VSAG_LIM) * __MAX78630_Config_VScale__);

						// End Case
						break;

					}
					case __VIMB_MAX__: {

						// Define Register
						Register V_IMB_MAX {0x00, 0x81, 23}; // Voltage imbalance alarm limit. Positive values only

						// Get Register
						return(this->Register_Pointer_Read(V_IMB_MAX));

						// End Case
						break;

					}
					case __IIMB_MAX__: {

						// Define Register
						Register I_IMB_MAX {0x00, 0x84, 23}; // Current imbalance alarm limit. Positive values only

						// Get Register
						return(this->Register_Pointer_Read(I_IMB_MAX));

						// End Case
						break;

					}

				}

				// End Function
				return(0);

			}

			// Set Sticky Function
			bool Get_Sticky(void) {

				// Define Register
				Register STICKY {0x00, 0x2D, 0}; // Alarm/status bits to hold until cleared by host

				// Return Result
				uint32_t _Sticky = this->Register_Pointer_Read(STICKY);

				// Decide Status
				if (_Sticky != 0x800001) {return(true);} else {return(false);}

				// End Function
				return(false);

			}

			// Get Alarm Mask
			uint32_t Get_Alarm_Mask(const uint8_t _Mask) {

				// Control for __MASK_AL1__
				if (_Mask == __MASK_AL1__) {

					// Define Register
					Register MASK1 {0x00, 0x1E, 0}; // Alarm Mask address 1

					// Get Register
					return(this->Register_Pointer_Read(MASK1));

				} else if (_Mask == __MASK_AL2__) {

					// Define Register
					Register MASK2 {0x00, 0x21, 0}; // Alarm Mask address 2

					// Get Register
					return(this->Register_Pointer_Read(MASK2));

				} else {

					// End Function
					return(0);

				}

			}

	};

#endif /* defined(__MAX78630__) */