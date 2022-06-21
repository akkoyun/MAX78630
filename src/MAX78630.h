#ifndef __MAX78630__
#define __MAX78630__

// Define Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Phase Type
#define __Phase_R__					(uint8_t)1
#define __Phase_S__					(uint8_t)2
#define __Phase_T__					(uint8_t)3
#define __Phase_Avg__				(uint8_t)4

// Scale Variables
#define __VSCALE__ 					667
#define __ISCALE__ 					7

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
#define __Active_Recieved__ 		(uint8_t)12
#define __Active_Delivered__ 		(uint8_t)13
#define __ReActive_Recieved__ 		(uint8_t)14
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


class MAX78630 {

	private:

		// Stream Object Definition
		Stream * Serial_Energy;

		// Device Register Structure
		struct Register {
			const uint8_t High_Address;
			const uint8_t Low_Address;
			const uint8_t Data_Type;
		};

		// Hardware Functions
		uint32_t Get_Device_Address(void) {

			// Define Objects
			Register DEVADDR {0x00, 0x4E, 0};

			// Declare Variable
			uint32_t _Result = 0;

			// Decide Command
			_Result = Register_Pointer_Read(DEVADDR);
			
			// End Function
			return(_Result);

		}
		uint32_t Get_System_Stat(void) {

			// Define Objects
			Register SYSSTAT {0x02, 0x43, 0};

			// Declare Variable
			uint32_t _Result = 0;

			// Decide Command
			_Result = Register_Pointer_Read(SYSSTAT);
			
			// End Function
			return(_Result);

		}
		uint32_t Get_Firmware(void) {

			// Define Objects
			Register FW_VERSION {0x00, 0x03, 0};

			// Declare Variable
			uint32_t _Result = 0;

			// Decide Command
			_Result = Register_Pointer_Read(FW_VERSION);
			
			// End Function
			return(_Result);

		}

		// Data Refresh Functions	
		uint32_t Get_Frame(void) {

			// Define Objects
			Register FRAME {0x00, 0x12, 0}; // Low-rate sample counter

			// Declare Variable
			uint32_t _Result = 0;

			// Decide Command
			_Result = Register_Pointer_Read(FRAME);
			
			// End Function
			return(_Result);

		}
		uint32_t Get_Cycle(void) {

			// Define Objects
			Register CYCLE {0x00, 0x0F, 0}; // High-rate sample counter

			// Declare Variable
			uint32_t _Result = 0;

			// Decide Command
			_Result = Register_Pointer_Read(CYCLE);
			
			// End Function
			return(_Result);

		}
		uint32_t Get_Divisor(void) {

			// Define Objects
			Register DIVISOR {0x00, 0x0C, 0}; // Actual samples in previous accumulation interval

			// Declare Variable
			uint32_t _Result = 0;

			// Decide Command
			_Result = Register_Pointer_Read(DIVISOR);
			
			// End Function
			return(_Result);

		}
		uint32_t Get_Samples(void) {

			// Define Objects
			Register SAMPLES {0x00, 0x09, 0}; // Minimum high-rate samples per accumulation interval

			// Declare Variable
			uint32_t _Result = 0;

			// Decide Command
			_Result = Register_Pointer_Read(SAMPLES);
			
			// End Function
			return(_Result);

		}

		// Communication Functions
		uint32_t Read_Baud(void) {

			// Define Objects
			Register BAUD {0x00, 0x51, 0}; // Baud rate for UART interface

			// Read Register
			uint32_t _Baud = Register_Pointer_Read(BAUD);

			// End Function
			return(_Baud);

		}
		bool Write_Baud(uint32_t _Baud) {

			// Define Objects
			Register BAUD {0x00, 0x51, 0}; // Baud rate for UART interface

			// Set Register
			bool _Response = Register_Pointer_Set(BAUD, _Baud);

			// End Function
			return(_Response);

		}

		// Scale Functions
		bool Write_VScale(uint32_t _Voltage_Scale) {

			// Define Objects
			Register VSCALE {0x01, 0x44, 0};

			// Set Register
			bool _Response = Register_Pointer_Set(VSCALE, _Voltage_Scale);

			// End Function
			return(_Response);

		}
		bool Write_IScale(uint32_t _Current_Scale) {

			// Define Objects
			Register IFSCALE {0x01, 0x41, 0};

			// Set Register
			bool _Response = Register_Pointer_Set(IFSCALE, _Current_Scale);

			// End Function
			return(_Response);

		}
		bool Set_Scale(uint32_t _VScale, uint32_t _IScale) {

			// VScale Set Command
			bool _Response_VScale = Write_VScale(_VScale);

			// IScale Set Command
			bool _Response_IScale = Write_IScale(_IScale);

			// End Function
			return(_Response_VScale and _Response_IScale);

		}

		// Calibration Functions
		bool Voltage_Calibration(const uint8_t _Phase, const float _Gain, const float _Offset) {

			// Control Input Parameters
			if (_Phase > 3) return(false);

			// Phase R Calibration
			if (_Phase == __Phase_R__) {

				// Define Objects
				Register V1_GAIN {0x00, 0x5D, 21}; // Voltage Gain Calibration
				Register V1_OFFS {0x00, 0x6F, 23}; // Voltage Offset Calibration

				bool _Result_Gain = Register_Pointer_Set(V1_GAIN, _FtoS(_Gain, V1_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = Register_Pointer_Set(V1_OFFS, _FtoS(_Offset, V1_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// Phase S Calibration
			if (_Phase == __Phase_S__) {

				// Define Objects
				Register V2_GAIN {0x00, 0x60, 21}; // Voltage Gain Calibration
				Register V2_OFFS {0x00, 0x72, 23}; // Voltage Offset Calibration

				bool _Result_Gain = Register_Pointer_Set(V2_GAIN, _FtoS(_Gain, V2_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = Register_Pointer_Set(V2_OFFS, _FtoS(_Offset, V2_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// Phase T Calibration
			if (_Phase == __Phase_T__) {

				// Define Objects
				Register V3_GAIN {0x00, 0x63, 21}; // Voltage Gain Calibration
				Register V3_OFFS {0x00, 0x75, 23}; // Voltage Offset Calibration

				bool _Result_Gain = Register_Pointer_Set(V3_GAIN, _FtoS(_Gain, V3_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = Register_Pointer_Set(V3_OFFS, _FtoS(_Offset, V3_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// End Function
			return(false);

		}
		bool Current_Calibration(const uint8_t _Phase, const float _Gain, const float _Offset) {

			// Control Input Parameters
			if (_Phase > 3) return(false);

			// Phase R Calibration
			if (_Phase == __Phase_R__) {

				// Define Objects
				Register I1_GAIN {0x00, 0x54, 21}; // Current Gain Calibration
				Register I1_OFFS {0x00, 0x66, 23}; // Current Offset Calibration

				bool _Result_Gain = Register_Pointer_Set(I1_GAIN, _FtoS(_Gain, I1_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = Register_Pointer_Set(I1_OFFS, _FtoS(_Offset, I1_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// Phase S Calibration
			if (_Phase == __Phase_S__) {

				// Define Objects
				Register I2_GAIN {0x00, 0x57, 21}; // Current Gain Calibration
				Register I2_OFFS {0x00, 0x69, 23}; // Current Offset Calibration

				bool _Result_Gain = Register_Pointer_Set(I2_GAIN, _FtoS(_Gain, I2_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = Register_Pointer_Set(I2_OFFS, _FtoS(_Offset, I2_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// Phase T Calibration
			if (_Phase == __Phase_T__) {

				// Define Objects
				Register I3_GAIN {0x00, 0x5A, 21}; // Current Gain Calibration
				Register I3_OFFS {0x00, 0x6C, 23}; // Current Offset Calibration

				bool _Result_Gain = Register_Pointer_Set(I3_GAIN, _FtoS(_Gain, I3_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = Register_Pointer_Set(I3_OFFS, _FtoS(_Offset, I3_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// End Function
			return(false);

		}
		bool Temperature_Calibration(const float _Gain, const float _Offset) {

			// Define Objects
			Register T_GAIN {0x00, 0x78, 0}; // Temperature Slope Calibration
			Register T_OFFS {0x00, 0x7B, 0}; // Temperature Offset Calibration

			bool _Result_Gain = Register_Pointer_Set(T_GAIN, _FtoS(_Gain, T_GAIN.Data_Type)); // Write Gain
			bool _Result_Offset = Register_Pointer_Set(T_OFFS, _FtoS(_Offset, T_GAIN.Data_Type)); // Write Offset

			// End Function
			return(_Result_Gain and _Result_Offset);

		}
		float Voltage_HPF_COEF(float _COEF) {

			// Define Objects
			Register HPF_COEF_V {0x00, 0x3F, 23}; // Voltage input HPF coefficient

			// Declare Variable
			float _Result = 0;

			// Decide Workflow
			if (_COEF == -999) {

				// Read Register
				_Result = Register_Pointer_Read(HPF_COEF_V); // Measure Phase R

			} else {

				// Set Register
				_Result = Register_Pointer_Set(HPF_COEF_V, _FtoS(_COEF, 23)); // Measure Phase R

			}
			
			// End Function
			return(_Result);

		}
		float Current_HPF_COEF(float _COEF) {

			// Define Objects
			Register HPF_COEF_I {0x00, 0x3C, 23}; // Current input HPF coefficient

			// Declare Variable
			float _Result = 0;

			// Decide Workflow
			if (_COEF == -999) {

				// Read Register
				_Result = Register_Pointer_Read(HPF_COEF_I); // Measure Phase R

			} else {

				// Set Register
				_Result = Register_Pointer_Set(HPF_COEF_I, _FtoS(_COEF, 23)); // Measure Phase R

			}
			
			// End Function
			return(_Result);

		}
		float Current_RMS_Offset(char Phase, float _Offset) {

			// Define Objects
			Register IARMS_OFF {0x00, 0xC3, 23}; // RMS Current dynamic offset adjust A
			Register IBRMS_OFF {0x00, 0xC6, 23}; // RMS Current dynamic offset adjust B
			Register ICRMS_OFF {0x00, 0xC9, 23}; // RMS Current dynamic offset adjust C

			// Declare Variable
			float _Result = 0;

			// Decide Workflow
			if (_Offset == -999) {

				// Read Register
				if (Phase == 'R') _Result = Register_Pointer_Read(IARMS_OFF); // Measure Phase R
				if (Phase == 'S') _Result = Register_Pointer_Read(IBRMS_OFF); // Measure Phase S
				if (Phase == 'T') _Result = Register_Pointer_Read(ICRMS_OFF); // Measure Phase T

			} else {

				// Set Register
				if (Phase == 'R') _Result = Register_Pointer_Set(IARMS_OFF, _FtoS(_Offset, 23)); // Measure Phase R
				if (Phase == 'S') _Result = Register_Pointer_Set(IBRMS_OFF, _FtoS(_Offset, 23)); // Measure Phase S
				if (Phase == 'T') _Result = Register_Pointer_Set(ICRMS_OFF, _FtoS(_Offset, 23)); // Measure Phase T

			}

			// End Function
			return(_Result);

		}
		float Active_Power_Offset(char Phase, float _Offset) {

			// Define Objects
			Register PA_OFFS {0x01, 0x14, 23}; // Active Power dynamic offset adjust A
			Register PB_OFFS {0x01, 0x17, 23}; // Active Power dynamic offset adjust B
			Register PC_OFFS {0x01, 0x1A, 23}; // Active Power dynamic offset adjust C

			// Declare Variable
			float _Result = 0;

			// Decide Workflow
			if (_Offset == -999) {

				// Read Register
				if (Phase == 'R') _Result = Register_Pointer_Read(PA_OFFS); // Measure Phase R
				if (Phase == 'S') _Result = Register_Pointer_Read(PB_OFFS); // Measure Phase S
				if (Phase == 'T') _Result = Register_Pointer_Read(PC_OFFS); // Measure Phase T

			} else {

				// Set Register
				if (Phase == 'R') _Result = Register_Pointer_Set(PA_OFFS, _FtoS(_Offset, 23)); // Measure Phase R
				if (Phase == 'S') _Result = Register_Pointer_Set(PB_OFFS, _FtoS(_Offset, 23)); // Measure Phase S
				if (Phase == 'T') _Result = Register_Pointer_Set(PC_OFFS, _FtoS(_Offset, 23)); // Measure Phase T

			}
			
			// End Function
			return(_Result);

		}
		float ReActive_Power_Offset(char Phase, float _Offset) {

			// Define Objects
			Register QA_OFFS {0x01, 0x0B, 23}; // Reactive Power dynamic offset adjust A
			Register QB_OFFS {0x01, 0x0E, 23}; // Reactive Power dynamic offset adjust B
			Register QC_OFFS {0x01, 0x11, 23}; // Reactive Power dynamic offset adjust C

			// Declare Variable
			float _Result = 0;

			// Decide Workflow
			if (_Offset == -999) {

				// Read Register
				if (Phase == 'R') _Result = Register_Pointer_Read(QA_OFFS); // Measure Phase R
				if (Phase == 'S') _Result = Register_Pointer_Read(QB_OFFS); // Measure Phase S
				if (Phase == 'T') _Result = Register_Pointer_Read(QC_OFFS); // Measure Phase T

			} else {

				// Set Register
				if (Phase == 'R') _Result = Register_Pointer_Set(QA_OFFS, _FtoS(_Offset, 23)); // Measure Phase R
				if (Phase == 'S') _Result = Register_Pointer_Set(QB_OFFS, _FtoS(_Offset, 23)); // Measure Phase S
				if (Phase == 'T') _Result = Register_Pointer_Set(QC_OFFS, _FtoS(_Offset, 23)); // Measure Phase T

			}

			// End Function
			return(_Result);

		}

		// Bucket Functions
		bool Write_Bucket(uint32_t _Bucket_H, uint32_t _Bucket_L) {

			// Define Objects
			Register BUCKET_LOW		{0x01, 0xD1, 0}; // Energy Bucket Size – Low word
			Register BUCKET_HIGH	{0x01, 0xD4, 0}; // Energy Bucket Size – High word

			// Set Command
			bool _Result_LOW = Register_Pointer_Set(BUCKET_LOW, _Bucket_L);
			bool _Result_HIGH = Register_Pointer_Set(BUCKET_HIGH, _Bucket_H);

			// Handle Response
			if (_Result_LOW and _Result_HIGH) return(false);

			// End Function
			return(true);

		}

		// Limit Functions
		bool Write_Limit(uint8_t _Limit_ID, float _Value);

		// Stream Functions
		void Clear_Buffer(void) {

			// Clear UART Buffer
			Serial_Energy->flush(); 
			
			while(Serial_Energy->available() > 0) {
				
				Serial_Energy->read(); 
				delay(5);

			}

		}

		// Register Functions
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
			Clear_Buffer();

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
		bool Register_Pointer_Set(Register _Command, uint32_t _Data) {

			// Clear Buffer
			Clear_Buffer();

			// Convert Parameters
			char _Parameter1 = _Data;
			char _Parameter2 = _Data>>8;
			char _Parameter3 = _Data>>16;

			// Calculate CheckSum
			uint8_t ChkS = 0x100 - ((0xAA + 0x0A + 0xA3 + _Command.Low_Address + _Command.High_Address + 0xD3 + _Parameter1 + _Parameter2 + _Parameter3) % 256); // Calculate checksum

			// Clear Serial Buffer
			Clear_Buffer();

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
			Serial_Energy->write(ChkS);

			// Command Delay
			delay(20);

			// Declare Variable
			uint8_t _Response[1];
			memset(_Response, '\0', 1);
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

			// End Function
			if (_Response[0 == 0xAD]) return(true);

			// End Function
			return(false);
			
		}
		uint32_t _FtoS(double _Variable, uint8_t _Data_Type) {

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

	public:

		// Object Declaration Functions
		bool Begin(Stream & _Serial) {

			//Set serial port
			Serial_Energy = & _Serial;

			// Clear Serial Buffer
			Clear_Buffer();

			// Select IC Command
			Serial_Energy->write(0xAA);			// Header (0xAA)
			Serial_Energy->write(0x04);			// Total Sended Byte (0x04)
			Serial_Energy->write(0xC4);			// Setting Command (0xC4)
			Serial_Energy->write(0x8E);			// CheckSum (0x8E)

			// Command Delay
			delay(10);

			// Clear Serial Buffer
			Clear_Buffer();

			// User Register Definition
			uint8_t _Config_Reg = 0b11110000;

			// Calculate CheckSum
			uint8_t _Calibration_CheckSum = 0x100 - ((0xAA + 0x07 + 0xCA + 0x65 + 0xFF + _Config_Reg) % 256);

			// Calibration Command TODO: Komut tanınmadı hatası alıyorum
			Serial_Energy->write(0xAA);						// Header (0xAA)
			Serial_Energy->write(0x07);						// Total Sended Byte (0x07)
			Serial_Energy->write(0xCA);						// Setting Command (0xCA)
			Serial_Energy->write(_Config_Reg);					// Config Register (0x)
			Serial_Energy->write(0xFF);						// Calibration Setting (0xFF)
			Serial_Energy->write(0x65);						// Setting Command (0x65)
			Serial_Energy->write(_Calibration_CheckSum);		// CheckSum (0xD1)

			// Command Delay
			delay(10);

			// Set Scale Registers
			Set_Scale(__VSCALE__, __ISCALE__);

			// Set Voltage Limits
			Set_Voltage_Limits(192, 253);

			// Set Frequency Limits
			Set_Frequency_Limits(47, 52);

			// Set Imbalance Limits
			Set_Imbalance_Limits(0.06, 0.06);

			// Bucket Set Command
			Write_Bucket(0x000821, 0xD0F4C2);

			// Set Min and Max Values
			Set_Min_Max_Address(1, 0x30); // VR
			Set_Min_Max_Address(2, 0x31); // VS
			Set_Min_Max_Address(3, 0x32); // VT
		//	Set_Min_Max_Address(4, 0x47); // IR
		//	Set_Min_Max_Address(5, 0x48); // IS
		//	Set_Min_Max_Address(6, 0x49); // IT

			// End Function
			return(true);

		}

		// Measurement Functions
		float Voltage(const uint8_t _Phase, const uint8_t _Type) {

			// Control Input Parameters
			if (_Phase > 4) return(0);

			// Declare Variable
			float _Result = 0;

			// Phase Select
			if (_Phase == __Phase_R__) {

				// Measurement Type Select
				if (_Type == __RMS__) {

					// Define Objects
					Register VA_RMS {0x00, 0x90, 23}; // RMS Voltage A

					// Get Measurement
					_Result = Register_Pointer_Read(VA_RMS); // Measure Phase R

				} else if (_Type == __Instant__) {

					// Define Objects
					Register VA {0x00, 0x87, 23}; // Instantaneous Voltage A

					// Get Measurement
					_Result = Register_Pointer_Read(VA); // Measure Phase R

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register VFUND_A {0x00, 0x9C, 23}; // Fundamental Voltage A

					// Get Measurement
					_Result = Register_Pointer_Read(VFUND_A); // Measure Phase R

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register VHARM_A {0x00, 0xA5, 23}; // Harmonic Voltage A

					// Get Measurement
					_Result = Register_Pointer_Read(VHARM_A); // Measure Phase R

				}

			} else if (_Phase == __Phase_S__) {

				// Measurement Type Select
				if (_Type == __RMS__) {

					// Define Objects
					Register VB_RMS {0x00, 0x93, 23}; // RMS Voltage B

					// Get Measurement
					_Result = Register_Pointer_Read(VB_RMS); // Measure Phase S

				} else if (_Type == __Instant__) {

					// Define Objects
					Register VB {0x00, 0x8A, 23}; // Instantaneous Voltage B

					// Get Measurement
					_Result = Register_Pointer_Read(VB); // Measure Phase S

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register VFUND_B {0x00, 0x9F, 23}; // Fundamental Voltage B

					// Get Measurement
					_Result = Register_Pointer_Read(VFUND_B); // Measure Phase S

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register VHARM_B {0x00, 0xA8, 23}; // Harmonic Voltage B

					// Get Measurement
					_Result = Register_Pointer_Read(VHARM_B); // Measure Phase S

				}

			} else if (_Phase == __Phase_T__) {

				// Measurement Type Select
				if (_Type == __RMS__) {

					// Define Objects
					Register VC_RMS {0x00, 0x96, 23}; // RMS Voltage C

					// Get Measurement
					_Result = Register_Pointer_Read(VC_RMS); // Measure Phase R

				} else if (_Type == __Instant__) {

					// Define Objects
					Register VC {0x00, 0x8D, 23}; // Instantaneous Voltage C

					// Get Measurement
					_Result = Register_Pointer_Read(VC); // Measure Phase R

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register VFUND_C {0x00, 0xA2, 23}; // Fundamental Voltage C

					// Get Measurement
					_Result = Register_Pointer_Read(VFUND_C); // Measure Phase R

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register VHARM_C {0x00, 0xAB, 23}; // Harmonic Voltage C

					// Get Measurement
					_Result = Register_Pointer_Read(VHARM_C); // Measure Phase R

				}

			} else if (_Phase == __Phase_Avg__) {

				// Measurement Type Select
				if (_Type == __RMS__) {

					// Define Objects
					Register VT_RMS {0x00, 0x99, 23}; // RMS Voltage average

					// Get Measurement
					_Result = Register_Pointer_Read(VT_RMS); // Measure Phase Average

				}

			}

			// End Function
			return(_Result * __VSCALE__);

		}
		float Current(const uint8_t _Phase, const uint8_t _Type) {

			// Control Input Parameters
			if (_Phase > 4) return(0);

			// Declare Variable
			float _Result = 0;

			// Phase Select
			if (_Phase == __Phase_R__) {

				// Measurement Type Select
				if (_Type == __RMS__) {

					// Define Objects
					Register IA_RMS {0x00, 0xD5, 23}; // RMS Current A

					// Get Measurement
					_Result = Register_Pointer_Read(IA_RMS); // Measure Phase R

				} else if (_Type == __Instant__) {

					// Define Objects
					Register IA	{0x00, 0xBA, 23}; // Instantaneous Current A

					// Get Measurement
					_Result = Register_Pointer_Read(IA); // Measure Phase R

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register IFUND_A {0x00, 0xE1, 23}; // Fundamental Current A

					// Get Measurement
					_Result = Register_Pointer_Read(IFUND_A); // Measure Phase R

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register IHARM_A {0x00, 0xEA, 23}; // Harmonic Current A

					// Get Measurement
					_Result = Register_Pointer_Read(IHARM_A); // Measure Phase R

				} else if (_Type == __Peak__) {

					// Define Objects
					Register IA_PEAK {0x00, 0xCC, 23}; // Peak Current A

					// Get Measurement
					_Result = Register_Pointer_Read(IA_PEAK); // Measure Phase R

				} else if (_Type == __Phase_Compensation__) {

					// Define Objects
					Register PHASECOMP1 {0x00, 0x42, 21}; // Phase compensation A

					// Get Measurement
					_Result = Register_Pointer_Read(PHASECOMP1); // Measure Phase R

				}

			} else if (_Phase == __Phase_S__) {

				// Measurement Type Select
				if (_Type == __RMS__) {

					// Define Objects
					Register IB_RMS {0x00, 0xD8, 23}; // RMS Current B

					// Get Measurement
					_Result = Register_Pointer_Read(IB_RMS); // Measure Phase R

				} else if (_Type == __Instant__) {

					// Define Objects
					Register IB	{0x00, 0xBD, 23}; // Instantaneous Current B

					// Get Measurement
					_Result = Register_Pointer_Read(IB); // Measure Phase R

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register IFUND_B {0x00, 0xE4, 23}; // Fundamental Current B

					// Get Measurement
					_Result = Register_Pointer_Read(IFUND_B); // Measure Phase R

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register IHARM_B {0x00, 0xED, 23}; // Harmonic Current B

					// Get Measurement
					_Result = Register_Pointer_Read(IHARM_B); // Measure Phase R

				} else if (_Type == __Peak__) {

					// Define Objects
					Register IB_PEAK {0x00, 0xCF, 23}; // Peak Current B

					// Get Measurement
					_Result = Register_Pointer_Read(IB_PEAK); // Measure Phase R

				} else if (_Type == __Phase_Compensation__) {

					// Define Objects
					Register PHASECOMP2 {0x00, 0x45, 21}; // Phase compensation B

					// Get Measurement
					_Result = Register_Pointer_Read(PHASECOMP2); // Measure Phase R

				}

			} else if (_Phase == __Phase_T__) {

				// Measurement Type Select
				if (_Type == __RMS__) {

					// Define Objects
					Register IC_RMS {0x00, 0xDB, 23}; // RMS Current C

					// Get Measurement
					_Result = Register_Pointer_Read(IC_RMS); // Measure Phase R

				} else if (_Type == __Instant__) {

					// Define Objects
					Register IC	{0x00, 0xC0, 23}; // Instantaneous Current C

					// Get Measurement
					_Result = Register_Pointer_Read(IC); // Measure Phase R

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register IFUND_C {0x00, 0xE7, 23}; // Fundamental Current C

					// Get Measurement
					_Result = Register_Pointer_Read(IFUND_C); // Measure Phase R

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register IHARM_C {0x00, 0xF0, 23}; // Harmonic Current C

					// Get Measurement
					_Result = Register_Pointer_Read(IHARM_C); // Measure Phase R

				} else if (_Type == __Peak__) {

					// Define Objects
					Register IC_PEAK {0x00, 0xD2, 23}; // Peak Current C

					// Get Measurement
					_Result = Register_Pointer_Read(IC_PEAK); // Measure Phase R

				}else if (_Type == __Phase_Compensation__) {

					// Define Objects
					Register PHASECOMP3 {0x00, 0x48, 21}; // Phase compensation C

					// Get Measurement
					_Result = Register_Pointer_Read(PHASECOMP3); // Measure Phase R

				}

			} else if (_Phase == __Phase_Avg__) {

				// Measurement Type Select
				if (_Type == __RMS__) {

					// Define Objects
					Register IT_RMS {0x00, 0xDE, 23}; // RMS Current average

					// Get Measurement
					_Result = Register_Pointer_Read(IT_RMS); // Measure Phase Average

				}

			}

			// End Function
			return(_Result * __ISCALE__);

		}
		float Power(const uint8_t _Phase, const uint8_t _Type) {

			// Control Input Parameters
			if (_Phase > 4) return(0);

			// Declare Variable
			float _Result = 0;

			// Phase Select
			if (_Phase == __Phase_R__) {

				// Measurement Type Select
				if (_Type == __Active__) {

					// Define Objects
					Register WATT_A	{0x01, 0x1D, 23}; // Active Power A

					// Get Measurement
					_Result = Register_Pointer_Read(WATT_A); // Measure Phase R

				} else if (_Type == __ReActive__) {

					// Define Objects
					Register VAR_A {0x01, 0x26, 23}; // Reactive Power A

					// Get Measurement
					_Result = Register_Pointer_Read(VAR_A); // Measure Phase R

				} else if (_Type == __Apparent__) {

					// Define Objects
					Register VA_A {0x01, 0x2F, 23}; // Apparent Power A

					// Get Measurement
					_Result = Register_Pointer_Read(VA_A); // Measure Phase R

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register PFUND_A {0x01, 0x4A, 23}; // Fundamental Power A

					// Get Measurement
					_Result = Register_Pointer_Read(PFUND_A); // Measure Phase R

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register PHARM_A {0x01, 0x53, 23}; // Harmonic Power A

					// Get Measurement
					_Result = Register_Pointer_Read(PHARM_A); // Measure Phase R

				} else if (_Type == __Fund_ReActive__) {

					// Define Objects
					Register QFUND_A {0x00, 0xF9, 23}; // Fundamental Reactive Power A

					// Get Measurement
					_Result = Register_Pointer_Read(QFUND_A); // Measure Phase R

				} else if (_Type == __Harm_ReActive__) {

					// Define Objects
					Register QHARM_A {0x01, 0x02, 23}; // Harmonic Reactive Power A

					// Get Measurement
					_Result = Register_Pointer_Read(QHARM_A); // Measure Phase R

				} else if (_Type == __Fund_VA__) {

					// Define Objects
					Register VAFUNDA {0x01, 0x5C, 23}; // Fundamental Volt Amperes A

					// Get Measurement
					_Result = Register_Pointer_Read(VAFUNDA); // Measure Phase R

				}

			} else if (_Phase == __Phase_S__) {

				// Measurement Type Select
				if (_Type == __Active__) {

					// Define Objects
					Register WATT_B	{0x01, 0x20, 23}; // Active Power B

					// Get Measurement
					_Result = Register_Pointer_Read(WATT_B); // Measure Phase R

				} else if (_Type == __ReActive__) {

					// Define Objects
					Register VAR_B {0x01, 0x29, 23}; // Reactive Power B

					// Get Measurement
					_Result = Register_Pointer_Read(VAR_B); // Measure Phase R

				} else if (_Type == __Apparent__) {

					// Define Objects
					Register VA_B {0x01, 0x32, 23}; // Apparent Power B

					// Get Measurement
					_Result = Register_Pointer_Read(VA_B); // Measure Phase R

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register PFUND_B {0x01, 0x4D, 23}; // Fundamental Power B

					// Get Measurement
					_Result = Register_Pointer_Read(PFUND_B); // Measure Phase R

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register PHARM_B {0x01, 0x56, 23}; // Harmonic Power B

					// Get Measurement
					_Result = Register_Pointer_Read(PHARM_B); // Measure Phase R

				} else if (_Type == __Fund_ReActive__) {

					// Define Objects
					Register QFUND_B {0x00, 0xFC, 23}; // Fundamental Reactive Power B

					// Get Measurement
					_Result = Register_Pointer_Read(QFUND_B); // Measure Phase R

				} else if (_Type == __Harm_ReActive__) {

					// Define Objects
					Register QHARM_B {0x01, 0x05, 23}; // Harmonic Reactive Power B

					// Get Measurement
					_Result = Register_Pointer_Read(QHARM_B); // Measure Phase R

				} else if (_Type == __Fund_VA__) {

					// Define Objects
					Register VAFUNDB {0x01, 0x5F, 23}; // Fundamental Volt Amperes B

					// Get Measurement
					_Result = Register_Pointer_Read(VAFUNDB); // Measure Phase R

				}

			} else if (_Phase == __Phase_T__) {

				// Measurement Type Select
				if (_Type == __Active__) {

					// Define Objects
					Register WATT_C	{0x01, 0x23, 23}; // Active Power C

					// Get Measurement
					_Result = Register_Pointer_Read(WATT_C); // Measure Phase R

				} else if (_Type == __ReActive__) {

					// Define Objects
					Register VAR_C {0x01, 0x2C, 23}; // Reactive Power C

					// Get Measurement
					_Result = Register_Pointer_Read(VAR_C); // Measure Phase R

				} else if (_Type == __Apparent__) {

					// Define Objects
					Register VA_C {0x01, 0x35, 23}; // Apparent Power C

					// Get Measurement
					_Result = Register_Pointer_Read(VA_C); // Measure Phase R

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register PFUND_C {0x01, 0x50, 23}; // Fundamental Power C

					// Get Measurement
					_Result = Register_Pointer_Read(PFUND_C); // Measure Phase R

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register PHARM_C {0x01, 0x59, 23}; // Harmonic Power C

					// Get Measurement
					_Result = Register_Pointer_Read(PHARM_C); // Measure Phase R

				} else if (_Type == __Fund_ReActive__) {

					// Define Objects
					Register QFUND_C {0x00, 0xFF, 23}; // Fundamental Reactive Power C

					// Get Measurement
					_Result = Register_Pointer_Read(QFUND_C); // Measure Phase R

				} else if (_Type == __Harm_ReActive__) {

					// Define Objects
					Register QHARM_C {0x01, 0x08, 23}; // Harmonic Reactive Power C

					// Get Measurement
					_Result = Register_Pointer_Read(QHARM_C); // Measure Phase R

				} else if (_Type == __Fund_VA__) {

					// Define Objects
					Register VAFUNDC {0x01, 0x62, 23}; // Fundamental Volt Amperes C

					// Get Measurement
					_Result = Register_Pointer_Read(VAFUNDC); // Measure Phase R

				}

			} else if (_Phase == __Phase_Avg__) {

				// Measurement Type Select
				if (_Type == __Active__) {

					// Define Objects
					Register WATT_T	{0x01, 0x38, 23}; // Active Power average

					// Get Measurement
					_Result = Register_Pointer_Read(WATT_T); // Measure Phase R

				} else if (_Type == __ReActive__) {

					// Define Objects
					Register VAR_T {0x01, 0x3B, 23}; // Reactive Power average

					// Get Measurement
					_Result = Register_Pointer_Read(VAR_T); // Measure Phase R

				} else if (_Type == __Apparent__) {

					// Define Objects
					Register VA_T {0x01, 0x3E, 23}; // Apparent Power average

					// Get Measurement
					_Result = Register_Pointer_Read(VA_T); // Measure Phase R

				}

			}

			// End Function
			return(_Result * __ISCALE__ * __VSCALE__);

		}
		float Energy(const uint8_t _Phase, const uint8_t _Type) {

			// Control Input Parameters
			if (_Phase > 3) return(0);

			// Declare Variable
			float _Result = 0;

			// Phase Select
			if (_Phase == __Phase_R__) {

				// Measurement Type Select
				if (_Type == __Active_Recieved__) {

					// Define Objects
					Register WHA_POS {0x01, 0xDD, 0}; // Received Active Energy Counter A

					// Get Measurement
					_Result = Register_Pointer_Read(WHA_POS); // Measure Phase R

				} else if (_Type == __Active_Delivered__) {

					// Define Objects
					Register WHA_NEG {0x01, 0xE6, 0}; // Delivered Active Energy Counter A

					// Get Measurement
					_Result = Register_Pointer_Read(WHA_NEG); // Measure Phase R

				} else if (_Type == __ReActive_Recieved__) {

					// Define Objects
					Register VARHA_POS {0x02, 0x13, 0}; // Reactive Energy Leading Counter A

					// Get Measurement
					_Result = Register_Pointer_Read(VARHA_POS); // Measure Phase R

				} else if (_Type == __ReActive_Delivered__) {

					// Define Objects
					Register VARHA_NEG {0x02, 0x1C, 0}; // Reactive Energy Lagging Counter A

					// Get Measurement
					_Result = Register_Pointer_Read(VARHA_NEG); // Measure Phase R

				} else if (_Type == __Energy_Reset__) {

					// Define Objects
					Register WHA_POS {0x01, 0xDD, 0}; // Received Active Energy Counter A

					// Get Measurement
					Register_Pointer_Set(WHA_POS, 0x00); // Measure Phase R

					// End Function
					return(1);

				}

			} else if (_Phase == __Phase_S__) {

				// Measurement Type Select
				if (_Type == __Active_Recieved__) {

					// Define Objects
					Register WHB_POS {0x01, 0xEF, 0}; // Received Active Energy Counter B

					// Get Measurement
					_Result = Register_Pointer_Read(WHB_POS); // Measure Phase R

				} else if (_Type == __Active_Delivered__) {

					// Define Objects
					Register WHB_NEG {0x01, 0xF8, 0}; // Delivered Active Energy Counter B

					// Get Measurement
					_Result = Register_Pointer_Read(WHB_NEG); // Measure Phase R

				} else if (_Type == __ReActive_Recieved__) {

					// Define Objects
					Register VARHB_POS {0x02, 0x25, 0}; // Reactive Energy Leading Counter B

					// Get Measurement
					_Result = Register_Pointer_Read(VARHB_POS); // Measure Phase R

				} else if (_Type == __ReActive_Delivered__) {

					// Define Objects
					Register VARHB_NEG {0x02, 0x2E, 0}; // Reactive Energy Lagging Counter B

					// Get Measurement
					_Result = Register_Pointer_Read(VARHB_NEG); // Measure Phase R

				} else if (_Type == __Energy_Reset__) {

					// Define Objects
					Register WHB_POS {0x01, 0xEF, 0}; // Received Active Energy Counter B

					// Get Measurement
					Register_Pointer_Set(WHB_POS, 0x00); // Measure Phase R

					// End Function
					return(1);
					
				}

			} else if (_Phase == __Phase_T__) {

				// Measurement Type Select
				if (_Type == __Active_Recieved__) {

					// Define Objects
					Register WHC_POS {0x02, 0x01, 0}; // Received Active Energy Counter C

					// Get Measurement
					_Result = Register_Pointer_Read(WHC_POS); // Measure Phase R

				} else if (_Type == __Active_Delivered__) {

					// Define Objects
					Register WHC_NEG {0x02, 0x0A, 0}; // Delivered Active Energy Counter C

					// Get Measurement
					_Result = Register_Pointer_Read(WHC_NEG); // Measure Phase R

				} else if (_Type == __ReActive_Recieved__) {

					// Define Objects
					Register VARHC_POS {0x02, 0x37, 0}; // Reactive Energy Leading Counter C

					// Get Measurement
					_Result = Register_Pointer_Read(VARHC_POS); // Measure Phase R

				} else if (_Type == __ReActive_Delivered__) {

					// Define Objects
					Register VARHC_NEG {0x02, 0x40, 0}; // Reactive Energy Lagging Counter C

					// Get Measurement
					_Result = Register_Pointer_Read(VARHC_NEG); // Measure Phase R

				} else if (_Type == __Energy_Reset__) {

					// Define Objects
					Register WHC_POS {0x02, 0x01, 0}; // Received Active Energy Counter C

					// Get Measurement
					Register_Pointer_Set(WHC_POS, 0x00); // Measure Phase R

					// End Function
					return(1);
					
				}

			}

			// End Function
			return(_Result * __ISCALE__ * __VSCALE__);
			
		}
		float Frequency(void) {

			// Define Objects
			Register FREQ {0x01, 0x80, 16}; // Line Frequency

			// Declare Variable
			float _Result = 0;

			// Get Data
			_Result = Register_Pointer_Read(FREQ);

			// End Function
			return(_Result);

		}
		float Power_Factor(uint8_t _Phase) {

			// Control Input Parameters
			if (_Phase > 4) return(0);

			// Declare Variable
			float _Result = 0;

			// Phase Select
			if (_Phase == __Phase_R__) {

				// Define Objects
				Register PFA {0x01, 0x65, 22}; // Power Factor A

				// Get Measurement
				_Result = Register_Pointer_Read(PFA); // Measure Phase Average

			} else if (_Phase == __Phase_S__) {

				// Define Objects
				Register PFB {0x01, 0x68, 22}; // Power Factor B

				// Get Measurement
				_Result = Register_Pointer_Read(PFB); // Measure Phase Average

			} else if (_Phase == __Phase_T__) {

				// Define Objects
				Register PFC {0x01, 0x6B, 22}; // Power Factor C

				// Get Measurement
				_Result = Register_Pointer_Read(PFC); // Measure Phase Average

			} else if (_Phase == __Phase_Avg__) {

				// Define Objects
				Register PFT {0x01, 0x6E, 22}; // T otal Power Factor

				// Get Measurement
				_Result = Register_Pointer_Read(PFT); // Measure Phase Average

			}
			
			// End Function
			return(_Result);

		}
		float IC_Temperature(void) {

			// Define Objects
			Register TEMPC {0x01, 0x74, 10}; // Chip Temperature

			// Declare Variable
			float _Result = 0;

			// Get Data
			_Result = Register_Pointer_Read(TEMPC);

			// End Function
			return(_Result);

		}

		// Set Functions
		uint8_t Set_Harmonic(uint32_t _Harmonic) {

			// Define Objects
			Register HARM {0x00, 0x4B, 0}; // Harmonic Selector, default: 1 

			// Declare Variable
			uint32_t _Result = 0;

			// Decide Action
			if (_Harmonic == 0) {
				
				// Read Register
				_Result = Register_Pointer_Read(HARM);

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

		// Limit Functions
		uint8_t Control_Limits(void) {

			// Define Objects
			Register STATUS {0x00, 0x15, 0}; // Alarm and device status bits

			// Read Status Register
			uint32_t _Status = Register_Pointer_Read(STATUS);

			// Clear Status Register
			Register_Pointer_Set(STATUS, 0x0000);

			// Control UV
			if (bitRead(_Status, 13) or bitRead(_Status, 15) or bitRead(_Status, 17)) return(1);

			// Control HV
			if (bitRead(_Status, 14) or bitRead(_Status, 16) or bitRead(_Status, 18)) return(2);

			// Control IIMB
			if (bitRead(_Status, 2)) return(10);

			// Control VIMB
			if (bitRead(_Status, 3)) return(9);

			// Control UPF
			if (bitRead(_Status, 10) or bitRead(_Status, 11) or bitRead(_Status, 12)) return(8);

			// Control FMIN
			if (bitRead(_Status, 21)) return(5);

			// Control FMAX
			if (bitRead(_Status, 22)) return(6);

			// End Function
			return(0);

		}
		bool Set_Voltage_Limits(const float _VMin, const float _VMax) {

			// Define Objects
			Register VRMS_MIN {0x00, 0xB1, 23}; // Voltage lower alarm limit
			Register VRMS_MAX {0x00, 0xB4, 23}; // Voltage upper alarm limit

			// Convert Variables
			uint32_t _VMax_RAW = _FtoS(_VMax / __VSCALE__, 23);
			uint32_t _VMin_RAW = _FtoS(_VMin / __VSCALE__, 23);

			// Write Registers
			bool _Result_Min = Register_Pointer_Set(VRMS_MIN, _VMin_RAW);
			bool _Result_Max = Register_Pointer_Set(VRMS_MAX, _VMax_RAW);

			// End Function
			return(_Result_Min and _Result_Max);

		}
		float Read_Voltage_Min_Limit(void) {

			// Define Objects
			Register VRMS_MIN {0x00, 0xB1, 23};		// Voltage lower alarm limit

			// Declare Variable
			float _Result = 0;

			_Result = Register_Pointer_Read(VRMS_MIN) * __VSCALE__;

			// End Function
			return(_Result);

		}
		float Read_Voltage_Max_Limit(void) {

			// Define Objects
			Register VRMS_MAX {0x00, 0xB4, 23}; // Voltage upper alarm limit

			// Declare Variable
			float _Result = 0;

			_Result = Register_Pointer_Read(VRMS_MAX) * __VSCALE__;

			// End Function
			return(_Result);

		}
		bool Set_Frequency_Limits(const float _FQMin, const float _FQMax) {

			// Define Objects
			Register F_MIN {0x01, 0x83, 16}; // Frequency Alarm Lower Limit
			Register F_MAX {0x01, 0x86, 16}; // Frequency Alarm Upper Limit

			// Convert Variables
			uint32_t _FMax_RAW = _FtoS(_FQMax, F_MIN.Data_Type);
			uint32_t _FMin_RAW = _FtoS(_FQMin, F_MAX.Data_Type);

			// Write Registers
			bool _Result_Min = Register_Pointer_Set(F_MIN, _FMin_RAW);
			bool _Result_Max = Register_Pointer_Set(F_MAX, _FMax_RAW);

			// End Function
			return(_Result_Min and _Result_Max);

		}
		float Read_Frequency_Min_Limit(void) {

			// Define Objects
			Register F_MIN {0x01, 0x83, 16}; // Frequency Alarm Lower Limit

			// Declare Variable
			float _Result = 0;

			_Result = Register_Pointer_Read(F_MIN);

			// End Function
			return(_Result);

		}
		float Read_Frequency_Max_Limit(void) {

			// Define Objects
			Register F_MAX {0x01, 0x86, 16}; // Frequency Alarm Upper Limit

			// Declare Variable
			float _Result = 0;

			_Result = Register_Pointer_Read(F_MAX);

			// End Function
			return(_Result);

		}
		bool Set_Imbalance_Limits(const float _VImb, const float _IImb) {

			// Define Objects
			Register V_IMB_MAX {0x00, 0x81, 23}; // Voltage imbalance alarm limit
			Register I_IMB_MAX {0x00, 0x84, 23}; // Current imbalance alarm limit

			// Convert Variables
			uint32_t _VImb_RAW = _FtoS(_VImb, V_IMB_MAX.Data_Type);
			uint32_t _IImb_RAW = _FtoS(_IImb, I_IMB_MAX.Data_Type);

			// Write Registers
			bool _Result_Vimb = Register_Pointer_Set(V_IMB_MAX, _VImb_RAW);
			bool _Result_Iimb = Register_Pointer_Set(I_IMB_MAX, _IImb_RAW);

			// End Function
			return(_Result_Vimb and _Result_Iimb);

		}
		float Read_Voltage_Imbalance_Max_Limit(void) {

			// Define Objects
			Register V_IMB_MAX {0x00, 0x81, 23}; // Voltage imbalance alarm limit

			// Declare Variable
			float _Result = 0;

			_Result = Register_Pointer_Read(V_IMB_MAX);

			// End Function
			return(_Result);

		}
		float Read_Current_Imbalance_Max_Limit(void) {

			// Define Objects
			Register I_IMB_MAX {0x00, 0x84, 23}; // Current imbalance alarm limit

			// Declare Variable
			float _Result = 0;

			_Result = Register_Pointer_Read(I_IMB_MAX);

			// End Function
			return(_Result);

		}
		float Voltage_SAG_Limit(void) {

			// Define Objects
			Register VSAG_LIM {0x00, 0xB7, 23}; // RMS Voltage Sag threshold

			// Declare Variable
			float _Result = 0;

			_Result = Register_Pointer_Read(VSAG_LIM); // Measure Phase R
			
			// End Function
			return(_Result * __VSCALE__);

		}

		// Min Max Tracking Functions
		bool Set_Min_Max_Address(uint8_t _MM_ADDR, uint32_t _Mask) {

			// Define Result Variable
			bool _Result = false;

			// Decide Command
			if (_MM_ADDR == 1) {

				// Define Register
				Register MMADDR0 {0x01, 0xB9, 0}; // Min/Max Monitor address 1

				// Set Command
				_Result = Register_Pointer_Set(MMADDR0, _Mask);

			}
			if (_MM_ADDR == 2) {

				// Define Register
				Register MMADDR1 {0x01, 0xBC, 0}; // Min/Max Monitor address 2

				// Set Command
				_Result = Register_Pointer_Set(MMADDR1, _Mask);

			}
			if (_MM_ADDR == 3) {

				// Define Register
				Register MMADDR2 {0x01, 0xBF, 0}; // Min/Max Monitor address 3

				// Set Command
				_Result = Register_Pointer_Set(MMADDR2, _Mask);

			}
			if (_MM_ADDR == 4) {

				// Define Register
				Register MMADDR3 {0x01, 0xC2, 0}; // Min/Max Monitor address 4

				// Set Command
				_Result = Register_Pointer_Set(MMADDR3, _Mask);

			}
			if (_MM_ADDR == 5) {

				// Define Register
				Register MMADDR4 {0x01, 0xC5, 0}; // Min/Max Monitor address 5

				// Set Command
				_Result = Register_Pointer_Set(MMADDR4, _Mask);

			}
			if (_MM_ADDR == 6) {

				// Define Register
				Register MMADDR5 {0x01, 0xC8, 0}; // Min/Max Monitor address 6

				// Set Command
				_Result = Register_Pointer_Set(MMADDR5, _Mask);

			}
			if (_MM_ADDR == 7) {

				// Define Register
				Register MMADDR6 {0x01, 0xCB, 0}; // Min/Max Monitor address 7

				// Set Command
				_Result = Register_Pointer_Set(MMADDR6, _Mask);

			}
			if (_MM_ADDR == 8) {

				// Define Register
				Register MMADDR7 {0x01, 0xCE, 0}; // Min/Max Monitor address 8

				// Set Command
				_Result = Register_Pointer_Set(MMADDR7, _Mask);

			}

			// End Function
			return(_Result);

		}
		float Get_Min_Value(uint8_t _MM_ADDR) {

			// Define Objects
			Register MIN0 {0x01, 0x89, 23}; // Minimum Recorded Value 1
			Register MIN1 {0x01, 0x8C, 23}; // Minimum Recorded Value 2
			Register MIN2 {0x01, 0x8F, 23}; // Minimum Recorded Value 3
			Register MIN3 {0x01, 0x92, 0}; // Minimum Recorded Value 4
			Register MIN4 {0x01, 0x95, 0}; // Minimum Recorded Value 5
			Register MIN5 {0x01, 0x98, 0}; // Minimum Recorded Value 6
			Register MIN6 {0x01, 0x9B, 0}; // Minimum Recorded Value 7
			Register MIN7 {0x01, 0x9E, 0}; // Minimum Recorded Value 8

			// Declare Variable
			float _Result = 0;

			if (_MM_ADDR == 1) _Result = Register_Pointer_Read(MIN0) * __VSCALE__; // Measure Phase R
			if (_MM_ADDR == 2) _Result = Register_Pointer_Read(MIN1) * __VSCALE__; // Measure Phase R
			if (_MM_ADDR == 3) _Result = Register_Pointer_Read(MIN2) * __ISCALE__; // Measure Phase R
			if (_MM_ADDR == 4) _Result = Register_Pointer_Read(MIN3); // Measure Phase R
			if (_MM_ADDR == 5) _Result = Register_Pointer_Read(MIN4); // Measure Phase R
			if (_MM_ADDR == 6) _Result = Register_Pointer_Read(MIN5); // Measure Phase R
			if (_MM_ADDR == 7) _Result = Register_Pointer_Read(MIN6); // Measure Phase R
			if (_MM_ADDR == 8) _Result = Register_Pointer_Read(MIN7); // Measure Phase R

			// End Function
			return(_Result);

		}
		float Get_Max_Value(uint8_t _MM_ADDR) {

			// Define Objects
			Register MAX0 {0x01, 0xA1, 23}; // Maximum Recorded Value 1
			Register MAX1 {0x01, 0xA4, 23};	// Maximum Recorded Value 2
			Register MAX2 {0x01, 0xA7, 23};	// Maximum Recorded Value 3
			Register MAX3 {0x01, 0xAA, 0}; // Maximum Recorded Value 4
			Register MAX4 {0x01, 0xAD, 0}; // Maximum Recorded Value 5
			Register MAX5 {0x01, 0xB0, 0}; // Maximum Recorded Value 6
			Register MAX6 {0x01, 0xB3, 0}; // Maximum Recorded Value 7
			Register MAX7 {0x01, 0xB6, 0}; // Maximum Recorded Value 8

			// Declare Variable
			float _Result = 0;

			if (_MM_ADDR == 1) _Result = Register_Pointer_Read(MAX0) * __VSCALE__; // Measure Phase R
			if (_MM_ADDR == 2) _Result = Register_Pointer_Read(MAX1) * __VSCALE__; // Measure Phase R
			if (_MM_ADDR == 3) _Result = Register_Pointer_Read(MAX2) * __VSCALE__; // Measure Phase R
			if (_MM_ADDR == 4) _Result = Register_Pointer_Read(MAX3); // Measure Phase R
			if (_MM_ADDR == 5) _Result = Register_Pointer_Read(MAX4); // Measure Phase R
			if (_MM_ADDR == 6) _Result = Register_Pointer_Read(MAX5); // Measure Phase R
			if (_MM_ADDR == 7) _Result = Register_Pointer_Read(MAX6); // Measure Phase R
			if (_MM_ADDR == 8) _Result = Register_Pointer_Read(MAX7); // Measure Phase R

			// End Function
			return(_Result);

		}
		bool Clear_Min_Value(uint8_t _MM_ADDR) {

			// Define Objects
			Register MIN0 {0x01, 0x89, 0}; // Minimum Recorded Value 1
			Register MIN1 {0x01, 0x8C, 0}; // Minimum Recorded Value 2
			Register MIN2 {0x01, 0x8F, 0}; // Minimum Recorded Value 3
			Register MIN3 {0x01, 0x92, 0}; // Minimum Recorded Value 4
			Register MIN4 {0x01, 0x95, 0}; // Minimum Recorded Value 5
			Register MIN5 {0x01, 0x98, 0}; // Minimum Recorded Value 6
			Register MIN6 {0x01, 0x9B, 0}; // Minimum Recorded Value 7
			Register MIN7 {0x01, 0x9E, 0}; // Minimum Recorded Value 8

			if (_MM_ADDR == 1) Register_Pointer_Set(MIN0, 0x00);
			if (_MM_ADDR == 2) Register_Pointer_Set(MIN1, 0x00);
			if (_MM_ADDR == 3) Register_Pointer_Set(MIN2, 0x00);
			if (_MM_ADDR == 4) Register_Pointer_Set(MIN3, 0x00);
			if (_MM_ADDR == 5) Register_Pointer_Set(MIN4, 0x00);
			if (_MM_ADDR == 6) Register_Pointer_Set(MIN5, 0x00);
			if (_MM_ADDR == 7) Register_Pointer_Set(MIN6, 0x00);
			if (_MM_ADDR == 8) Register_Pointer_Set(MIN7, 0x00);

			// End Function
			return(true);

		}
		bool Clear_Max_Value(uint8_t _MM_ADDR) {

			// Define Objects
			Register MAX0 {0x01, 0xA1, 0}; // Maximum Recorded Value 1
			Register MAX1 {0x01, 0xA4, 0}; // Maximum Recorded Value 2
			Register MAX2 {0x01, 0xA7, 0}; // Maximum Recorded Value 3
			Register MAX3 {0x01, 0xAA, 0}; // Maximum Recorded Value 4
			Register MAX4 {0x01, 0xAD, 0}; // Maximum Recorded Value 5
			Register MAX5 {0x01, 0xB0, 0}; // Maximum Recorded Value 6
			Register MAX6 {0x01, 0xB3, 0}; // Maximum Recorded Value 7
			Register MAX7 {0x01, 0xB6, 0}; // Maximum Recorded Value 8

			if (_MM_ADDR == 1) Register_Pointer_Set(MAX0, 0x00);
			if (_MM_ADDR == 2) Register_Pointer_Set(MAX1, 0x00);
			if (_MM_ADDR == 3) Register_Pointer_Set(MAX2, 0x00);
			if (_MM_ADDR == 4) Register_Pointer_Set(MAX3, 0x00);
			if (_MM_ADDR == 5) Register_Pointer_Set(MAX4, 0x00);
			if (_MM_ADDR == 6) Register_Pointer_Set(MAX5, 0x00);
			if (_MM_ADDR == 7) Register_Pointer_Set(MAX6, 0x00);
			if (_MM_ADDR == 8) Register_Pointer_Set(MAX7, 0x00);

			// End Function
			return(true);

		}

		// TODO: Working on functions
		bool Alarm(void) {

			// Alarm Registers
			//Register STATUS_SET		{0x00, 0x1B, 0, false};		// Used to set/force alarm/status bits
			//Register MASK1			{0x00, 0x1E, 0, true};		// Alarm/status mask for AL1 output pin
			//Register MASK2			{0x00, 0x21, 0, true};		// Alarm/status mask for AL2 output pin
			//Register MASK3			{0x00, 0x24, 0, true};		// Alarm/status mask for AL3 output pin
			//Register MASK4			{0x00, 0x27, 0, true};		// Alarm/status mask for AL4 output pin
			//Register MASK5			{0x00, 0x2A, 0, true};		// Alarm/status mask for AL5 output pin
			//Register STICKY			{0x00, 0x2D, 0, true};		// Alarm/status bits to hold until cleared by host

			// End Function
			return(true);

		}
		bool DIO(void) {

			// DIO Registers
			//Register DIO_STATE		{0x00, 0x30, 0, false};		// State of DIO pins
			//Register DIO_DIR		{0x00, 0x33, 0, true};		// Direction of DIO pins. 1=Input ; 0=Output
			//Register DIO_POL		{0x00, 0x36, 0, true};		// Polarity of DIO pins. 1=Active High ; 0=Active Low

			// End Function
			return(true);

		}

};

#endif /* defined(__MAX78630__) */