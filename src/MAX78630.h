#ifndef __MAX78630__
#define __MAX78630__

// Define Arduino Library
#ifndef __Arduino__
	#include <Arduino.h>
#endif

// Include Library Definitions
#include "Definitions.h"

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
		 * @brief IC Config Defaults Struct
		 */
		struct Config_Struct {

			/**
			 * @brief Config Defaults
			 */
			uint16_t	VScale		= 667;
			uint16_t	IScale		= 7;
			uint32_t	Bucket_High = 0x000821;
			uint32_t	Bucket_Low  = 0xD0F4C2;

			/**
			 * @brief Measurement Defaults
			 */
			struct Measurement_Struct {
				bool	Frequency 		= true;
				bool	Voltage_R		= true;
				bool	Voltage_S		= true;
				bool	Voltage_T		= true;
				bool	Current_R		= true;
				bool	Current_S		= true;
				bool	Current_T		= true;
				bool	PowerFactor_R	= true;
				bool	PowerFactor_S	= true;
				bool	PowerFactor_T	= true;
				bool	PowerFactor_A	= true;
				bool	ActiveEnergy	= true;
				bool	ReActiveEnergy	= true;
				bool	Temperature 	= true;
			} Measurement;

		} Config;

		/**
		 * @brief IC Limit Defaults Struct
		 */
		struct Limit_Struct {

			/**
			 * @brief Voltage Limit Defaults Struct
			 */
			struct Voltage_Default_Struct {
				float	Min			= 192;
				float	Max			= 223;
				float	Imbalance	= 0.06;
			} Voltage;

			/**
			 * @brief Current Limit Defaults Struct
			 */
			struct Current_Default_Struct {
				float	Max			= 5;
				float	Imbalance	= 0.06;
			} Current;

			/**
			 * @brief Frequency Limit Defaults Struct
			 */
			struct Frequency_Default_Struct {
				float	Min		= 47;
				float	Max		= 52;
			} Frequency;

			/**
			 * @brief Temperature Limit Defaults Struct
			 */
			struct Temperature_Default_Struct {
				float	Min		= 10;
				float	Max		= 28;
			} Temperature;

		} Limit;

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
			char _Parameter2 = _Data >> 8;
			char _Parameter3 = _Data >> 16;

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
		 * @brief Data Converter Function
		 * @version 01.00.00
		 * @param _Variable Variable A
		 * @param _Data_Type Data Type
		 * @return uint32_t Variable B
		 */
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
			 * @brief IC Temperature
			 */
			float Temperature;

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
		 * @brief Status Struct
		 */
		struct Status_Struct {

			/**
			 * @brief Imbalance Status
			 */
			struct Imbalance_Status_Struct {
				bool V_IMBAL		= false;
				bool I_IMBAL		= false;
			} Imbalance;
			
			/**
			 * @brief SAG Status
			 */
			struct SAG_Status_Struct {
				bool VA_SAG			= false;
				bool VB_SAG			= false;
				bool VC_SAG			= false;
			} SAG;

			/**
			 * @brief Current Status
			 */
			struct Current_Status_Struct {
				bool OV_IRMSA		= false;
				bool OV_IRMSB		= false;
				bool OV_IRMSC		= false;
			} Current;

			/**
			 * @brief Power Factor Status
			 */
			struct PowerFactor_Status_Struct {
				bool UN_PFA			= false;
				bool UN_PFB			= false;
				bool UN_PFC			= false;
			} PowerFactor;

			/**
			 * @brief Voltage Status
			 */
			struct Voltage_Status_Struct {
				bool UN_VRMSA		= false;
				bool OV_VRMSA		= false;
				bool UN_VRMSB		= false;
				bool OV_VRMSB		= false;
				bool UN_VRMSC		= false;
				bool OV_VRMSC		= false;
			} Voltage;

			/**
			 * @brief Temperature Status
			 */
			struct Temperature_Status_Struct {
				bool OV_Temp		= false;
				bool UN_Temp		= false;
			} Temperature;

			/**
			 * @brief Frequency Status
			 */
			struct Frequency_Status_Struct {
				bool OV_Freq		= false;
				bool UN_Freq		= false;
			} Frequency;

		} Status;

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
			this->Set_VScale(this->Config.VScale);
			this->Set_IScale(this->Config.IScale);

			// Bucket Set Command
			this->Set_Bucket(this->Config.Bucket_High, this->Config.Bucket_Low);

			// Set Sticky
			this->Set_Sticky(false);

			// Set Limits
			this->Set_Limit(__VRMS_MIN__, this->Limit.Voltage.Min);
			this->Set_Limit(__VRMS_MAX__, this->Limit.Voltage.Max);
			this->Set_Limit(__IRMS_MAX__, this->Limit.Current.Max);
			this->Set_Limit(__F_MIN__, this->Limit.Frequency.Min);
			this->Set_Limit(__F_MAX__, this->Limit.Frequency.Max);
			this->Set_Limit(__VIMB_MAX__, this->Limit.Voltage.Imbalance);
			this->Set_Limit(__IIMB_MAX__, this->Limit.Current.Imbalance);
			this->Set_Limit(__T_MIN__, this->Limit.Temperature.Min);
			this->Set_Limit(__T_MAX__, this->Limit.Temperature.Max);

			// Set Min Max Address
			this->Set_Min_Max_Address(1, 0x30); this->Clear_Min_Value(1); this->Clear_Max_Value(1);
			this->Set_Min_Max_Address(2, 0x31); this->Clear_Min_Value(2); this->Clear_Max_Value(2);
			this->Set_Min_Max_Address(3, 0x32); this->Clear_Min_Value(3); this->Clear_Max_Value(3);
			this->Set_Min_Max_Address(4, 0x47); this->Clear_Min_Value(4); this->Clear_Max_Value(4);
			this->Set_Min_Max_Address(5, 0x48); this->Clear_Min_Value(5); this->Clear_Max_Value(5);
			this->Set_Min_Max_Address(6, 0x49); this->Clear_Min_Value(6); this->Clear_Max_Value(6);

		}

		// Utility Functions

		/**
		 * @brief Get Device Address Function.
		 * @version 01.00.00
		 * @return uint32_t Device Address
		 */
		uint32_t Get_Device_Address(void) {

			// Define Register
			Register DEVADDR {0x00,	0x4E, 0}; // High order address bits for I2C and UART interfaces

			// Return Register
			return(this->Register_Pointer_Read(DEVADDR));

		}

		/**
		 * @brief Get Device Firmware Function
		 * @version 01.00.00
		 * @return uint32_t Device Firmware.
		 */
		uint32_t Get_Firmware(void) {

			// Define Register
			Register FW_VERSION {0x00, 0x03, 0}; // Hardware and firmware version

			// Return Register
			return(this->Register_Pointer_Read(FW_VERSION));

		}

		/**
		 * @brief Get Communication Baud Function.
		 * @version 01.00.00
		 * @return uint32_t 
		 */
		uint32_t Get_Baud(void) {

			// Define Register
			Register BAUD {0x00, 0x51, 0}; // Baud rate for UART interface

			// Return Register
			return(this->Register_Pointer_Read(BAUD));

		}

		/**
		 * @brief Set Baud Function
		 * @version 01.00.00
		 * @param _Baud Communication Baud Rate
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_Baud(const uint32_t _Baud) {

			// Define Register
			Register BAUD {0x00, 0x51, 0}; // Baud rate for UART interface

			// Return Result
			return(this->Register_Pointer_Set(BAUD, _Baud));

		}

		/**
		 * @brief Get System Status Function.
		 * @version 01.00.00
		 * @return uint32_t System Status
		 */
		uint32_t Get_System_Stat(void) {

			// Define Register
			Register SYSSTAT {0x02, 0x43, 0}; // Bit 23 is a sticky register with status of any SPI Errors

			// Return Register
			return(this->Register_Pointer_Read(SYSSTAT));

		}

		/**
		 * @brief Get Device Frame Rate Function.
		 * @version 01.00.00
		 * @return uint32_t Frame Rate
		 */
		uint32_t Get_Frame(void) {

			// Define Register
			Register FRAME {0x00, 0x12, 0}; // Low-rate sample counter

			// Return Register
			return(this->Register_Pointer_Read(FRAME));

		}

		/**
		 * @brief Get Sample Counter Function
		 * @version 01.00.00
		 * @return uint32_t Sample Counter
		 */
		uint32_t Get_Cycle(void) {

			// Define Register
			Register CYCLE {0x00, 0x0F, 0}; // High-rate sample counter

			// Return Register
			return(this->Register_Pointer_Read(CYCLE));

		}

		/**
		 * @brief Get Divisor Value Function
		 * @version 01.00.00
		 * @return uint32_t Divisor Value
		 */
		uint32_t Get_Divisor(void) {

			// Define Register
			Register DIVISOR {0x00, 0x0C, 0}; // Actual samples in previous accumulation interval

			// Return Register
			return(this->Register_Pointer_Read(DIVISOR));

		}

		/**
		 * @brief Get Samples per Accumulation Interval Function.
		 * @version 01.00.00
		 * @return uint32_t Sample per Accumulation Interval
		 */
		uint32_t Get_Samples(void) {

			// Define Register
			Register SAMPLES {0x00, 0x09, 0}; // Minimum high-rate samples per accumulation interval

			// Return Register
			return(this->Register_Pointer_Read(SAMPLES));

		}

		// Measurement Functions

		/**
		 * @brief Voltage Measurement Function.
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__, __Phase_Avg__
		 * @param _Type __RMS__, __Instant__, __Fundamental__, __Harmonic__
		 * @return float Voltage Measurement [Volt]
		 */
		float Voltage(const uint8_t _Phase, const uint8_t _Type) {

			// Decide Phase
			switch (_Phase) {

				case __Phase_R__:

					// Decide Type
					switch (_Type) {

						case __RMS__:

							// Define Register
							Register VA_RMS {0x00, 0x90, 23}; // RMS Voltage

							// Get Register
							return(this->Register_Pointer_Read(VA_RMS) * this->Config.VScale);

							// End Case
							break;

						case __Instant__:

							// Define Register
							Register VA {0x00, 0x87, 23}; // Instantaneous Voltage

							// Get Register
							return(this->Register_Pointer_Read(VA) * this->Config.VScale);

							// End Case
							break;

						case __Fundamental__:

							// Define Register
							Register VFUND_A {0x00, 0x9C, 23}; // Fundamental Voltage

							// Get Register
							return(this->Register_Pointer_Read(VFUND_A) * this->Config.VScale);

							// End Case
							break;

						case __Harmonic__:

							// Define Register
							Register VHARM_A {0x00, 0xA5, 23}; // Harmonic Voltage

							// Get Register
							return(this->Register_Pointer_Read(VHARM_A) * this->Config.VScale);

							// End Case
							break;

					default:
						break;
					}

					// End Case
					break;

				case __Phase_S__:

					// Decide Type
					switch (_Type) {

						case __RMS__:

							// Define Register
							Register VB_RMS {0x00, 0x93, 23}; // RMS Voltage

							// Get Register
							return(this->Register_Pointer_Read(VB_RMS) * this->Config.VScale);

							// End Case
							break;

						case __Instant__:

							// Define Register
							Register VB {0x00, 0x8A, 23}; // Instantaneous Voltage

							// Get Register
							return(this->Register_Pointer_Read(VB) * this->Config.VScale);

							// End Case
							break;

						case __Fundamental__:

							// Define Register
							Register VFUND_B {0x00, 0x9F, 23}; // Fundamental Voltage

							// Get Register
							return(this->Register_Pointer_Read(VFUND_B) * this->Config.VScale);

							// End Case
							break;

						case __Harmonic__:

							// Define Register
							Register VHARM_B {0x00, 0xA8, 23}; // Harmonic Voltage

							// Get Register
							return(this->Register_Pointer_Read(VHARM_B) * this->Config.VScale);

							// End Case
							break;

					default:
						break;
					}

					// End Case
					break;

				case __Phase_T__:

					// Decide Type
					switch (_Type) {

						case __RMS__:

							// Define Register
							Register VC_RMS {0x00, 0x96, 23}; // RMS Voltage

							// Get Register
							return(this->Register_Pointer_Read(VC_RMS) * this->Config.VScale);

							// End Case
							break;

						case __Instant__:

							// Define Register
							Register VC {0x00, 0x8D, 23}; // Instantaneous Voltage

							// Get Register
							return(this->Register_Pointer_Read(VC) * this->Config.VScale);

							// End Case
							break;

						case __Fundamental__:

							// Define Register
							Register VFUND_C {0x00, 0xA2, 23}; // Fundamental Voltage

							// Get Register
							return(this->Register_Pointer_Read(VFUND_C) * this->Config.VScale);

							// End Case
							break;

						case __Harmonic__:

							// Define Register
							Register VHARM_C {0x00, 0xAB, 23}; // Harmonic Voltage

							// Get Register
							return(this->Register_Pointer_Read(VHARM_C) * this->Config.VScale);

							// End Case
							break;

					default:
						break;
					}

					// End Case
					break;

				case __Phase_Avg__:

					// Define Register
					Register VT_RMS {0x00, 0x99, 23}; // RMS Voltage average (Total / 3)

					// Get Register
					return(this->Register_Pointer_Read(VT_RMS) * this->Config.VScale);

					// End Case
					break;

			default:
				break;
			}

			// End Function
			return(0);

		}

		/**
		 * @brief Frequency Measurement Function.
		 * @version 01.00.00
		 * @return float Frequency Measurement [Hz]
		 */
		float Frequency(void) {

			// Define Register
			Register FREQ {0x01, 0x80, 16}; // Line Frequency

			// Return Register
			return(this->Register_Pointer_Read(FREQ));

		}

		/**
		 * @brief Current Measurement Function.
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__, __Phase_Avg__
		 * @param _Type __RMS__, __Instant__, __Fundamental__, __Harmonic__, __Peak__, __Phase_Compensation__
		 * @return float Current Measurement [Amper]
		 */
		float Current(const uint8_t _Phase, const uint8_t _Type) {

			// Decide Phase
			switch (_Phase) {

				case __Phase_R__:

					// Decide Type
					switch (_Type) {

						case __RMS__:

							// Define Register
							Register IA_RMS {0x00, 0xD5, 23}; // RMS Current

							// Get Register
							return(this->Register_Pointer_Read(IA_RMS) * this->Config.IScale);

							// End Case
							break;

						case __Instant__:

							// Define Register
							Register IA {0x00, 0xBA, 23}; // Instantaneous Current

							// Get Register
							return(this->Register_Pointer_Read(IA) * this->Config.IScale);

							// End Case
							break;

						case __Fundamental__:

							// Define Register
							Register IFUND_A {0x00, 0xE1, 23}; // Fundamental Current

							// Get Register
							return(this->Register_Pointer_Read(IFUND_A) * this->Config.IScale);

							// End Case
							break;

						case __Harmonic__:

							// Define Register
							Register IHARM_A {0x00, 0xEA, 23}; // Harmonic Current

							// Get Register
							return(this->Register_Pointer_Read(IHARM_A) * this->Config.IScale);

							// End Case
							break;

						case __Peak__:

							// Define Register
							Register IA_PEAK {0x00, 0xCC, 23}; // Peak Current

							// Get Register
							return(this->Register_Pointer_Read(IA_PEAK) * this->Config.IScale);

							// End Case
							break;

						case __Phase_Compensation__:

							// Define Register
							Register PHASECOMP1 {0x00, 0x42, 21}; // Phase compensation (±4 samples) for AV1 input

							// Get Register
							return(this->Register_Pointer_Read(PHASECOMP1) * this->Config.IScale);

							// End Case
							break;

					default:
						break;
					}

					// End Case
					break;

				case __Phase_S__:

					// Decide Type
					switch (_Type) {

						case __RMS__:

							// Define Register
							Register IB_RMS {0x00, 0xD8, 23}; // RMS Current

							// Get Register
							return(this->Register_Pointer_Read(IB_RMS) * this->Config.IScale);

							// End Case
							break;

						case __Instant__:

							// Define Register
							Register IB {0x00, 0xBD, 23}; // Instantaneous Current

							// Get Register
							return(this->Register_Pointer_Read(IB) * this->Config.IScale);

							// End Case
							break;

						case __Fundamental__:

							// Define Register
							Register IFUND_B {0x00, 0xE4, 23}; // Fundamental Current

							// Get Register
							return(this->Register_Pointer_Read(IFUND_B) * this->Config.IScale);

							// End Case
							break;

						case __Harmonic__:

							// Define Register
							Register IHARM_B {0x00, 0xED, 23}; // Harmonic Current

							// Get Register
							return(this->Register_Pointer_Read(IHARM_B) * this->Config.IScale);

							// End Case
							break;

						case __Peak__:

							// Define Register
							Register IB_PEAK {0x00, 0xCF, 23}; // Peak Current

							// Get Register
							return(this->Register_Pointer_Read(IB_PEAK) * this->Config.IScale);

							// End Case
							break;

						case __Phase_Compensation__:

							// Define Register
							Register PHASECOMP2 {0x00, 0x45, 21}; // Phase compensation (±4 samples) for AV2 input

							// Get Register
							return(this->Register_Pointer_Read(PHASECOMP2) * this->Config.IScale);

							// End Case
							break;

					default:
						break;
					}

					// End Case
					break;

				case __Phase_T__:

					// Decide Type
					switch (_Type) {

						case __RMS__:

							// Define Register
							Register IC_RMS {0x00, 0xDB, 23}; // RMS Current

							// Get Register
							return(this->Register_Pointer_Read(IC_RMS) * this->Config.IScale);

							// End Case
							break;

						case __Instant__:

							// Define Register
							Register IC {0x00, 0xC0, 23}; // Instantaneous Current

							// Get Register
							return(this->Register_Pointer_Read(IC) * this->Config.IScale);

							// End Case
							break;

						case __Fundamental__:

							// Define Register
							Register IFUND_C {0x00, 0xE7, 23}; // Fundamental Current

							// Get Register
							return(this->Register_Pointer_Read(IFUND_C) * this->Config.IScale);

							// End Case
							break;

						case __Harmonic__:

							// Define Register
							Register IHARM_C {0x00, 0xF0, 23}; // Harmonic Current

							// Get Register
							return(this->Register_Pointer_Read(IHARM_C) * this->Config.IScale);

							// End Case
							break;

						case __Peak__:

							// Define Register
							Register IC_PEAK {0x00, 0xD2, 23}; // Peak Current

							// Get Register
							return(this->Register_Pointer_Read(IC_PEAK) * this->Config.IScale);

							// End Case
							break;

						case __Phase_Compensation__:

							// Define Register
							Register PHASECOMP3 {0x00, 0x48, 21}; // Phase compensation (±4 samples) for AV3 input

							// Get Register
							return(this->Register_Pointer_Read(PHASECOMP3) * this->Config.IScale);

							// End Case
							break;

					default:
						break;
					}

					// End Case
					break;

				case __Phase_Avg__:

					// Define Register
					Register IT_RMS {0x00, 0xDE, 23}; // RMS Current average (Total / 3)

					// Get Register
					return(this->Register_Pointer_Read(IT_RMS) * this->Config.IScale);

					// End Case
					break;

			default:
				break;
			}

			// End Function
			return(0);

		}

		/**
		 * @brief Power Measurement Function.
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__, __Phase_Avg__
		 * @param _Type __Active__, __ReActive__, __Apparent__, __Fundamental__, __Harmonic__, __Fund_ReActive__, __Harm_ReActive__, __Fund_VA__
		 * @return float Power Measurement [Watt]
		 */
		float Power(const uint8_t _Phase, const uint8_t _Type) {

			// Decide Phase
			switch (_Phase) {

				case __Phase_R__:

					// Decide Type
					switch (_Type) {

						case __Active__:

							// Define Register
							Register WATT_A {0x01, 0x1D, 23}; // Active Power

							// Get Register
							return(this->Register_Pointer_Read(WATT_A) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __ReActive__:

							// Define Register
							Register VAR_A {0x01, 0x26, 23}; // Reactive Power

							// Get Register
							return(this->Register_Pointer_Read(VAR_A) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Apparent__:

							// Define Register
							Register VA_A {0x01, 0x2F, 23};	// Apparent Power

							// Get Register
							return(this->Register_Pointer_Read(VA_A) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Fundamental__:

							// Define Register
							Register PFUND_A {0x01, 0x4A, 23}; // Fundamental Power

							// Get Register
							return(this->Register_Pointer_Read(PFUND_A) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Harmonic__:

							// Define Register
							Register PHARM_A {0x01, 0x53, 23}; // Harmonic Power

							// Get Register
							return(this->Register_Pointer_Read(PHARM_A) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Fund_ReActive__:

							// Define Register
							Register QFUND_A {0x00, 0xF9, 23}; // Fundamental Reactive Power

							// Get Register
							return(this->Register_Pointer_Read(QFUND_A) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Harm_ReActive__:

							// Define Register
							Register QHARM_A {0x01, 0x02, 23}; // Harmonic Reactive Power

							// Get Register
							return(this->Register_Pointer_Read(QHARM_A) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Fund_VA__:

							// Define Register
							Register VAFUNDA {0x01, 0x5C, 23}; // Fundamental Volt Amperes

							// Get Register
							return(this->Register_Pointer_Read(VAFUNDA) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

					default:
						break;
					}

					// End Case
					break;

				case __Phase_S__:

					// Decide Type
					switch (_Type) {

						case __Active__:

							// Define Register
							Register WATT_B {0x01, 0x20, 23}; // Active Power

							// Get Register
							return(this->Register_Pointer_Read(WATT_B) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __ReActive__:

							// Define Register
							Register VAR_B {0x01, 0x29, 23}; // Reactive Power

							// Get Register
							return(this->Register_Pointer_Read(VAR_B) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Apparent__:

							// Define Register
							Register VA_B {0x01, 0x32, 23};	// Apparent Power

							// Get Register
							return(this->Register_Pointer_Read(VA_B) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Fundamental__:

							// Define Register
							Register PFUND_B {0x01, 0x4D, 23}; // Fundamental Power

							// Get Register
							return(this->Register_Pointer_Read(PFUND_B) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Harmonic__:

							// Define Register
							Register PHARM_B {0x01, 0x56, 23}; // Harmonic Power

							// Get Register
							return(this->Register_Pointer_Read(PHARM_B) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Fund_ReActive__:

							// Define Register
							Register QFUND_B {0x00, 0xFC, 23}; // Fundamental Reactive Power

							// Get Register
							return(this->Register_Pointer_Read(QFUND_B) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Harm_ReActive__:

							// Define Register
							Register QHARM_B {0x01, 0x05, 23}; // Harmonic Reactive Power

							// Get Register
							return(this->Register_Pointer_Read(QHARM_B) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Fund_VA__:

							// Define Register
							Register VAFUNDB {0x01, 0x5F, 23}; // Fundamental Volt Amperes

							// Get Register
							return(this->Register_Pointer_Read(VAFUNDB) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

					default:
						break;
					}

					// End Case
					break;

				case __Phase_T__:

					// Decide Type
					switch (_Type) {

						case __Active__:

							// Define Register
							Register WATT_C {0x01, 0x23, 23}; // Active Power

							// Get Register
							return(this->Register_Pointer_Read(WATT_C) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __ReActive__:

							// Define Register
							Register VAR_C {0x01, 0x2C, 23}; // Reactive Power

							// Get Register
							return(this->Register_Pointer_Read(VAR_C) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Apparent__:

							// Define Register
							Register VA_C {0x01, 0x35, 23}; // Apparent Power

							// Get Register
							return(this->Register_Pointer_Read(VA_C) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Fundamental__:

							// Define Register
							Register PFUND_C {0x01, 0x50, 23}; // Fundamental Power

							// Get Register
							return(this->Register_Pointer_Read(PFUND_C) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Harmonic__:

							// Define Register
							Register PHARM_C {0x01, 0x59, 23}; // Harmonic Power

							// Get Register
							return(this->Register_Pointer_Read(PHARM_C) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Fund_ReActive__:

							// Define Register
							Register QFUND_C {0x00, 0xFF, 23}; // Fundamental Reactive Power

							// Get Register
							return(this->Register_Pointer_Read(QFUND_C) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Harm_ReActive__:

							// Define Register
							Register QHARM_C {0x01, 0x08, 23}; // Harmonic Reactive Power

							// Get Register
							return(this->Register_Pointer_Read(QHARM_C) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Fund_VA__:

							// Define Register
							Register VAFUNDC {0x01, 0x62, 23}; // Fundamental Volt Amperes

							// Get Register
							return(this->Register_Pointer_Read(VAFUNDC) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

					default:
						break;
					}

					// End Case
					break;

				case __Phase_Avg__:

					// Define Register
					Register VT_RMS {0x00, 0x99, 23}; // RMS Voltage average (Total / 3)

					// Decide Type
					switch (_Type) {

						case __Active__:

							// Define Register
							Register WATT_T {0x01, 0x38, 23}; // Active Power average (Total / 3)

							// Get Register
							return(this->Register_Pointer_Read(WATT_T) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __ReActive__:

							// Define Register
							Register VAR_T {0x01, 0x3B, 23}; // Reactive Power average (Total / 3)

							// Get Register
							return(this->Register_Pointer_Read(VAR_T) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

						case __Apparent__:

							// Define Register
							Register VA_T {0x01, 0x3E, 23}; // Apparent Power average (Total / 3)

							// Get Register
							return(this->Register_Pointer_Read(VA_T) * this->Config.VScale * this->Config.IScale);

							// End Case
							break;

					default:
						break;
					}

					// End Case
					break;

			default:
				break;
			}

			// End Function
			return(0);

		}

		/**
		 * @brief Energy Measurement Function.
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__
		 * @param _Type __Active_Received__, __Active_Delivered__, __ReActive_Received__, __ReActive_Delivered__, __Energy_Reset__
		 * @return float Energy Measurement [Watt/Hour]
		 */
		float Energy(const uint8_t _Phase, const uint8_t _Type) {

			// Decide Phase
			switch (_Phase) {

				case __Phase_R__:

					// Decide Type
					switch (_Type) {

						case __Active_Received__:

							// Define Register
							Register WHA_POS {0x01, 0xDD, 0}; // Received Active Energy Counter

							// Get Register
							return(this->Register_Pointer_Read(WHA_POS));

							// End Case
							break;

						case __Active_Delivered__:

							// Define Register
							Register WHA_NEG {0x01, 0xE6, 0}; // Delivered Active Energy Counter

							// Get Register
							return(this->Register_Pointer_Read(WHA_NEG));

							// End Case
							break;

						case __ReActive_Received__:

							// Define Register
							Register VARHA_POS {0x02, 0x13, 0};	// Reactive Energy Leading Counter

							// Get Register
							return(this->Register_Pointer_Read(VARHA_POS));

							// End Case
							break;

						case __ReActive_Delivered__:

							// Define Register
							Register VARHA_NEG {0x02, 0x1C, 0};	// Reactive Energy Lagging Counter

							// Get Register
							return(this->Register_Pointer_Read(VARHA_NEG));

							// End Case
							break;

						case __Energy_Reset__:

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

					default:
						break;
					}

					// End Case
					break;

				case __Phase_S__:

					// Decide Type
					switch (_Type) {

						case __Active_Received__:

							// Define Register
							Register WHB_POS {0x01, 0xEF, 0}; // Received Active Energy Counter

							// Get Register
							return(this->Register_Pointer_Read(WHB_POS));

							// End Case
							break;

						case __Active_Delivered__:

							// Define Register
							Register WHB_NEG {0x01, 0xF8, 0}; // Delivered Active Energy Counter

							// Get Register
							return(this->Register_Pointer_Read(WHB_NEG));

							// End Case
							break;

						case __ReActive_Received__:

							// Define Register
							Register VARHB_POS {0x02, 0x25, 0};	// Reactive Energy Leading Counter

							// Get Register
							return(this->Register_Pointer_Read(VARHB_POS));

							// End Case
							break;

						case __ReActive_Delivered__:

							// Define Register
							Register VARHB_NEG {0x02, 0x2E, 0};	// Reactive Energy Lagging Counter

							// Get Register
							return(this->Register_Pointer_Read(VARHB_NEG));

							// End Case
							break;

						case __Energy_Reset__:

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

					default:
						break;
					}

					// End Case
					break;

				case __Phase_T__:

					// Decide Type
					switch (_Type) {

						case __Active_Received__:

							// Define Register
							Register WHC_POS {0x02, 0x01, 0}; // Received Active Energy Counter

							// Get Register
							return(this->Register_Pointer_Read(WHC_POS));

							// End Case
							break;

						case __Active_Delivered__:

							// Define Register
							Register WHC_NEG {0x02, 0x0A, 0}; // Delivered Active Energy Counter

							// Get Register
							return(this->Register_Pointer_Read(WHC_NEG));

							// End Case
							break;

						case __ReActive_Received__:

							// Define Register
							Register VARHC_POS {0x02, 0x37, 0};	// Reactive Energy Leading Counter

							// Get Register
							return(this->Register_Pointer_Read(VARHC_POS));

							// End Case
							break;

						case __ReActive_Delivered__:

							// Define Register
							Register VARHC_NEG {0x02, 0x40, 0}; // Reactive Energy Lagging Counter

							// Get Register
							return(this->Register_Pointer_Read(VARHC_NEG));

							// End Case
							break;

						case __Energy_Reset__:

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

					default:
						break;
					}

					// End Case
					break;

			default:
				break;
			}

			// End Function
			return(0);

		}

		/**
		 * @brief Power Factor Measurement Function.
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__, __Phase_Avg__
		 * @return float Power Factor Measurement [Cos fi]
		 */
		float Power_Factor(const uint8_t _Phase) {

			// Decide Phase
			switch (_Phase) {

				case __Phase_R__:

					// Define Register
					Register PFA {0x01, 0x65, 22}; // Power Factor

					// Return Register
					return(this->Register_Pointer_Read(PFA));

					// End Case
					break;

				case __Phase_S__:

					// Define Register
					Register PFB {0x01, 0x68, 22}; // Power Factor

					// Return Register
					return(this->Register_Pointer_Read(PFB));

					// End Case
					break;

				case __Phase_T__:

					// Define Register
					Register PFC {0x01, 0x6B, 22}; // Power Factor

					// Return Register
					return(this->Register_Pointer_Read(PFC));

					// End Case
					break;

				case __Phase_Avg__:

					// Define Register
					Register PF_T {0x01, 0x6E, 22}; // T otal Power Factor

					// Return Register
					return(this->Register_Pointer_Read(PF_T));

					// End Case
					break;

			default:
				break;
			}

			// End Function
			return(0);

		}

		/**
		 * @brief IC Temperature Measurement Function.
		 * @version 01.00.00
		 * @return float IC Temperature Measurement [C]
		 */
		float Temperature(void) {

			// Define Register
			Register TEMPC {0x01, 0x74, 10}; // Chip Temperature (Celsius°)

			// Return Register
			return(this->Register_Pointer_Read(TEMPC));

		}

		/**
		 * @brief Measure Parameters Batch
		 * @version 01.00.00
		 */
		void Measure(void) {

			/**
			 * @brief R Phase RMS Measurement
			 */
			if (Config.Measurement.Voltage_R) {

				// Clear Variable
				Measurement.Voltage.RMS_R = 0;

				// Get Measurement
				Measurement.Voltage.RMS_R = this->Voltage(__Phase_R__, __RMS__);

				// Command Delay
				delay(10);

			}

			/**
			 * @brief S Phase RMS Measurement
			 */
			if (Config.Measurement.Voltage_S) {

				// Clear Variable
				Measurement.Voltage.RMS_S = 0;

				// Get Measurement
				Measurement.Voltage.RMS_S = this->Voltage(__Phase_S__, __RMS__);

				// Command Delay
				delay(10);

			}

			/**
			 * @brief T Phase RMS Measurement
			 */
			if (Config.Measurement.Voltage_T) {

				// Clear Variable
				Measurement.Voltage.RMS_T = 0;

				// Get Measurement
				Measurement.Voltage.RMS_T = this->Voltage(__Phase_T__, __RMS__);

				// Command Delay
				delay(10);

			}

			/**
			 * @brief R Phase RMS Measurement
			 */
			if (Config.Measurement.Current_R) {

				// Clear Variable
				Measurement.Current.RMS_R = 0;

				// Get Measurement
				Measurement.Current.RMS_R = this->Current(__Phase_R__, __RMS__);

				// Command Delay
				delay(10);

			}

			/**
			 * @brief S Phase RMS Measurement
			 */
			if (Config.Measurement.Current_S) {

				// Clear Variable
				Measurement.Current.RMS_S = 0;

				// Get Measurement
				Measurement.Current.RMS_S = this->Current(__Phase_S__, __RMS__);

				// Command Delay
				delay(10);

			}

			/**
			 * @brief T Phase RMS Measurement
			 */
			if (Config.Measurement.Current_T) {

				// Clear Variable
				Measurement.Current.RMS_T = 0;

				// Get Measurement
				Measurement.Current.RMS_T = this->Current(__Phase_T__, __RMS__);

				// Command Delay
				delay(10);

			}

			/**
			 * @brief Frequency Measurement
			 */
			if (Config.Measurement.Frequency) {

				// Clear Variable
				Measurement.Frequency = 0;

				// Get Measurement
				Measurement.Frequency = this->Frequency();

				// Command Delay
				delay(10);

			}

			/**
			 * @brief Temperature Measurement
			 */
			if (Config.Measurement.Temperature) {

				// Clear Variable
				Measurement.Temperature = 0;

				// Get Measurement
				Measurement.Temperature = this->Temperature();

				// Command Delay
				delay(10);

			}

			/**
			 * @brief R Phase RMS Measurement
			 */
			if (Config.Measurement.PowerFactor_R) {

				// Clear Variable
				Measurement.PowerFactor.Phase_R = 0;

				// Get Measurement
				Measurement.PowerFactor.Phase_R = this->Power_Factor(__Phase_R__);

				// Command Delay
				delay(10);

			}

			/**
			 * @brief S Phase RMS Measurement
			 */
			if (Config.Measurement.PowerFactor_S) {

				// Clear Variable
				Measurement.PowerFactor.Phase_S = 0;

				// Get Measurement
				Measurement.PowerFactor.Phase_S = this->Power_Factor(__Phase_S__);

				// Command Delay
				delay(10);

			}

			/**
			 * @brief T Phase RMS Measurement
			 */
			if (Config.Measurement.PowerFactor_T) {

				// Clear Variable
				Measurement.PowerFactor.Phase_T = 0;

				// Get Measurement
				Measurement.PowerFactor.Phase_T = this->Power_Factor(__Phase_T__);

				// Command Delay
				delay(10);

			}

			/**
			 * @brief Average Measurement
			 */
			if (Config.Measurement.PowerFactor_A) {

				// Clear Variable
				Measurement.PowerFactor.Average = 0;

				// Get Measurement
				Measurement.PowerFactor.Average = this->Power_Factor(__Phase_Avg__);

				// Command Delay
				delay(10);

			}

			/**
			 * @brief Active Energy
			 */
			if (Config.Measurement.ActiveEnergy) {

				// Clear Variable
				Measurement.Energy.Active = 0;

				// Calculate Total Energy Consumption
				Measurement.Energy.Active = this->Energy(__Phase_R__, __Active_Received__) + this->Energy(__Phase_S__, __Active_Received__) + this->Energy(__Phase_T__, __Active_Received__);

				// Calculate Total Cumulative Energy Consumption
				Measurement.Energy.Active_Cumulative += Measurement.Energy.Active;

				delay(10);

			}

			/**
			 * @brief ReActive Energy
			 */
			if (Config.Measurement.ReActiveEnergy) {

				// Clear Variable
				Measurement.Energy.ReActive = 0;

				// Calculate Total Energy Consumption
				Measurement.Energy.ReActive = this->Energy(__Phase_R__, __ReActive_Received__) + this->Energy(__Phase_S__, __ReActive_Received__) + this->Energy(__Phase_T__, __ReActive_Received__);

				// Calculate Total Cumulative Energy Consumption
				Measurement.Energy.ReActive_Cumulative += Measurement.Energy.ReActive;

				delay(10);

			}

			// Clear Energy Consumption
			if (Config.Measurement.ActiveEnergy or Config.Measurement.ActiveEnergy) {

				this->Energy(__Phase_R__, __Energy_Reset__);
				this->Energy(__Phase_S__, __Energy_Reset__);
				this->Energy(__Phase_T__, __Energy_Reset__);

			}

		}

		// Set Scale Functions

		/**
		 * @brief Set Voltage Scale Function
		 * @version 01.00.00
		 * @param _Voltage_Scale Voltage Scale
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_VScale(const uint32_t _Voltage_Scale) {

			// Define Register
			Register VSCALE {0x01, 0x44, 0}; // Scratch register (see Scaling Registers section)

			// Set Register
			return(this->Register_Pointer_Set(VSCALE, _Voltage_Scale));

		}

		/**
		 * @brief Set Current Scale Function
		 * @version 01.00.00
		 * @param _Current_Scale Current Scale
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_IScale(const uint32_t _Current_Scale) {

			// Define Register
			Register IFSCALE {0x01, 0x41, 0}; // Scratch register (see Scaling Registers section)

			// Set Register
			return(this->Register_Pointer_Set(IFSCALE, _Current_Scale));

		}

		/**
		 * @brief Get VSCALE Function.
		 * @version 01.00.00
		 * @return uint32_t 
		 */
		uint32_t Get_VSCale(void) {

			// Define Register
			Register VSCALE {0x01, 0x44, 0}; // Scratch register (see Scaling Registers section)

			// Return Register
			return(this->Register_Pointer_Read(VSCALE));

		}

		/**
		 * @brief Get ISCALE Function.
		 * @version 01.00.00
		 * @return uint32_t 
		 */
		uint32_t Get_ISCale(void) {

			// Define Register
			Register IFSCALE {0x01, 0x41, 0}; // Scratch register (see Scaling Registers section)

			// Return Register
			return(this->Register_Pointer_Read(IFSCALE));

		}

		/**
		 * @brief Bucket Size Get Function.
		 * @version 01.00.00
		 */
		uint32_t Get_Bucket_H(void) {

			// Define Register
			Register BUCKET_HIGH {0x01, 0xD4, 0}; // Energy Bucket Size – High word

			// Set Command
			return(this->Register_Pointer_Read(BUCKET_HIGH));

		}

		/**
		 * @brief Bucket Size Get Function.
		 * @version 01.00.00
		 */
		uint32_t Get_Bucket_L(void) {

			// Define Register
			Register BUCKET_LOW {0x01, 0xD1, 0}; // Energy Bucket Size – Low word

			// Set Command
			return(this->Register_Pointer_Read(BUCKET_LOW));

		}

		/**
		 * @brief Bucket Size Set Function.
		 * @version 01.00.00
		 * @param _Bucket_H Bucket High Word
		 * @param _Bucket_L Bucket Low Word
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_Bucket(const uint32_t _Bucket_H, const uint32_t _Bucket_L) {

			// Define Register
			Register BUCKET_HIGH {0x01,	0xD4, 0}; // Energy Bucket Size – High word
			Register BUCKET_LOW {0x01, 0xD1, 0}; // Energy Bucket Size – Low word

			// Set Command
			bool _Result_LOW = this->Register_Pointer_Set(BUCKET_LOW, _Bucket_L);
			bool _Result_HIGH = this->Register_Pointer_Set(BUCKET_HIGH, _Bucket_H);

			// End Function
			return(_Result_LOW and _Result_HIGH);

		}

		/**
		 * @brief Set Harmonic Channel Function.
		 * @version 01.00.00
		 * @param _Harmonic Harmonic Channel
		 * @return uint8_t 
		 * TODO: Not Tested.
		 */
		uint8_t Set_Harmonic(uint32_t _Harmonic) {

			Register HARM 			{0x00, 		0x4B, 		0};			// Harmonic Selector, default: 1 (fundamental)

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

		// Limit Functions

		/**
		 * @brief Limit Set Function.
		 * @version 01.00.00
		 * @param _Limit_Type Limit Type
		 * @param _Limit_Value Limit Value
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_Limit(const uint8_t _Limit_Type, const float _Limit_Value) {

			// Decide Type
			switch (_Limit_Type) {

				case __VRMS_MIN__:

					// Define Register
					Register VRMS_MIN {0x00, 0xB1, 23};	// Voltage lower alarm limit. Positive values only

					// Get Register
					return(this->Register_Pointer_Set(VRMS_MIN, this->FtoS(_Limit_Value / Config.VScale, VRMS_MIN.Data_Type)));

					// End Case
					break;

				case __VRMS_MAX__:

					// Define Register
					Register VRMS_MAX {0x00, 0xB4, 23};	// Voltage upper alarm limit. Positive values only

					// Get Register
					return(this->Register_Pointer_Set(VRMS_MAX, this->FtoS(_Limit_Value / Config.VScale, VRMS_MAX.Data_Type)));

					// End Case
					break;

				case __IRMS_MAX__:

					// Define Register
					Register IRMS_MAX {0x00, 0xF3, 23}; // Current upper alarm limit. Positive values only

					// Get Register
					return(this->Register_Pointer_Set(IRMS_MAX, this->FtoS(_Limit_Value / Config.IScale, IRMS_MAX.Data_Type)));

					// End Case
					break;

				case __F_MIN__:

					// Define Register
					Register F_MIN {0x01, 0x83, 16}; // Frequency Alarm Lower Limit

					// Get Register
					return(this->Register_Pointer_Set(F_MIN, this->FtoS(_Limit_Value, F_MIN.Data_Type)));

					// End Case
					break;

				case __F_MAX__:

					// Define Register
					Register F_MAX {0x01, 0x86, 16}; // Frequency Alarm Upper Limit

					// Get Register
					return(this->Register_Pointer_Set(F_MAX, this->FtoS(_Limit_Value, F_MAX.Data_Type)));

					// End Case
					break;

				case __T_MIN__:

					// Define Register
					Register T_MIN {0x01, 0x7A, 10}; // Temperature Alarm Lower Limit

					// Get Register
					return(this->Register_Pointer_Set(T_MIN, this->FtoS(_Limit_Value, T_MIN.Data_Type)));

					// End Case
					break;

				case __T_MAX__:

					// Define Register
					Register T_MAX {0x01, 0x7D, 10}; // Temperature Alarm Upper Limit

					// Get Register
					return(this->Register_Pointer_Set(T_MAX, this->FtoS(_Limit_Value, T_MAX.Data_Type)));

					// End Case
					break;

				case __PF_MIN__:

					// Define Register
					Register PF_MIN {0x01, 0x71, 22}; // Power Factor lower alarm limit

					// Get Register
					return(this->Register_Pointer_Set(PF_MIN, this->FtoS(_Limit_Value, PF_MIN.Data_Type)));

					// End Case
					break;

				case __VSAG_LIM__:

					// Define Register
					Register VSAG_LIM {0x00, 0xB7, 23}; // RMS Voltage Sag threshold. Positive values only

					// Get Register
					return(this->Register_Pointer_Set(VSAG_LIM, this->FtoS(_Limit_Value / Config.VScale, VSAG_LIM.Data_Type)));

					// End Case
					break;

				case __VIMB_MAX__:

					// Define Register
					Register V_IMB_MAX {0x00, 0x81, 23}; // Voltage imbalance alarm limit. Positive values only

					// Get Register
					return(this->Register_Pointer_Set(V_IMB_MAX, this->FtoS(_Limit_Value, V_IMB_MAX.Data_Type)));

					// End Case
					break;

				case __IIMB_MAX__:

					// Define Register
					Register I_IMB_MAX {0x00, 0x84, 23}; // Current imbalance alarm limit. Positive values only

					// Get Register
					return(this->Register_Pointer_Set(I_IMB_MAX, this->FtoS(_Limit_Value, I_IMB_MAX.Data_Type)));

					// End Case
					break;


			default:
				break;
			}

			// End Function
			return(false);

		}

		/**
		 * @brief Limit Read Function.
		 * @version 01.00.00
		 * @param _Limit_Type 
		 * @return float Limit Value.
		 */
		float Get_Limit(const uint8_t _Limit_Type) {

			// Decide Type
			switch (_Limit_Type) {

				case __VRMS_MIN__:

					// Define Register
					Register VRMS_MIN {0x00, 0xB1, 23};	// Voltage lower alarm limit. Positive values only

					// Get Register
					return(this->Register_Pointer_Read(VRMS_MIN) * Config.VScale);

					// End Case
					break;

				case __VRMS_MAX__:

					// Define Register
					Register VRMS_MAX {0x00, 0xB4, 23};	// Voltage upper alarm limit. Positive values only

					// Get Register
					return(this->Register_Pointer_Read(VRMS_MAX) * Config.VScale);

					// End Case
					break;

				case __IRMS_MAX__:

					// Define Register
					Register IRMS_MAX {0x00, 0xF3, 23}; // Current upper alarm limit. Positive values only

					// Get Register
					return(this->Register_Pointer_Read(IRMS_MAX) * Config.IScale);

					// End Case
					break;

				case __F_MIN__:

					// Define Register
					Register F_MIN {0x01, 0x83, 16}; // Frequency Alarm Lower Limit

					// Get Register
					return(this->Register_Pointer_Read(F_MIN));

					// End Case
					break;

				case __F_MAX__:

					// Define Register
					Register F_MAX {0x01, 0x86, 16}; // Frequency Alarm Upper Limit

					// Get Register
					return(this->Register_Pointer_Read(F_MAX));

					// End Case
					break;

				case __T_MIN__:

					// Define Register
					Register T_MIN {0x01, 0x7A, 10}; // Temperature Alarm Lower Limit

					// Get Register
					return(this->Register_Pointer_Read(T_MIN));

					// End Case
					break;

				case __T_MAX__:

					// Define Register
					Register T_MAX {0x01, 0x7D, 10}; // Temperature Alarm Upper Limit

					// Get Register
					return(this->Register_Pointer_Read(T_MAX));

					// End Case
					break;

				case __PF_MIN__:

					// Define Register
					Register PF_MIN {0x01, 0x71, 22}; // Power Factor lower alarm limit

					// Get Register
					return(this->Register_Pointer_Read(PF_MIN));

					// End Case
					break;

				case __VSAG_LIM__:

					// Define Register
					Register VSAG_LIM {0x00, 0xB7, 23}; // RMS Voltage Sag threshold. Positive values only

					// Get Register
					return(this->Register_Pointer_Read(VSAG_LIM) * Config.VScale);

					// End Case
					break;

				case __VIMB_MAX__:

					// Define Register
					Register V_IMB_MAX {0x00, 0x81, 23}; // Voltage imbalance alarm limit. Positive values only

					// Get Register
					return(this->Register_Pointer_Read(V_IMB_MAX));

					// End Case
					break;

				case __IIMB_MAX__:

					// Define Register
					Register I_IMB_MAX {0x00, 0x84, 23}; // Current imbalance alarm limit. Positive values only

					// Get Register
					return(this->Register_Pointer_Read(I_IMB_MAX));

					// End Case
					break;


			default:
				break;
			}

			// End Function
			return(0);

		}

		// Min/Max Functions

		/**
		 * @brief Set Min-Max Control Address
		 * @version 01.00.00
		 * @param _MM_ADDR Address Channel
		 * @param _Mask Address Mask
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_Min_Max_Address(const uint8_t _MM_ADDR, const uint32_t _Mask) {

			// Decide Address
			switch (_MM_ADDR) {

				case 1:

					// Define Register
					Register MMADDR0 {0x01, 0xB9, 0}; // Min/Max Monitor address 1

					// Get Register
					return(this->Register_Pointer_Set(MMADDR0, _Mask));

					// End Case
					break;

				case 2:

					// Define Register
					Register MMADDR1 {0x01, 0xBC, 0}; // Min/Max Monitor address 2

					// Get Register
					return(this->Register_Pointer_Set(MMADDR1, _Mask));

					// End Case
					break;

				case 3:

					// Define Register
					Register MMADDR2 {0x01, 0xBF, 0}; // Min/Max Monitor address 3

					// Get Register
					return(this->Register_Pointer_Set(MMADDR2, _Mask));

					// End Case
					break;

				case 4:

					// Define Register
					Register MMADDR3 {0x01, 0xC2, 0}; // Min/Max Monitor address 4

					// Get Register
					return(this->Register_Pointer_Set(MMADDR3, _Mask));

					// End Case
					break;

				case 5:

					// Define Register
					Register MMADDR4 {0x01, 0xC5, 0}; // Min/Max Monitor address 5

					// Get Register
					return(this->Register_Pointer_Set(MMADDR4, _Mask));

					// End Case
					break;

				case 6:

					// Define Register
					Register MMADDR5 {0x01, 0xC8, 0}; // Min/Max Monitor address 6

					// Get Register
					return(this->Register_Pointer_Set(MMADDR5, _Mask));

					// End Case
					break;

				case 7:

					// Define Register
					Register MMADDR6 {0x01, 0xCB, 0}; // Min/Max Monitor address 7

					// Get Register
					return(this->Register_Pointer_Set(MMADDR6, _Mask));

					// End Case
					break;

				case 8:

					// Define Register
					Register MMADDR7 {0x01, 0xCE, 0}; // Min/Max Monitor address 8

					// Get Register
					return(this->Register_Pointer_Set(MMADDR7, _Mask));

					// End Case
					break;


			default:
				break;
			}

			// End Function
			return(false);

		}

		/**
		 * @brief Get Min Value From Address
		 * @version 01.00.00
		 * @param _MM_ADDR Address Channel
		 * @return float Minimum Value
		 */
		float Get_Min_Value(const uint8_t _MM_ADDR) {

			// Decide Address
			switch (_MM_ADDR) {

				case 1:

					// Define Register
					Register MIN0 {0x01, 0x89, 23}; // Minimum Recorded Value 1

					// Get Register
					return(this->Register_Pointer_Read(MIN0) * Config.VScale);

					// End Case
					break;

				case 2:

					// Define Register
					Register MIN1 {0x01, 0x8C, 23}; // Minimum Recorded Value 2

					// Get Register
					return(this->Register_Pointer_Read(MIN1) * Config.VScale);

					// End Case
					break;

				case 3:

					// Define Register
					Register MIN2 {0x01, 0x8F, 23}; // Minimum Recorded Value 3

					// Get Register
					return(this->Register_Pointer_Read(MIN2) * Config.VScale);

					// End Case
					break;

				case 4:

					// Define Register
					Register MIN3 {0x01, 0x92, 23}; // Minimum Recorded Value 4

					// Get Register
					return(this->Register_Pointer_Read(MIN3) * Config.IScale);

					// End Case
					break;

				case 5:

					// Define Register
					Register MIN4 {0x01, 0x95, 23}; // Minimum Recorded Value 5

					// Get Register
					return(this->Register_Pointer_Read(MIN4) * Config.IScale);

					// End Case
					break;

				case 6:

					// Define Register
					Register MIN5 {0x01, 0x98, 23}; // Minimum Recorded Value 6

					// Get Register
					return(this->Register_Pointer_Read(MIN5) * Config.IScale);

					// End Case
					break;

				case 7:

					// Define Register
					Register MIN6 {0x01, 0x9B, 0}; // Minimum Recorded Value 7

					// Get Register
					return(this->Register_Pointer_Read(MIN6));

					// End Case
					break;

				case 8:

					// Define Register
					Register MIN7 {0x01, 0x9E, 0}; // Minimum Recorded Value 8

					// Get Register
					return(this->Register_Pointer_Read(MIN7));

					// End Case
					break;


			default:
				break;
			}

			// End Function
			return(0);

		}

		/**
		 * @brief Get Max Value From Address
		 * @version 01.00.00
		 * @param _MM_ADDR Address Channel
		 * @return float Maximum Value
		 */
		float Get_Max_Value(const uint8_t _MM_ADDR) {

			// Decide Address
			switch (_MM_ADDR) {

				case 1:

					// Define Register
					Register MAX0 {0x01, 0xA1, 23}; // Maximum Recorded Value 1

					// Get Register
					return(this->Register_Pointer_Read(MAX0) * Config.VScale);

					// End Case
					break;

				case 2:

					// Define Register
					Register MAX1 {0x01, 0xA4, 23}; // Maximum Recorded Value 2

					// Get Register
					return(this->Register_Pointer_Read(MAX1) * Config.VScale);

					// End Case
					break;

				case 3:

					// Define Register
					Register MAX2 {0x01, 0xA7, 23}; // Maximum Recorded Value 3

					// Get Register
					return(this->Register_Pointer_Read(MAX2) * Config.VScale);

					// End Case
					break;

				case 4:

					// Define Register
					Register MAX3 {0x01, 0xAA, 23}; // Maximum Recorded Value 4

					// Get Register
					return(this->Register_Pointer_Read(MAX3) * Config.IScale);

					// End Case
					break;

				case 5:

					// Define Register
					Register MAX4 {0x01, 0xAD, 23}; // Maximum Recorded Value 5

					// Get Register
					return(this->Register_Pointer_Read(MAX4) * Config.IScale);

					// End Case
					break;

				case 6:

					// Define Register
					Register MAX5 {0x01, 0xB0, 23}; // Maximum Recorded Value 6

					// Get Register
					return(this->Register_Pointer_Read(MAX5) * Config.IScale);

					// End Case
					break;

				case 7:

					// Define Register
					Register MAX6 {0x01, 0xB3, 0}; // Maximum Recorded Value 7

					// Get Register
					return(this->Register_Pointer_Read(MAX6));

					// End Case
					break;

				case 8:

					// Define Register
					Register MAX7 {0x01, 0xB6, 0}; // Maximum Recorded Value 8

					// Get Register
					return(this->Register_Pointer_Read(MAX7));

					// End Case
					break;


			default:
				break;
			}

			// End Function
			return(0);

		}

		/**
		 * @brief Clear Min Value From Address
		 * @version 01.00.00
		 * @param _MM_ADDR Address Channel
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Clear_Min_Value(const uint8_t _MM_ADDR) {

			// Decide Address
			switch (_MM_ADDR) {

				case 1:

					// Define Register
					Register MIN0 {0x01, 0x89, 23}; // Minimum Recorded Value 1

					// Clear Register
					Register_Pointer_Set(MIN0, 0x00);

					// End Function
					return(true);

					// End Case
					break;

				case 2:

					// Define Register
					Register MIN1 {0x01, 0x8C, 23}; // Minimum Recorded Value 2

					// Clear Register
					Register_Pointer_Set(MIN1, 0x00);

					// End Function
					return(true);

					// End Case
					break;

				case 3:

					// Define Register
					Register MIN2 {0x01, 0x8F, 23}; // Minimum Recorded Value 3

					// Clear Register
					Register_Pointer_Set(MIN2, 0x00);

					// End Function
					return(true);

					// End Case
					break;

				case 4:

					// Define Register
					Register MIN3 {0x01, 0x92, 23}; // Minimum Recorded Value 4

					// Clear Register
					Register_Pointer_Set(MIN3, 0x00);

					// End Function
					return(true);

					// End Case
					break;

				case 5:

					// Define Register
					Register MIN4 {0x01, 0x95, 23}; // Minimum Recorded Value 5

					// Clear Register
					Register_Pointer_Set(MIN4, 0x00);

					// End Function
					return(true);

					// End Case
					break;

				case 6:

					// Define Register
					Register MIN5 {0x01, 0x98, 23}; // Minimum Recorded Value 6

					// Clear Register
					Register_Pointer_Set(MIN5, 0x00);

					// End Function
					return(true);

					// End Case
					break;

				case 7:

					// Define Register
					Register MIN6 {0x01, 0x9B, 0}; // Minimum Recorded Value 7

					// Clear Register
					Register_Pointer_Set(MIN6, 0x00);

					// End Function
					return(true);

					// End Case
					break;

				case 8:

					// Define Register
					Register MIN7 {0x01, 0x9E, 0}; // Minimum Recorded Value 8

					// Clear Register
					Register_Pointer_Set(MIN7, 0x00);

					// End Function
					return(true);

					// End Case
					break;


			default:
				break;
			}

			// End Function
			return(false);

		}

		/**
		 * @brief Clear Max Value From Address
		 * @version 01.00.00
		 * @param _MM_ADDR Address Channel
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Clear_Max_Value(const uint8_t _MM_ADDR) {

			// Decide Address
			switch (_MM_ADDR) {

				case 1:

					// Define Register
					Register MAX0 {0x01, 0xA1, 23}; // Maximum Recorded Value 1

					// Clear Register
					Register_Pointer_Set(MAX0, 0x00);

					// End Function
					return(true);

					// End Case
					break;

				case 2:

					// Define Register
					Register MAX1 {0x01, 0xA4, 23}; // Maximum Recorded Value 2

					// Clear Register
					Register_Pointer_Set(MAX1, 0x00);

					// End Function
					return(true);

					// End Case
					break;

				case 3:

					// Define Register
					Register MAX2 {0x01, 0xA7, 23}; // Maximum Recorded Value 3

					// Clear Register
					Register_Pointer_Set(MAX2, 0x00);

					// End Function
					return(true);

					// End Case
					break;

				case 4:

					// Define Register
					Register MAX3 {0x01, 0xAA, 23}; // Maximum Recorded Value 4

					// Clear Register
					Register_Pointer_Set(MAX3, 0x00);

					// End Function
					return(true);

					// End Case
					break;

				case 5:

					// Define Register
					Register MAX4 {0x01, 0xAD, 23}; // Maximum Recorded Value 5

					// Clear Register
					Register_Pointer_Set(MAX4, 0x00);

					// End Function
					return(true);

					// End Case
					break;

				case 6:

					// Define Register
					Register MAX5 {0x01, 0xB0, 23}; // Maximum Recorded Value 6

					// Clear Register
					Register_Pointer_Set(MAX5, 0x00);

					// End Function
					return(true);

					// End Case
					break;

				case 7:

					// Define Register
					Register MAX6 {0x01, 0xB3, 0}; // Maximum Recorded Value 7

					// Clear Register
					Register_Pointer_Set(MAX6, 0x00);

					// End Function
					return(true);

					// End Case
					break;

				case 8:

					// Define Register
					Register MAX7 {0x01, 0xB6, 0}; // Maximum Recorded Value 8

					// Clear Register
					Register_Pointer_Set(MAX7, 0x00);

					// End Function
					return(true);

					// End Case
					break;


			default:
				break;
			}

			// End Function
			return(false);

		}

		// Calibration Functions

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

			Register I3_GAIN 		{0x00, 		0x5A, 		21};		// Current Gain Calibration.Positive values only
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

		/**
		 * @brief Temperature Calibration Function
		 * @version 01.00.00
		 * @param _Gain Gain Value of Temperature
		 * @param _Offset OffSet Value of Temperature
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Temperature_Calibration(const float _Gain, const float _Offset) {

			Register T_GAIN 		{0x00, 		0x78, 		0};			// Temperature Slope Calibration
			Register T_OFFS 		{0x00, 		0x7B, 		0};			// Temperature Offset Calibration

			bool _Result_Gain = this->Register_Pointer_Set(T_GAIN, this->FtoS(_Gain, T_GAIN.Data_Type)); // Write Gain
			bool _Result_Offset = this->Register_Pointer_Set(T_OFFS, this->FtoS(_Offset, T_GAIN.Data_Type)); // Write Offset

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

		/**
		 * @brief Current High Pass Filter Coefficient Function.
		 * @version 01.00.00
		 * @param _COEF Coefficient Value
		 * @return float Coefficient Value
		 * TODO: Not Tested.
		 */
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

		/**
		 * @brief Current RMS Offset Set Function.
		 * @version 01.00.00
		 * @param Phase 
		 * @param _Offset 
		 * @return float 
		 * TODO: Not Tested
		 */
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

		/**
		 * @brief Active Power Offset Set Function.
		 * @version 01.00.00
		 * @param Phase 
		 * @param _Offset 
		 * @return float 
		 * TODO: Not Tested
		 */
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

		/**
		 * @brief ReActive Power Offset Set Function.
		 * @version 01.00.00
		 * @param Phase 
		 * @param _Offset 
		 * @return float 
		 * TODO: Not Tested
		 */
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

		// Status Functions

		/**
		 * @brief Set Sticky Function
		 * @version 01.00.00
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
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

		/**
		 * @brief Set Sticky Function
		 * @version 01.00.00
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
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

		/**
		 * @brief Limit Parameters Control Function.
		 * @version 01.00.00
		 */
		void Get_Status(void) {

			// Define Register
			Register STATUS {0x00, 0x15, 0}; // Alarm and device status bits

			// Read Status Register
			uint32_t _Last_Status = this->Register_Pointer_Read(STATUS);

			// Control Status
			if (bitRead(_Last_Status, 2)) {this->Status.Imbalance.I_IMBAL = true;} else {this->Status.Imbalance.I_IMBAL = false;}
			if (bitRead(_Last_Status, 3)) {this->Status.Imbalance.V_IMBAL = true;} else {this->Status.Imbalance.V_IMBAL = false;}
			if (bitRead(_Last_Status, 4)) {this->Status.SAG.VA_SAG = true;} else {this->Status.SAG.VA_SAG = false;}
			if (bitRead(_Last_Status, 5)) {this->Status.SAG.VB_SAG = true;} else {this->Status.SAG.VB_SAG = false;}
			if (bitRead(_Last_Status, 6)) {this->Status.SAG.VC_SAG = true;} else {this->Status.SAG.VC_SAG = false;}
			if (bitRead(_Last_Status, 7)) {this->Status.Current.OV_IRMSA = true;} else {this->Status.Current.OV_IRMSA = false;}
			if (bitRead(_Last_Status, 8)) {this->Status.Current.OV_IRMSB = true;} else {this->Status.Current.OV_IRMSB = false;}
			if (bitRead(_Last_Status, 9)) {this->Status.Current.OV_IRMSC = true;} else {this->Status.Current.OV_IRMSC = false;}
			if (bitRead(_Last_Status, 10)) {this->Status.PowerFactor.UN_PFA = true;} else {this->Status.PowerFactor.UN_PFA = false;}
			if (bitRead(_Last_Status, 11)) {this->Status.PowerFactor.UN_PFB = true;} else {this->Status.PowerFactor.UN_PFB = false;}
			if (bitRead(_Last_Status, 12)) {this->Status.PowerFactor.UN_PFC = true;} else {this->Status.PowerFactor.UN_PFC = false;}
			if (bitRead(_Last_Status, 13)) {this->Status.Voltage.UN_VRMSA = true;} else {this->Status.Voltage.UN_VRMSA = false;}
			if (bitRead(_Last_Status, 14)) {this->Status.Voltage.OV_VRMSA = true;} else {this->Status.Voltage.OV_VRMSA = false;}
			if (bitRead(_Last_Status, 15)) {this->Status.Voltage.UN_VRMSB = true;} else {this->Status.Voltage.UN_VRMSB = false;}
			if (bitRead(_Last_Status, 16)) {this->Status.Voltage.OV_VRMSB = true;} else {this->Status.Voltage.OV_VRMSB = false;}
			if (bitRead(_Last_Status, 17)) {this->Status.Voltage.UN_VRMSC = true;} else {this->Status.Voltage.UN_VRMSC = false;}
			if (bitRead(_Last_Status, 18)) {this->Status.Voltage.OV_VRMSC = true;} else {this->Status.Voltage.OV_VRMSC = false;}
			if (bitRead(_Last_Status, 19)) {this->Status.Temperature.UN_Temp = true;} else {this->Status.Temperature.UN_Temp = false;}
			if (bitRead(_Last_Status, 20)) {this->Status.Temperature.OV_Temp = true;} else {this->Status.Temperature.OV_Temp = false;}
			if (bitRead(_Last_Status, 21)) {this->Status.Frequency.UN_Freq = true;} else {this->Status.Frequency.UN_Freq = false;}
			if (bitRead(_Last_Status, 22)) {this->Status.Frequency.OV_Freq = true;} else {this->Status.Frequency.OV_Freq = false;}

		}
	
		/**
		 * @brief Clear IC Status Register.
		 * @version 01.00.00
		 */
		void Clear_Status(void) {

			// Define Register
			Register STATUS {0x00, 0x15, 0}; // Alarm and device status bits

			// Clear Status Register
			Register_Pointer_Set(STATUS, 0x0000);

		}

		// Alarm and DIO Functions

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