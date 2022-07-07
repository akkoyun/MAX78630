#ifndef __MAX78630__
#define __MAX78630__

// Define Arduino Library
#ifndef __Arduino__
	#include <Arduino.h>
#endif

// Include Library Definitions
#include "Definitions.h"
#include "Variable.h"

class MAX78630 {

	private:

		/**
		 * @brief Serial Channel Variable
		 */
		Stream * Serial_Energy;

		/**
		 * @brief IC Register Address Structure
		 */
		struct Register {
			const uint8_t High_Address;
			const uint8_t Low_Address;
			const uint8_t Data_Type;
		};

		/**
		 * @brief Get Device Address Function.
		 * @version 01.00.00
		 * @return uint32_t Device Address
		 */
		uint32_t Get_Device_Address(void) {

			// Define Objects
			Register DEVADDR {0x00, 0x4E, 0};

			// Declare Variable
			uint32_t _Result = 0;

			// Decide Command
			_Result = this->Register_Pointer_Read(DEVADDR);
			
			// End Function
			return(_Result);

		}

		/**
		 * @brief Get System Status Function.
		 * @version 01.00.00
		 * @return uint32_t System Status
		 */
		uint32_t Get_System_Stat(void) {

			// Define Objects
			Register SYSSTAT {0x02, 0x43, 0};

			// Declare Variable
			uint32_t _Result = 0;

			// Decide Command
			_Result = this->Register_Pointer_Read(SYSSTAT);
			
			// End Function
			return(_Result);

		}

		/**
		 * @brief Get Device Firmware Function
		 * @version 01.00.00
		 * @return uint32_t Device Firmware.
		 */
		uint32_t Get_Firmware(void) {

			// Define Objects
			Register FW_VERSION {0x00, 0x03, 0};

			// Declare Variable
			uint32_t _Result = 0;

			// Decide Command
			_Result = this->Register_Pointer_Read(FW_VERSION);
			
			// End Function
			return(_Result);

		}

		/**
		 * @brief Get Device Frame Rate Function.
		 * @version 01.00.00
		 * @return uint32_t Frame Rate
		 */
		uint32_t Get_Frame(void) {

			// Define Objects
			Register FRAME {0x00, 0x12, 0}; // Low-rate sample counter

			// Declare Variable
			uint32_t _Result = 0;

			// Decide Command
			_Result = this->Register_Pointer_Read(FRAME);
			
			// End Function
			return(_Result);

		}

		/**
		 * @brief Get Sample Counter Function
		 * @version 01.00.00
		 * @return uint32_t Sample Counter
		 */
		uint32_t Get_Cycle(void) {

			// Define Objects
			Register CYCLE {0x00, 0x0F, 0}; // High-rate sample counter

			// Declare Variable
			uint32_t _Result = 0;

			// Decide Command
			_Result = this->Register_Pointer_Read(CYCLE);
			
			// End Function
			return(_Result);

		}

		/**
		 * @brief Get Divisor Value Function
		 * @version 01.00.00
		 * @return uint32_t Divisor Value
		 */
		uint32_t Get_Divisor(void) {

			// Define Objects
			Register DIVISOR {0x00, 0x0C, 0}; // Actual samples in previous accumulation interval

			// Declare Variable
			uint32_t _Result = 0;

			// Decide Command
			_Result = this->Register_Pointer_Read(DIVISOR);
			
			// End Function
			return(_Result);

		}

		/**
		 * @brief Get Samples per Accumulation Interval Function.
		 * @version 01.00.00
		 * @return uint32_t Sample per Accumulation Interval
		 */
		uint32_t Get_Samples(void) {

			// Define Objects
			Register SAMPLES {0x00, 0x09, 0}; // Minimum high-rate samples per accumulation interval

			// Declare Variable
			uint32_t _Result = 0;

			// Decide Command
			_Result = this->Register_Pointer_Read(SAMPLES);
			
			// End Function
			return(_Result);

		}

		/**
		 * @brief Get Communication Baud Function.
		 * @version 01.00.00
		 * @return uint32_t 
		 */
		uint32_t Get_Baud(void) {

			// Define Objects
			Register BAUD {0x00, 0x51, 0}; // Baud rate for UART interface

			// Read Register
			uint32_t _Baud = this->Register_Pointer_Read(BAUD);

			// End Function
			return(_Baud);

		}

		/**
		 * @brief Set Baud Function
		 * @version 01.00.00
		 * @param _Baud Communication Baud Rate
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_Baud(uint32_t _Baud) {

			// Define Objects
			Register BAUD {0x00, 0x51, 0}; // Baud rate for UART interface

			// Set Register
			bool _Response = this->Register_Pointer_Set(BAUD, _Baud);

			// End Function
			return(_Response);

		}

		/**
		 * @brief Set Voltage Scale Function
		 * @version 01.00.00
		 * @param _Voltage_Scale Voltage Scale
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_VScale(uint32_t _Voltage_Scale) {

			// Define Objects
			Register VSCALE {0x01, 0x44, 0};

			// Set Register
			bool _Response = this->Register_Pointer_Set(VSCALE, _Voltage_Scale);

			// End Function
			return(_Response);

		}

		/**
		 * @brief Set Current Scale Function
		 * @version 01.00.00
		 * @param _Current_Scale Current Scale
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_IScale(uint32_t _Current_Scale) {

			// Define Objects
			Register IFSCALE {0x01, 0x41, 0};

			// Set Register
			bool _Response = this->Register_Pointer_Set(IFSCALE, _Current_Scale);

			// End Function
			return(_Response);

		}

		/**
		 * @brief Set Voltage and Current Scale Batch Function.
		 * @version 01.00.00
		 * @param _VScale Voltage Scale
		 * @param _IScale Current Scale
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_Scale(uint32_t _VScale, uint32_t _IScale) {

			// VScale Set Command
			bool _Response_VScale = this->Set_VScale(_VScale);

			// IScale Set Command
			bool _Response_IScale = this->Set_IScale(_IScale);

			// End Function
			return(_Response_VScale and _Response_IScale);

		}

		/**
		 * @brief Voltage Calibration Function
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__
		 * @param _Gain Gain Value of Voltage
		 * @param _Offset OffSet Value of Voltage
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Voltage_Calibration(const uint8_t _Phase, const float _Gain, const float _Offset) {

			// Control Input Parameters
			if (_Phase > 3) return(false);

			// Phase R Calibration
			if (_Phase == __Phase_R__) {

				// Define Objects
				Register V1_GAIN {0x00, 0x5D, 21}; // Voltage Gain Calibration
				Register V1_OFFS {0x00, 0x6F, 23}; // Voltage Offset Calibration

				bool _Result_Gain = this->Register_Pointer_Set(V1_GAIN, _FtoS(_Gain, V1_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = this->Register_Pointer_Set(V1_OFFS, _FtoS(_Offset, V1_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// Phase S Calibration
			if (_Phase == __Phase_S__) {

				// Define Objects
				Register V2_GAIN {0x00, 0x60, 21}; // Voltage Gain Calibration
				Register V2_OFFS {0x00, 0x72, 23}; // Voltage Offset Calibration

				bool _Result_Gain = this->Register_Pointer_Set(V2_GAIN, _FtoS(_Gain, V2_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = this->Register_Pointer_Set(V2_OFFS, _FtoS(_Offset, V2_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// Phase T Calibration
			if (_Phase == __Phase_T__) {

				// Define Objects
				Register V3_GAIN {0x00, 0x63, 21}; // Voltage Gain Calibration
				Register V3_OFFS {0x00, 0x75, 23}; // Voltage Offset Calibration

				bool _Result_Gain = this->Register_Pointer_Set(V3_GAIN, _FtoS(_Gain, V3_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = this->Register_Pointer_Set(V3_OFFS, _FtoS(_Offset, V3_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// End Function
			return(false);

		}

		/**
		 * @brief Current Calibration Function
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__
		 * @param _Gain Gain Value of Current
		 * @param _Offset OffSet Value of Current
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Current_Calibration(const uint8_t _Phase, const float _Gain, const float _Offset) {

			// Control Input Parameters
			if (_Phase > 3) return(false);

			// Phase R Calibration
			if (_Phase == __Phase_R__) {

				// Define Objects
				Register I1_GAIN {0x00, 0x54, 21}; // Current Gain Calibration
				Register I1_OFFS {0x00, 0x66, 23}; // Current Offset Calibration

				bool _Result_Gain = this->Register_Pointer_Set(I1_GAIN, _FtoS(_Gain, I1_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = this->Register_Pointer_Set(I1_OFFS, _FtoS(_Offset, I1_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// Phase S Calibration
			if (_Phase == __Phase_S__) {

				// Define Objects
				Register I2_GAIN {0x00, 0x57, 21}; // Current Gain Calibration
				Register I2_OFFS {0x00, 0x69, 23}; // Current Offset Calibration

				bool _Result_Gain = this->Register_Pointer_Set(I2_GAIN, _FtoS(_Gain, I2_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = this->Register_Pointer_Set(I2_OFFS, _FtoS(_Offset, I2_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// Phase T Calibration
			if (_Phase == __Phase_T__) {

				// Define Objects
				Register I3_GAIN {0x00, 0x5A, 21}; // Current Gain Calibration
				Register I3_OFFS {0x00, 0x6C, 23}; // Current Offset Calibration

				bool _Result_Gain = this->Register_Pointer_Set(I3_GAIN, _FtoS(_Gain, I3_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = this->Register_Pointer_Set(I3_OFFS, _FtoS(_Offset, I3_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// End Function
			return(false);

		}

		/**
		 * @brief Temperature Calibration Function
		 * @version 01.00.00
		 * @param _Gain Gain Value of Temperature
		 * @param _Offset OffSet Value of Temperature
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Temperature_Calibration(const float _Gain, const float _Offset) {

			// Define Objects
			Register T_GAIN {0x00, 0x78, 0}; // Temperature Slope Calibration
			Register T_OFFS {0x00, 0x7B, 0}; // Temperature Offset Calibration

			bool _Result_Gain = this->Register_Pointer_Set(T_GAIN, _FtoS(_Gain, T_GAIN.Data_Type)); // Write Gain
			bool _Result_Offset = this->Register_Pointer_Set(T_OFFS, _FtoS(_Offset, T_GAIN.Data_Type)); // Write Offset

			// End Function
			return(_Result_Gain and _Result_Offset);

		}

		/**
		 * @brief Voltage High Pass Filter Coefficient Function.
		 * @version 01.00.00
		 * @param _COEF Coefficient Value
		 * @return float Coefficient Value
		 * TODO: Not Tested.
		 */
		float Voltage_HPF_COEF(float _COEF) {

			// Define Objects
			Register HPF_COEF_V {0x00, 0x3F, 23}; // Voltage input HPF coefficient

			// Declare Variable
			float _Result = 0;

			// Decide Workflow
			if (_COEF == -999) {

				// Read Register
				_Result = this->Register_Pointer_Read(HPF_COEF_V); // Measure Phase R

			} else {

				// Set Register
				_Result = this->Register_Pointer_Set(HPF_COEF_V, _FtoS(_COEF, 23)); // Measure Phase R

			}
			
			// End Function
			return(_Result);

		}

		/**
		 * @brief Current High Pass Filter Coefficient Function.
		 * @version 01.00.00
		 * @param _COEF Coefficient Value
		 * @return float Coefficient Value
		 * TODO: Not Tested.
		 */
		float Current_HPF_COEF(float _COEF) {

			// Define Objects
			Register HPF_COEF_I {0x00, 0x3C, 23}; // Current input HPF coefficient

			// Declare Variable
			float _Result = 0;

			// Decide Workflow
			if (_COEF == -999) {

				// Read Register
				_Result = this->Register_Pointer_Read(HPF_COEF_I); // Measure Phase R

			} else {

				// Set Register
				_Result = this->Register_Pointer_Set(HPF_COEF_I, _FtoS(_COEF, 23)); // Measure Phase R

			}
			
			// End Function
			return(_Result);

		}

		/**
		 * @brief Current RMS Offset Set Function.
		 * @version 01.00.00
		 * @param Phase 
		 * @param _Offset 
		 * @return float 
		 * TODO: Not Tested
		 */
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
				if (Phase == 'R') _Result = this->Register_Pointer_Read(IARMS_OFF); // Measure Phase R
				if (Phase == 'S') _Result = this->Register_Pointer_Read(IBRMS_OFF); // Measure Phase S
				if (Phase == 'T') _Result = this->Register_Pointer_Read(ICRMS_OFF); // Measure Phase T

			} else {

				// Set Register
				if (Phase == 'R') _Result = this->Register_Pointer_Set(IARMS_OFF, _FtoS(_Offset, 23)); // Measure Phase R
				if (Phase == 'S') _Result = this->Register_Pointer_Set(IBRMS_OFF, _FtoS(_Offset, 23)); // Measure Phase S
				if (Phase == 'T') _Result = this->Register_Pointer_Set(ICRMS_OFF, _FtoS(_Offset, 23)); // Measure Phase T

			}

			// End Function
			return(_Result);

		}

		/**
		 * @brief Active Power Offset Set Function.
		 * @version 01.00.00
		 * @param Phase 
		 * @param _Offset 
		 * @return float 
		 * TODO: Not Tested
		 */
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
				if (Phase == 'R') _Result = this->Register_Pointer_Read(PA_OFFS); // Measure Phase R
				if (Phase == 'S') _Result = this->Register_Pointer_Read(PB_OFFS); // Measure Phase S
				if (Phase == 'T') _Result = this->Register_Pointer_Read(PC_OFFS); // Measure Phase T

			} else {

				// Set Register
				if (Phase == 'R') _Result = this->Register_Pointer_Set(PA_OFFS, _FtoS(_Offset, 23)); // Measure Phase R
				if (Phase == 'S') _Result = this->Register_Pointer_Set(PB_OFFS, _FtoS(_Offset, 23)); // Measure Phase S
				if (Phase == 'T') _Result = this->Register_Pointer_Set(PC_OFFS, _FtoS(_Offset, 23)); // Measure Phase T

			}
			
			// End Function
			return(_Result);

		}

		/**
		 * @brief ReActive Power Offset Set Function.
		 * @version 01.00.00
		 * @param Phase 
		 * @param _Offset 
		 * @return float 
		 * TODO: Not Tested
		 */
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
				if (Phase == 'R') _Result = this->Register_Pointer_Read(QA_OFFS); // Measure Phase R
				if (Phase == 'S') _Result = this->Register_Pointer_Read(QB_OFFS); // Measure Phase S
				if (Phase == 'T') _Result = this->Register_Pointer_Read(QC_OFFS); // Measure Phase T

			} else {

				// Set Register
				if (Phase == 'R') _Result = this->Register_Pointer_Set(QA_OFFS, _FtoS(_Offset, 23)); // Measure Phase R
				if (Phase == 'S') _Result = this->Register_Pointer_Set(QB_OFFS, _FtoS(_Offset, 23)); // Measure Phase S
				if (Phase == 'T') _Result = this->Register_Pointer_Set(QC_OFFS, _FtoS(_Offset, 23)); // Measure Phase T

			}

			// End Function
			return(_Result);

		}

		/**
		 * @brief Bucket Size Set Function.
		 * @version 01.00.00
		 * @param _Bucket_H Bucket High Word
		 * @param _Bucket_L Bucket Low Word
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Write_Bucket(uint32_t _Bucket_H, uint32_t _Bucket_L) {

			// Define Objects
			Register BUCKET_LOW		{0x01, 0xD1, 0}; // Energy Bucket Size – Low word
			Register BUCKET_HIGH	{0x01, 0xD4, 0}; // Energy Bucket Size – High word

			// Set Command
			bool _Result_LOW = this->Register_Pointer_Set(BUCKET_LOW, _Bucket_L);
			bool _Result_HIGH = this->Register_Pointer_Set(BUCKET_HIGH, _Bucket_H);

			// Handle Response
			if (_Result_LOW and _Result_HIGH) return(false);

			// End Function
			return(true);

		}

		/**
		 * @brief Clear Serial Buffer Function
		 * @version 01.00.00
		 */
		void Clear_Buffer(void) {

			// Clear UART Buffer
			Serial_Energy->flush(); 
			
			while(Serial_Energy->available() > 0) {
				
				Serial_Energy->read(); 
				delay(5);

			}

		}

		/**
		 * @brief Register Read Function.
		 * @version 01.00.00
		 * @param _Command Register Address
		 * @return double Register Value
		 */
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

		/**
		 * @brief Write Register Function
		 * @version 01.00.00
		 * @param _Command Register Address
		 * @param _Data Register Value
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Register_Pointer_Set(Register _Command, uint32_t _Data) {

			// Clear Buffer
			this->Clear_Buffer();

			// Convert Parameters
			char _Parameter1 = _Data;
			char _Parameter2 = _Data>>8;
			char _Parameter3 = _Data>>16;

			// Calculate CheckSum
			uint8_t ChkS = 0x100 - ((0xAA + 0x0A + 0xA3 + _Command.Low_Address + _Command.High_Address + 0xD3 + _Parameter1 + _Parameter2 + _Parameter3) % 256); // Calculate checksum

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
			uint8_t Response;

			// Read Serial Char
			Response = Serial_Energy->read();

			// End Function
			if (Response == 0xAD) return(true);

			// End Function
			return(false);
			
		}

		/**
		 * @brief Data Converter Function
		 * @version 01.00.00
		 * @param _Variable Variable A
		 * @param _Data_Type Data Type
		 * @return uint32_t Variable B
		 */
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

		/**
		 * @brief Electrical Measurement Variable Structure
		 */
		struct Measurement_Structure {

			/**
			 * @brief Voltage Structure
			 */
			struct Voltage_Structure {

				/**
				 * @brief RMS Voltage R
				 */
				float RMS_R;

				/**
				 * @brief RMS Voltage S
				 */
				float RMS_S;

				/**
				 * @brief RMS Voltage T
				 */
				float RMS_T;

			} Voltage;

			/**
			 * @brief Current Structure
			 */
			struct Current_Structure {

				/**
				 * @brief RMS Current R
				 */
				float RMS_R;

				/**
				 * @brief RMS Current S
				 */
				float RMS_S;

				/**
				 * @brief RMS Current T
				 */
				float RMS_T;

			} Current;

			/**
			 * @brief Line Frequency
			 */
			float Frequency;

			/**
			 * @brief PowerFactor Structure
			 */
			struct PowerFactor_Structure {

				/**
				 * @brief PowerFactor R
				 */
				float Phase_R;

				/**
				 * @brief PowerFactor S
				 */
				float Phase_S;

				/**
				 * @brief PowerFactor T
				 */
				float Phase_T;

				/**
				 * @brief PowerFactor A
				 */
				float Average;

			} PowerFactor;

			/**
			 * @brief Energy Structure
			 */
			struct Energy_Structure {

				/**
				 * @brief Active
				 */
				uint16_t Active;

				/**
				 * @brief ReActive
				 */
				uint16_t ReActive;

				/**
				 * @brief Active Cumulative
				 */
				uint32_t Active_Cumulative;

				/**
				 * @brief ReActive Cumulative
				 */
				uint32_t ReActive_Cumulative;

			} Energy;

		} Measurement;

		/**
		 * @brief Begin and Set Start Parameters.
		 * @version 01.00.00
		 * @param _Serial Serial Channel
		 */
		void Begin(Stream & _Serial) {

			//Set serial port
			Serial_Energy = & _Serial;

			// Clear Serial Buffer
			this->Clear_Buffer();

			// Select IC Command
			Serial_Energy->write(0xAA);			// Header (0xAA)
			Serial_Energy->write(0x04);			// Total Sended Byte (0x04)
			Serial_Energy->write(0xC4);			// Setting Command (0xC4)
			Serial_Energy->write(0x8E);			// CheckSum (0x8E)

			// Command Delay
			delay(10);

			// Set Scale Registers
			this->Set_Scale(__VSCALE__, __ISCALE__);

			// Set Default Limits
			this->Set_Default_Limits();

			// Bucket Set Command
			this->Write_Bucket(Bucket_High, Bucket_Low);

			// Set Min and Max Address
			this->Set_Default_Min_Max_Address();

		}

		/**
		 * @brief Measure Parameters Batch
		 * @version 01.00.00
		 */
		void Measure(void) {

			/**
			 * @brief R Phase RMS Measurement
			 */
			#ifdef Measurement_Voltage_R 

				// Clear Variable
				Measurement.Voltage.RMS_R = 0;

				// Get Measurement
				Measurement.Voltage.RMS_R = this->Voltage(__Phase_R__, __RMS__);

				// Command Delay
				delay(10);

			#endif

			/**
			 * @brief S Phase RMS Measurement
			 */
			#ifdef Measurement_Voltage_S

				// Clear Variable
				Measurement.Voltage.RMS_S = 0;

				// Get Measurement
				Measurement.Voltage.RMS_S = this->Voltage(__Phase_S__, __RMS__);

				// Command Delay
				delay(10);

			#endif

			/**
			 * @brief T Phase RMS Measurement
			 */
			#ifdef Measurement_Voltage_T

				// Clear Variable
				Measurement.Voltage.RMS_T = 0;

				// Get Measurement
				Measurement.Voltage.RMS_T = this->Voltage(__Phase_T__, __RMS__);

				// Command Delay
				delay(10);

			#endif

			/**
			 * @brief R Phase RMS Measurement
			 */
			#ifdef Measurement_Current_R

				// Clear Variable
				Measurement.Current.RMS_R = 0;

				// Get Measurement
				Measurement.Current.RMS_R = this->Current(__Phase_R__, __RMS__);

				// Command Delay
				delay(10);

			#endif

			/**
			 * @brief S Phase RMS Measurement
			 */
			#ifdef Measurement_Current_S

				// Clear Variable
				Measurement.Current.RMS_S = 0;

				// Get Measurement
				Measurement.Current.RMS_S = this->Current(__Phase_S__, __RMS__);

				// Command Delay
				delay(10);

			#endif

			/**
			 * @brief T Phase RMS Measurement
			 */
			#ifdef Measurement_Current_T

				// Clear Variable
				Measurement.Current.RMS_T = 0;

				// Get Measurement
				Measurement.Current.RMS_T = this->Current(__Phase_T__, __RMS__);

				// Command Delay
				delay(10);

			#endif

			/**
			 * @brief Frequency Measurement
			 */
			#ifdef Measurement_Frequency

				// Clear Variable
				Measurement.Frequency = 0;

				// Get Measurement
				Measurement.Frequency = this->Frequency();

				// Command Delay
				delay(10);

			#endif

			/**
			 * @brief R Phase RMS Measurement
			 */
			#ifdef Measurement_PowerFactor_R

				// Clear Variable
				Measurement.PowerFactor.Phase_R = 0;

				// Get Measurement
				Measurement.PowerFactor.Phase_R = this->Power_Factor(__Phase_R__);

				// Command Delay
				delay(10);

			#endif

			/**
			 * @brief S Phase RMS Measurement
			 */
			#ifdef Measurement_PowerFactor_S

				// Clear Variable
				Measurement.PowerFactor.Phase_S = 0;

				// Get Measurement
				Measurement.PowerFactor.Phase_S = this->Power_Factor(__Phase_S__);

				// Command Delay
				delay(10);

			#endif

			/**
			 * @brief T Phase RMS Measurement
			 */
			#ifdef Measurement_PowerFactor_T

				// Clear Variable
				Measurement.PowerFactor.Phase_T = 0;

				// Get Measurement
				Measurement.PowerFactor.Phase_T = this->Power_Factor(__Phase_T__);

				// Command Delay
				delay(10);

			#endif

			/**
			 * @brief Average Measurement
			 */
			#ifdef Measurement_PowerFactor_A

				// Clear Variable
				Measurement.PowerFactor.Average = 0;

				// Get Measurement
				Measurement.PowerFactor.Average = this->Power_Factor(__Phase_Avg__);

				// Command Delay
				delay(10);

			#endif

			/**
			 * @brief Active Energy
			 */
			#ifdef Measurement_Energy_Active

				// Clear Variable
				Measurement.Energy.Active = 0;

				// Calculate Total Energy Consumption
				Measurement.Energy.Active = this->Energy(__Phase_R__, __Active_Received__) + this->Energy(__Phase_S__, __Active_Received__) + this->Energy(__Phase_T__, __Active_Received__);

				// Calculate Total Cumulative Energy Consumption
				Measurement.Energy.Active_Cumulative += Measurement.Energy.Active;

				delay(10);

			#endif

			/**
			 * @brief ReActive Energy
			 */
			#ifdef Measurement_Energy_ReActive

				// Clear Variable
				Measurement.Energy.ReActive = 0;

				// Calculate Total Energy Consumption
				Measurement.Energy.ReActive = this->Energy(__Phase_R__, __ReActive_Received__) + this->Energy(__Phase_S__, __ReActive_Received__) + this->Energy(__Phase_T__, __ReActive_Received__);

				// Calculate Total Cumulative Energy Consumption
				Measurement.Energy.ReActive_Cumulative += Measurement.Energy.ReActive;

				delay(10);

			#endif

			// Clear Energy Consumption
			this->Energy(__Phase_R__, __Energy_Reset__);
			this->Energy(__Phase_S__, __Energy_Reset__);
			this->Energy(__Phase_T__, __Energy_Reset__);

			// Command Delay
			delay(10);

		}
		
		/**
		 * @brief Voltage Measurement Function.
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__, __Phase_Avg__
		 * @param _Type __RMS__, __Instant__, __Fundamental__, __Harmonic__
		 * @return float Voltage Measurement [Volt]
		 */
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
					_Result = this->Register_Pointer_Read(VA_RMS); // Measure Phase R

				} else if (_Type == __Instant__) {

					// Define Objects
					Register VA {0x00, 0x87, 23}; // Instantaneous Voltage A

					// Get Measurement
					_Result = this->Register_Pointer_Read(VA); // Measure Phase R

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register VFUND_A {0x00, 0x9C, 23}; // Fundamental Voltage A

					// Get Measurement
					_Result = this->Register_Pointer_Read(VFUND_A); // Measure Phase R

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register VHARM_A {0x00, 0xA5, 23}; // Harmonic Voltage A

					// Get Measurement
					_Result = this->Register_Pointer_Read(VHARM_A); // Measure Phase R

				}

			} else if (_Phase == __Phase_S__) {

				// Measurement Type Select
				if (_Type == __RMS__) {

					// Define Objects
					Register VB_RMS {0x00, 0x93, 23}; // RMS Voltage B

					// Get Measurement
					_Result = this->Register_Pointer_Read(VB_RMS); // Measure Phase S

				} else if (_Type == __Instant__) {

					// Define Objects
					Register VB {0x00, 0x8A, 23}; // Instantaneous Voltage B

					// Get Measurement
					_Result = this->Register_Pointer_Read(VB); // Measure Phase S

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register VFUND_B {0x00, 0x9F, 23}; // Fundamental Voltage B

					// Get Measurement
					_Result = this->Register_Pointer_Read(VFUND_B); // Measure Phase S

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register VHARM_B {0x00, 0xA8, 23}; // Harmonic Voltage B

					// Get Measurement
					_Result = this->Register_Pointer_Read(VHARM_B); // Measure Phase S

				}

			} else if (_Phase == __Phase_T__) {

				// Measurement Type Select
				if (_Type == __RMS__) {

					// Define Objects
					Register VC_RMS {0x00, 0x96, 23}; // RMS Voltage C

					// Get Measurement
					_Result = this->Register_Pointer_Read(VC_RMS); // Measure Phase R

				} else if (_Type == __Instant__) {

					// Define Objects
					Register VC {0x00, 0x8D, 23}; // Instantaneous Voltage C

					// Get Measurement
					_Result = this->Register_Pointer_Read(VC); // Measure Phase R

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register VFUND_C {0x00, 0xA2, 23}; // Fundamental Voltage C

					// Get Measurement
					_Result = this->Register_Pointer_Read(VFUND_C); // Measure Phase R

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register VHARM_C {0x00, 0xAB, 23}; // Harmonic Voltage C

					// Get Measurement
					_Result = this->Register_Pointer_Read(VHARM_C); // Measure Phase R

				}

			} else if (_Phase == __Phase_Avg__) {

				// Measurement Type Select
				if (_Type == __RMS__) {

					// Define Objects
					Register VT_RMS {0x00, 0x99, 23}; // RMS Voltage average

					// Get Measurement
					_Result = this->Register_Pointer_Read(VT_RMS); // Measure Phase Average

				}

			}

			// End Function
			return(_Result * __VSCALE__);

		}

		/**
		 * @brief Frequency Measurement Function.
		 * @version 01.00.00
		 * @return float Frequency Measurement [Hz]
		 */
		float Frequency(void) {

			// Define Objects
			Register FREQ {0x01, 0x80, 16}; // Line Frequency

			// Declare Variable
			float _Result = 0;

			// Get Data
			_Result = this->Register_Pointer_Read(FREQ);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Current Measurement Function.
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__, __Phase_Avg__
		 * @param _Type __RMS__, __Instant__, __Fundamental__, __Harmonic__, __Peak__, __Phase_Compensation__
		 * @return float Current Measurement [Amper]
		 */
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
					_Result = this->Register_Pointer_Read(IA_RMS); // Measure Phase R

				} else if (_Type == __Instant__) {

					// Define Objects
					Register IA	{0x00, 0xBA, 23}; // Instantaneous Current A

					// Get Measurement
					_Result = this->Register_Pointer_Read(IA); // Measure Phase R

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register IFUND_A {0x00, 0xE1, 23}; // Fundamental Current A

					// Get Measurement
					_Result = this->Register_Pointer_Read(IFUND_A); // Measure Phase R

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register IHARM_A {0x00, 0xEA, 23}; // Harmonic Current A

					// Get Measurement
					_Result = this->Register_Pointer_Read(IHARM_A); // Measure Phase R

				} else if (_Type == __Peak__) {

					// Define Objects
					Register IA_PEAK {0x00, 0xCC, 23}; // Peak Current A

					// Get Measurement
					_Result = this->Register_Pointer_Read(IA_PEAK); // Measure Phase R

				} else if (_Type == __Phase_Compensation__) {

					// Define Objects
					Register PHASECOMP1 {0x00, 0x42, 21}; // Phase compensation A

					// Get Measurement
					_Result = this->Register_Pointer_Read(PHASECOMP1); // Measure Phase R

				}

			} else if (_Phase == __Phase_S__) {

				// Measurement Type Select
				if (_Type == __RMS__) {

					// Define Objects
					Register IB_RMS {0x00, 0xD8, 23}; // RMS Current B

					// Get Measurement
					_Result = this->Register_Pointer_Read(IB_RMS); // Measure Phase R

				} else if (_Type == __Instant__) {

					// Define Objects
					Register IB	{0x00, 0xBD, 23}; // Instantaneous Current B

					// Get Measurement
					_Result = this->Register_Pointer_Read(IB); // Measure Phase R

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register IFUND_B {0x00, 0xE4, 23}; // Fundamental Current B

					// Get Measurement
					_Result = this->Register_Pointer_Read(IFUND_B); // Measure Phase R

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register IHARM_B {0x00, 0xED, 23}; // Harmonic Current B

					// Get Measurement
					_Result = this->Register_Pointer_Read(IHARM_B); // Measure Phase R

				} else if (_Type == __Peak__) {

					// Define Objects
					Register IB_PEAK {0x00, 0xCF, 23}; // Peak Current B

					// Get Measurement
					_Result = this->Register_Pointer_Read(IB_PEAK); // Measure Phase R

				} else if (_Type == __Phase_Compensation__) {

					// Define Objects
					Register PHASECOMP2 {0x00, 0x45, 21}; // Phase compensation B

					// Get Measurement
					_Result = this->Register_Pointer_Read(PHASECOMP2); // Measure Phase R

				}

			} else if (_Phase == __Phase_T__) {

				// Measurement Type Select
				if (_Type == __RMS__) {

					// Define Objects
					Register IC_RMS {0x00, 0xDB, 23}; // RMS Current C

					// Get Measurement
					_Result = this->Register_Pointer_Read(IC_RMS); // Measure Phase R

				} else if (_Type == __Instant__) {

					// Define Objects
					Register IC	{0x00, 0xC0, 23}; // Instantaneous Current C

					// Get Measurement
					_Result = this->Register_Pointer_Read(IC); // Measure Phase R

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register IFUND_C {0x00, 0xE7, 23}; // Fundamental Current C

					// Get Measurement
					_Result = this->Register_Pointer_Read(IFUND_C); // Measure Phase R

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register IHARM_C {0x00, 0xF0, 23}; // Harmonic Current C

					// Get Measurement
					_Result = this->Register_Pointer_Read(IHARM_C); // Measure Phase R

				} else if (_Type == __Peak__) {

					// Define Objects
					Register IC_PEAK {0x00, 0xD2, 23}; // Peak Current C

					// Get Measurement
					_Result = this->Register_Pointer_Read(IC_PEAK); // Measure Phase R

				}else if (_Type == __Phase_Compensation__) {

					// Define Objects
					Register PHASECOMP3 {0x00, 0x48, 21}; // Phase compensation C

					// Get Measurement
					_Result = this->Register_Pointer_Read(PHASECOMP3); // Measure Phase R

				}

			} else if (_Phase == __Phase_Avg__) {

				// Measurement Type Select
				if (_Type == __RMS__) {

					// Define Objects
					Register IT_RMS {0x00, 0xDE, 23}; // RMS Current average

					// Get Measurement
					_Result = this->Register_Pointer_Read(IT_RMS); // Measure Phase Average

				}

			}

			// End Function
			return(_Result * __ISCALE__);

		}

		/**
		 * @brief Power Measurement Function.
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__, __Phase_Avg__
		 * @param _Type __Active__, __ReActive__, __Apparent__, __Fundamental__, __Harmonic__, __Fund_ReActive__, __Harm_ReActive__, __Fund_VA__
		 * @return float Power Measurement [Watt]
		 */
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
					_Result = this->Register_Pointer_Read(WATT_A); // Measure Phase R

				} else if (_Type == __ReActive__) {

					// Define Objects
					Register VAR_A {0x01, 0x26, 23}; // Reactive Power A

					// Get Measurement
					_Result = this->Register_Pointer_Read(VAR_A); // Measure Phase R

				} else if (_Type == __Apparent__) {

					// Define Objects
					Register VA_A {0x01, 0x2F, 23}; // Apparent Power A

					// Get Measurement
					_Result = this->Register_Pointer_Read(VA_A); // Measure Phase R

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register PFUND_A {0x01, 0x4A, 23}; // Fundamental Power A

					// Get Measurement
					_Result = this->Register_Pointer_Read(PFUND_A); // Measure Phase R

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register PHARM_A {0x01, 0x53, 23}; // Harmonic Power A

					// Get Measurement
					_Result = this->Register_Pointer_Read(PHARM_A); // Measure Phase R

				} else if (_Type == __Fund_ReActive__) {

					// Define Objects
					Register QFUND_A {0x00, 0xF9, 23}; // Fundamental Reactive Power A

					// Get Measurement
					_Result = this->Register_Pointer_Read(QFUND_A); // Measure Phase R

				} else if (_Type == __Harm_ReActive__) {

					// Define Objects
					Register QHARM_A {0x01, 0x02, 23}; // Harmonic Reactive Power A

					// Get Measurement
					_Result = this->Register_Pointer_Read(QHARM_A); // Measure Phase R

				} else if (_Type == __Fund_VA__) {

					// Define Objects
					Register VAFUNDA {0x01, 0x5C, 23}; // Fundamental Volt Amperes A

					// Get Measurement
					_Result = this->Register_Pointer_Read(VAFUNDA); // Measure Phase R

				}

			} else if (_Phase == __Phase_S__) {

				// Measurement Type Select
				if (_Type == __Active__) {

					// Define Objects
					Register WATT_B	{0x01, 0x20, 23}; // Active Power B

					// Get Measurement
					_Result = this->Register_Pointer_Read(WATT_B); // Measure Phase R

				} else if (_Type == __ReActive__) {

					// Define Objects
					Register VAR_B {0x01, 0x29, 23}; // Reactive Power B

					// Get Measurement
					_Result = this->Register_Pointer_Read(VAR_B); // Measure Phase R

				} else if (_Type == __Apparent__) {

					// Define Objects
					Register VA_B {0x01, 0x32, 23}; // Apparent Power B

					// Get Measurement
					_Result = this->Register_Pointer_Read(VA_B); // Measure Phase R

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register PFUND_B {0x01, 0x4D, 23}; // Fundamental Power B

					// Get Measurement
					_Result = this->Register_Pointer_Read(PFUND_B); // Measure Phase R

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register PHARM_B {0x01, 0x56, 23}; // Harmonic Power B

					// Get Measurement
					_Result = this->Register_Pointer_Read(PHARM_B); // Measure Phase R

				} else if (_Type == __Fund_ReActive__) {

					// Define Objects
					Register QFUND_B {0x00, 0xFC, 23}; // Fundamental Reactive Power B

					// Get Measurement
					_Result = this->Register_Pointer_Read(QFUND_B); // Measure Phase R

				} else if (_Type == __Harm_ReActive__) {

					// Define Objects
					Register QHARM_B {0x01, 0x05, 23}; // Harmonic Reactive Power B

					// Get Measurement
					_Result = this->Register_Pointer_Read(QHARM_B); // Measure Phase R

				} else if (_Type == __Fund_VA__) {

					// Define Objects
					Register VAFUNDB {0x01, 0x5F, 23}; // Fundamental Volt Amperes B

					// Get Measurement
					_Result = this->Register_Pointer_Read(VAFUNDB); // Measure Phase R

				}

			} else if (_Phase == __Phase_T__) {

				// Measurement Type Select
				if (_Type == __Active__) {

					// Define Objects
					Register WATT_C	{0x01, 0x23, 23}; // Active Power C

					// Get Measurement
					_Result = this->Register_Pointer_Read(WATT_C); // Measure Phase R

				} else if (_Type == __ReActive__) {

					// Define Objects
					Register VAR_C {0x01, 0x2C, 23}; // Reactive Power C

					// Get Measurement
					_Result = this->Register_Pointer_Read(VAR_C); // Measure Phase R

				} else if (_Type == __Apparent__) {

					// Define Objects
					Register VA_C {0x01, 0x35, 23}; // Apparent Power C

					// Get Measurement
					_Result = this->Register_Pointer_Read(VA_C); // Measure Phase R

				} else if (_Type == __Fundamental__) {

					// Define Objects
					Register PFUND_C {0x01, 0x50, 23}; // Fundamental Power C

					// Get Measurement
					_Result = this->Register_Pointer_Read(PFUND_C); // Measure Phase R

				} else if (_Type == __Harmonic__) {

					// Define Objects
					Register PHARM_C {0x01, 0x59, 23}; // Harmonic Power C

					// Get Measurement
					_Result = this->Register_Pointer_Read(PHARM_C); // Measure Phase R

				} else if (_Type == __Fund_ReActive__) {

					// Define Objects
					Register QFUND_C {0x00, 0xFF, 23}; // Fundamental Reactive Power C

					// Get Measurement
					_Result = this->Register_Pointer_Read(QFUND_C); // Measure Phase R

				} else if (_Type == __Harm_ReActive__) {

					// Define Objects
					Register QHARM_C {0x01, 0x08, 23}; // Harmonic Reactive Power C

					// Get Measurement
					_Result = this->Register_Pointer_Read(QHARM_C); // Measure Phase R

				} else if (_Type == __Fund_VA__) {

					// Define Objects
					Register VAFUNDC {0x01, 0x62, 23}; // Fundamental Volt Amperes C

					// Get Measurement
					_Result = this->Register_Pointer_Read(VAFUNDC); // Measure Phase R

				}

			} else if (_Phase == __Phase_Avg__) {

				// Measurement Type Select
				if (_Type == __Active__) {

					// Define Objects
					Register WATT_T	{0x01, 0x38, 23}; // Active Power average

					// Get Measurement
					_Result = this->Register_Pointer_Read(WATT_T); // Measure Phase R

				} else if (_Type == __ReActive__) {

					// Define Objects
					Register VAR_T {0x01, 0x3B, 23}; // Reactive Power average

					// Get Measurement
					_Result = this->Register_Pointer_Read(VAR_T); // Measure Phase R

				} else if (_Type == __Apparent__) {

					// Define Objects
					Register VA_T {0x01, 0x3E, 23}; // Apparent Power average

					// Get Measurement
					_Result = this->Register_Pointer_Read(VA_T); // Measure Phase R

				}

			}

			// End Function
			return(_Result * __ISCALE__ * __VSCALE__);

		}

		/**
		 * @brief Energy Measurement Function.
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__
		 * @param _Type __Active_Received__, __Active_Delivered__, __ReActive_Received__, __ReActive_Delivered__, __Energy_Reset__
		 * @return float Energy Measurement [Watt/Hour]
		 */
		float Energy(const uint8_t _Phase, const uint8_t _Type) {

			// Control Input Parameters
			if (_Phase > 3) return(0);

			// Declare Variable
			float _Result = 0;

			// Phase Select
			if (_Phase == __Phase_R__) {

				// Measurement Type Select
				if (_Type == __Active_Received__) {

					// Define Objects
					Register WHA_POS {0x01, 0xDD, 0}; // Received Active Energy Counter A

					// Get Measurement
					_Result = this->Register_Pointer_Read(WHA_POS); // Measure Phase R

				} else if (_Type == __Active_Delivered__) {

					// Define Objects
					Register WHA_NEG {0x01, 0xE6, 0}; // Delivered Active Energy Counter A

					// Get Measurement
					_Result = this->Register_Pointer_Read(WHA_NEG); // Measure Phase R

				} else if (_Type == __ReActive_Received__) {

					// Define Objects
					Register VARHA_POS {0x02, 0x13, 0}; // Reactive Energy Leading Counter A

					// Get Measurement
					_Result = this->Register_Pointer_Read(VARHA_POS); // Measure Phase R

				} else if (_Type == __ReActive_Delivered__) {

					// Define Objects
					Register VARHA_NEG {0x02, 0x1C, 0}; // Reactive Energy Lagging Counter A

					// Get Measurement
					_Result = this->Register_Pointer_Read(VARHA_NEG); // Measure Phase R

				} else if (_Type == __Energy_Reset__) {

					// Define Objects
					Register WHA_POS {0x01, 0xDD, 0}; // Received Active Energy Counter A
					Register VARHA_POS {0x02, 0x13, 0}; // Reactive Energy Leading Counter A

					// Clear Register
					Register_Pointer_Set(WHA_POS, 0x00); // Clear Active Energy Counter A
					Register_Pointer_Set(VARHA_POS, 0x00); // Clear ReActive Energy Counter A

					// End Function
					return(1);

				}

			} else if (_Phase == __Phase_S__) {

				// Measurement Type Select
				if (_Type == __Active_Received__) {

					// Define Objects
					Register WHB_POS {0x01, 0xEF, 0}; // Received Active Energy Counter B

					// Get Measurement
					_Result = this->Register_Pointer_Read(WHB_POS); // Measure Phase R

				} else if (_Type == __Active_Delivered__) {

					// Define Objects
					Register WHB_NEG {0x01, 0xF8, 0}; // Delivered Active Energy Counter B

					// Get Measurement
					_Result = this->Register_Pointer_Read(WHB_NEG); // Measure Phase R

				} else if (_Type == __ReActive_Received__) {

					// Define Objects
					Register VARHB_POS {0x02, 0x25, 0}; // Reactive Energy Leading Counter B

					// Get Measurement
					_Result = this->Register_Pointer_Read(VARHB_POS); // Measure Phase R

				} else if (_Type == __ReActive_Delivered__) {

					// Define Objects
					Register VARHB_NEG {0x02, 0x2E, 0}; // Reactive Energy Lagging Counter B

					// Get Measurement
					_Result = this->Register_Pointer_Read(VARHB_NEG); // Measure Phase R

				} else if (_Type == __Energy_Reset__) {

					// Define Objects
					Register WHB_POS {0x01, 0xEF, 0}; // Received Active Energy Counter B
					Register VARHB_POS {0x02, 0x25, 0}; // Reactive Energy Leading Counter B

					// Clear Register
					Register_Pointer_Set(WHB_POS, 0x00); // Clear Active Energy Counter B
					Register_Pointer_Set(VARHB_POS, 0x00); // Clear ReActive Energy Counter B

					// End Function
					return(1);
					
				}

			} else if (_Phase == __Phase_T__) {

				// Measurement Type Select
				if (_Type == __Active_Received__) {

					// Define Objects
					Register WHC_POS {0x02, 0x01, 0}; // Received Active Energy Counter C

					// Get Measurement
					_Result = this->Register_Pointer_Read(WHC_POS); // Measure Phase R

				} else if (_Type == __Active_Delivered__) {

					// Define Objects
					Register WHC_NEG {0x02, 0x0A, 0}; // Delivered Active Energy Counter C

					// Get Measurement
					_Result = this->Register_Pointer_Read(WHC_NEG); // Measure Phase R

				} else if (_Type == __ReActive_Received__) {

					// Define Objects
					Register VARHC_POS {0x02, 0x37, 0}; // Reactive Energy Leading Counter C

					// Get Measurement
					_Result = this->Register_Pointer_Read(VARHC_POS); // Measure Phase R

				} else if (_Type == __ReActive_Delivered__) {

					// Define Objects
					Register VARHC_NEG {0x02, 0x40, 0}; // Reactive Energy Lagging Counter C

					// Get Measurement
					_Result = this->Register_Pointer_Read(VARHC_NEG); // Measure Phase R

				} else if (_Type == __Energy_Reset__) {

					// Define Objects
					Register WHC_POS {0x02, 0x01, 0}; // Received Active Energy Counter C
					Register VARHC_POS {0x02, 0x37, 0}; // Reactive Energy Leading Counter C

					// Clear Register
					Register_Pointer_Set(WHC_POS, 0x00); // Clear Active Energy Counter C
					Register_Pointer_Set(VARHC_POS, 0x00); // Clear ReActive Energy Counter C

					// End Function
					return(1);
					
				}

			}

			// End Function
			return(_Result);
			
		}

		/**
		 * @brief Power Factor Measurement Function.
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__, __Phase_Avg__
		 * @return float Power Factor Measurement [Cos fi]
		 */
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
				_Result = this->Register_Pointer_Read(PFA); // Measure Phase Average

			} else if (_Phase == __Phase_S__) {

				// Define Objects
				Register PFB {0x01, 0x68, 22}; // Power Factor B

				// Get Measurement
				_Result = this->Register_Pointer_Read(PFB); // Measure Phase Average

			} else if (_Phase == __Phase_T__) {

				// Define Objects
				Register PFC {0x01, 0x6B, 22}; // Power Factor C

				// Get Measurement
				_Result = this->Register_Pointer_Read(PFC); // Measure Phase Average

			} else if (_Phase == __Phase_Avg__) {

				// Define Objects
				Register PFT {0x01, 0x6E, 22}; // Total Power Factor

				// Get Measurement
				_Result = this->Register_Pointer_Read(PFT); // Measure Phase Average

			}
			
			// End Function
			return(_Result);

		}

		/**
		 * @brief IC Temperature Measurement Function.
		 * @version 01.00.00
		 * @return float IC Temperature Measurement [C]
		 */
		float IC_Temperature(void) {

			// Define Objects
			Register TEMPC {0x01, 0x74, 10}; // Chip Temperature

			// Declare Variable
			float _Result = 0;

			// Get Data
			_Result = this->Register_Pointer_Read(TEMPC);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Limit Parameters Control Function.
		 * @version 01.00.00
		 * @return uint8_t 
		 * 0 - No Error
		 * 1 - Low Voltage Error
		 * 2 - High Voltage Error
		 * 5 - Low Frequency Error
		 * 6 - High Frequency Error
		 * 8 - Low Power Factor Error
		 * 9 - Voltage Imbalance Error
		 * 10 - Current Imbalance Error
		 */
		uint8_t Control_Limits(void) {

			// Define Objects
			Register STATUS {0x00, 0x15, 0}; // Alarm and device status bits

			// Read Status Register
			uint32_t _Status = this->Register_Pointer_Read(STATUS);

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

		/**
		 * @brief Set Limit Parameters to Default Values Function.
		 * @version 01.00.00
		 */
		void Set_Default_Limits(void) {

			// Set Voltage Limits
			this->Set_Voltage_Limits(Limit_Voltage_Min, Limit_Voltage_Max);

			// Set Frequency Limits
			this->Set_Frequency_Limits(Limit_Frequency_Min, Limit_Frequency_Max);

			// Set Imbalance Limits
			this->Set_Imbalance_Limits(Limit_Voltage_IMB, Limit_Current_IMB);

		}

		/**
		 * @brief Voltage Limit Set Function.
		 * @version 01.00.00
		 * @param _VMin Minimum Voltage Limit
		 * @param _VMax Maximum Voltage Limit
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_Voltage_Limits(const float _VMin, const float _VMax) {

			// Define Objects
			Register VRMS_MIN {0x00, 0xB1, 23}; // Voltage lower alarm limit
			Register VRMS_MAX {0x00, 0xB4, 23}; // Voltage upper alarm limit

			// Convert Variables
			uint32_t _VMax_RAW = this->_FtoS(_VMax / __VSCALE__, 23);
			uint32_t _VMin_RAW = this->_FtoS(_VMin / __VSCALE__, 23);

			// Write Registers
			bool _Result_Min = this->Register_Pointer_Set(VRMS_MIN, _VMin_RAW);
			bool _Result_Max = this->Register_Pointer_Set(VRMS_MAX, _VMax_RAW);

			// End Function
			return(_Result_Min and _Result_Max);

		}

		/**
		 * @brief Voltage Min Limit Read Function.
		 * @version 01.00.00
		 * @return float Minimum Voltage Limit Value.
		 */
		float Read_Voltage_Min_Limit(void) {

			// Define Objects
			Register VRMS_MIN {0x00, 0xB1, 23};		// Voltage lower alarm limit

			// Declare Variable
			float _Result = 0;

			_Result = this->Register_Pointer_Read(VRMS_MIN) * __VSCALE__;

			// End Function
			return(_Result);

		}

		/**
		 * @brief Voltage Max Limit Read Function.
		 * @version 01.00.00
		 * @return float Maximum Voltage Limit Value.
		 */
		float Read_Voltage_Max_Limit(void) {

			// Define Objects
			Register VRMS_MAX {0x00, 0xB4, 23}; // Voltage upper alarm limit

			// Declare Variable
			float _Result = 0;

			_Result = this->Register_Pointer_Read(VRMS_MAX) * __VSCALE__;

			// End Function
			return(_Result);

		}

		/**
		 * @brief Frequency Limit Set Function.
		 * @version 01.00.00
		 * @param _FQMin Minimum Frequency Limit
		 * @param _FQMax Maximum Frequency Limit
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_Frequency_Limits(const float _FQMin, const float _FQMax) {

			// Define Objects
			Register F_MIN {0x01, 0x83, 16}; // Frequency Alarm Lower Limit
			Register F_MAX {0x01, 0x86, 16}; // Frequency Alarm Upper Limit

			// Convert Variables
			uint32_t _FMax_RAW = this->_FtoS(_FQMax, F_MIN.Data_Type);
			uint32_t _FMin_RAW = this->_FtoS(_FQMin, F_MAX.Data_Type);

			// Write Registers
			bool _Result_Min = this->Register_Pointer_Set(F_MIN, _FMin_RAW);
			bool _Result_Max = this->Register_Pointer_Set(F_MAX, _FMax_RAW);

			// End Function
			return(_Result_Min and _Result_Max);

		}

		/**
		 * @brief Frequency Min Limit Read Function.
		 * @version 01.00.00
		 * @return float Minimum Frequency Limit Value.
		 */
		float Read_Frequency_Min_Limit(void) {

			// Define Objects
			Register F_MIN {0x01, 0x83, 16}; // Frequency Alarm Lower Limit

			// Declare Variable
			float _Result = 0;

			_Result = this->Register_Pointer_Read(F_MIN);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Frequency Max Limit Read Function.
		 * @version 01.00.00
		 * @return float Maximum Frequency Limit Value.
		 */
		float Read_Frequency_Max_Limit(void) {

			// Define Objects
			Register F_MAX {0x01, 0x86, 16}; // Frequency Alarm Upper Limit

			// Declare Variable
			float _Result = 0;

			_Result = this->Register_Pointer_Read(F_MAX);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Imbalance Limit Set Function.
		 * @version 01.00.00
		 * @param _VImb Voltage Imbalance Limit
		 * @param _IImb Current Imbalance Limit
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_Imbalance_Limits(const float _VImb, const float _IImb) {

			// Define Objects
			Register V_IMB_MAX {0x00, 0x81, 23}; // Voltage imbalance alarm limit
			Register I_IMB_MAX {0x00, 0x84, 23}; // Current imbalance alarm limit

			// Convert Variables
			uint32_t _VImb_RAW = this->_FtoS(_VImb, V_IMB_MAX.Data_Type);
			uint32_t _IImb_RAW = this->_FtoS(_IImb, I_IMB_MAX.Data_Type);

			// Write Registers
			bool _Result_Vimb = this->Register_Pointer_Set(V_IMB_MAX, _VImb_RAW);
			bool _Result_Iimb = this->Register_Pointer_Set(I_IMB_MAX, _IImb_RAW);

			// End Function
			return(_Result_Vimb and _Result_Iimb);

		}

		/**
		 * @brief Voltage Imbalance Limit Read Function.
		 * @version 01.00.00
		 * @return float Voltage Imbalance Limit Value.
		 */
		float Read_Voltage_Imbalance_Max_Limit(void) {

			// Define Objects
			Register V_IMB_MAX {0x00, 0x81, 23}; // Voltage imbalance alarm limit

			// Declare Variable
			float _Result = 0;

			_Result = this->Register_Pointer_Read(V_IMB_MAX);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Current Imbalance Limit Read Function.
		 * @version 01.00.00
		 * @return float Current Imbalance Limit Value.
		 */
		float Read_Current_Imbalance_Max_Limit(void) {

			// Define Objects
			Register I_IMB_MAX {0x00, 0x84, 23}; // Current imbalance alarm limit

			// Declare Variable
			float _Result = 0;

			_Result = this->Register_Pointer_Read(I_IMB_MAX);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Set Default Min Max Address
		 * @version 01.00.00
		 */
		void Set_Default_Min_Max_Address(void) {

			// Set Min Max Address
			this->Set_Min_Max_Address(1, 0x30); // VR
			this->Set_Min_Max_Address(2, 0x31); // VS
			this->Set_Min_Max_Address(3, 0x32); // VT
			this->Set_Min_Max_Address(4, 0x47); // IR
			this->Set_Min_Max_Address(5, 0x48); // IS
			this->Set_Min_Max_Address(6, 0x49); // IT

		}

		/**
		 * @brief Set Min-Max Control Address
		 * @version 01.00.00
		 * @param _MM_ADDR Address Channel
		 * @param _Mask Address Mask
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_Min_Max_Address(uint8_t _MM_ADDR, uint32_t _Mask) {

			// Define Result Variable
			bool _Result = false;

			// Decide Command
			if (_MM_ADDR == 1) {

				// Define Register
				Register MMADDR0 {0x01, 0xB9, 0}; // Min/Max Monitor address 1

				// Set Command
				_Result = this->Register_Pointer_Set(MMADDR0, _Mask);

			}
			if (_MM_ADDR == 2) {

				// Define Register
				Register MMADDR1 {0x01, 0xBC, 0}; // Min/Max Monitor address 2

				// Set Command
				_Result = this->Register_Pointer_Set(MMADDR1, _Mask);

			}
			if (_MM_ADDR == 3) {

				// Define Register
				Register MMADDR2 {0x01, 0xBF, 0}; // Min/Max Monitor address 3

				// Set Command
				_Result = this->Register_Pointer_Set(MMADDR2, _Mask);

			}
			if (_MM_ADDR == 4) {

				// Define Register
				Register MMADDR3 {0x01, 0xC2, 0}; // Min/Max Monitor address 4

				// Set Command
				_Result = this->Register_Pointer_Set(MMADDR3, _Mask);

			}
			if (_MM_ADDR == 5) {

				// Define Register
				Register MMADDR4 {0x01, 0xC5, 0}; // Min/Max Monitor address 5

				// Set Command
				_Result = this->Register_Pointer_Set(MMADDR4, _Mask);

			}
			if (_MM_ADDR == 6) {

				// Define Register
				Register MMADDR5 {0x01, 0xC8, 0}; // Min/Max Monitor address 6

				// Set Command
				_Result = this->Register_Pointer_Set(MMADDR5, _Mask);

			}
			if (_MM_ADDR == 7) {

				// Define Register
				Register MMADDR6 {0x01, 0xCB, 0}; // Min/Max Monitor address 7

				// Set Command
				_Result = this->Register_Pointer_Set(MMADDR6, _Mask);

			}
			if (_MM_ADDR == 8) {

				// Define Register
				Register MMADDR7 {0x01, 0xCE, 0}; // Min/Max Monitor address 8

				// Set Command
				_Result = this->Register_Pointer_Set(MMADDR7, _Mask);

			}

			// End Function
			return(_Result);

		}

		/**
		 * @brief Get Min Value From Address
		 * @version 01.00.00
		 * @param _MM_ADDR Address Channel
		 * @return float Minimum Value
		 */
		float Get_Min_Value(uint8_t _MM_ADDR) {

			// Define Objects
			Register MIN0 {0x01, 0x89, 23}; // Minimum Recorded Value 1
			Register MIN1 {0x01, 0x8C, 23}; // Minimum Recorded Value 2
			Register MIN2 {0x01, 0x8F, 23}; // Minimum Recorded Value 3
			Register MIN3 {0x01, 0x92, 23}; // Minimum Recorded Value 4
			Register MIN4 {0x01, 0x95, 23}; // Minimum Recorded Value 5
			Register MIN5 {0x01, 0x98, 23}; // Minimum Recorded Value 6
			Register MIN6 {0x01, 0x9B, 0}; // Minimum Recorded Value 7
			Register MIN7 {0x01, 0x9E, 0}; // Minimum Recorded Value 8

			// Declare Variable
			float _Result = 0;

			if (_MM_ADDR == 1) _Result = this->Register_Pointer_Read(MIN0) * __VSCALE__; 
			if (_MM_ADDR == 2) _Result = this->Register_Pointer_Read(MIN1) * __VSCALE__; 
			if (_MM_ADDR == 3) _Result = this->Register_Pointer_Read(MIN2) * __VSCALE__; 
			if (_MM_ADDR == 4) _Result = this->Register_Pointer_Read(MIN3) * __ISCALE__; 
			if (_MM_ADDR == 5) _Result = this->Register_Pointer_Read(MIN4) * __ISCALE__; 
			if (_MM_ADDR == 6) _Result = this->Register_Pointer_Read(MIN5) * __ISCALE__; 
			if (_MM_ADDR == 7) _Result = this->Register_Pointer_Read(MIN6); 
			if (_MM_ADDR == 8) _Result = this->Register_Pointer_Read(MIN7); 

			// End Function
			return(_Result);

		}

		/**
		 * @brief Get Max Value From Address
		 * @version 01.00.00
		 * @param _MM_ADDR Address Channel
		 * @return float Maximum Value
		 */
		float Get_Max_Value(uint8_t _MM_ADDR) {

			// Define Objects
			Register MAX0 {0x01, 0xA1, 23}; // Maximum Recorded Value 1
			Register MAX1 {0x01, 0xA4, 23};	// Maximum Recorded Value 2
			Register MAX2 {0x01, 0xA7, 23};	// Maximum Recorded Value 3
			Register MAX3 {0x01, 0xAA, 23}; // Maximum Recorded Value 4
			Register MAX4 {0x01, 0xAD, 23}; // Maximum Recorded Value 5
			Register MAX5 {0x01, 0xB0, 23}; // Maximum Recorded Value 6
			Register MAX6 {0x01, 0xB3, 0}; // Maximum Recorded Value 7
			Register MAX7 {0x01, 0xB6, 0}; // Maximum Recorded Value 8

			// Declare Variable
			float _Result = 0;

			if (_MM_ADDR == 1) _Result = this->Register_Pointer_Read(MAX0) * __VSCALE__;
			if (_MM_ADDR == 2) _Result = this->Register_Pointer_Read(MAX1) * __VSCALE__;
			if (_MM_ADDR == 3) _Result = this->Register_Pointer_Read(MAX2) * __VSCALE__;
			if (_MM_ADDR == 4) _Result = this->Register_Pointer_Read(MAX3) * __ISCALE__;
			if (_MM_ADDR == 5) _Result = this->Register_Pointer_Read(MAX4) * __ISCALE__;
			if (_MM_ADDR == 6) _Result = this->Register_Pointer_Read(MAX5) * __ISCALE__;
			if (_MM_ADDR == 7) _Result = this->Register_Pointer_Read(MAX6);
			if (_MM_ADDR == 8) _Result = this->Register_Pointer_Read(MAX7);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Clear Min Value From Address
		 * @version 01.00.00
		 * @param _MM_ADDR Address Channel
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
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

		/**
		 * @brief Clear Max Value From Address
		 * @version 01.00.00
		 * @param _MM_ADDR Address Channel
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
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

		/**
		 * @brief Get Voltage SAG Limit Function.
		 * @version 01.00.00
		 * @return float Voltage SAG
		 */
		float Get_Voltage_SAG_Limit(void) {

			// Define Objects
			Register VSAG_LIM {0x00, 0xB7, 23}; // RMS Voltage Sag threshold

			// Declare Variable
			float _Result = 0;

			_Result = this->Register_Pointer_Read(VSAG_LIM); // Measure Phase R
			
			// End Function
			return(_Result * __VSCALE__);

		}

		/**
		 * @brief Set Harmonic Channel Function.
		 * @version 01.00.00
		 * @param _Harmonic Harmonic Channel
		 * @return uint8_t 
		 * TODO: Not Tested.
		 */
		uint8_t Set_Harmonic(uint32_t _Harmonic) {

			// Define Objects
			Register HARM {0x00, 0x4B, 0}; // Harmonic Selector, default: 1 

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

		/**
		 * @brief Set Alarm Function
		 * @version 01.00.00
		 * @return true Function Succeed
		 * @return false Function Fails
		 * TODO: Working on this function
		 */
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

		/**
		 * @brief Set DIO Function
		 * @version 01.00.00
		 * @return true Function Succeed
		 * @return false Function Fails
		 * TODO: Working on this function
		 */
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