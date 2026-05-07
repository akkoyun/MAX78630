#ifndef __MAX78630__
#define __MAX78630__

	// Define Arduino Library
	#ifndef __Arduino__
		#include <Arduino.h>
	#endif

	// Include External Files
	#include "Definitions.h"
	#include "Config.h"
	#include "Registers.h"

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

			// Limit Variables
			struct Limit_Struct {
				float V_Min = MAX78630Config::VoltageMin;
				float V_Max = MAX78630Config::VoltageMax;
				float I_Max = MAX78630Config::CurrentMax;
				float FQ_Min = MAX78630Config::FreqMin;
				float FQ_Max = MAX78630Config::FreqMax;
				float VImb_Max = MAX78630Config::VImbMax;
				float IImb_Max = MAX78630Config::IImbMax;
				float T_Min = MAX78630Config::TempMin;
				float T_Max = MAX78630Config::TempMax;
			} Limit_Variables;

			// Calculate CheckSum Function
			uint8_t CheckSum(const uint8_t _Low_Address, const uint8_t _High_Address) {

				// Calculate CheckSum
				return(0x100 - ((MAX78630Def::Proto::Header + MAX78630Def::Proto::Bytes7 + MAX78630Def::Proto::RWAddr + _Low_Address + _High_Address + MAX78630Def::Proto::Read3Bytes) % 256));

			}
			uint8_t CheckSum(const uint8_t _Low_Address, const uint8_t _High_Address, const uint8_t _Data1, const uint8_t _Data2, const uint8_t _Data3) {

				// Calculate CheckSum
				return(0x100 - ((MAX78630Def::Proto::Header + MAX78630Def::Proto::Bytes10 + MAX78630Def::Proto::RWAddr + _Low_Address + _High_Address + MAX78630Def::Proto::Write3Bytes + _Data1 + _Data2 + _Data3) % 256));

			}
			uint8_t CheckSum(const uint8_t _Response[6]) {

				// Calculate CheckSum
				return(0x100 - ((_Response[0] + _Response[1] + _Response[2] + _Response[3] + _Response[4]) % 256));

			}
			uint8_t CheckSum(const uint8_t _Address) {

				// Calculate CheckSum
				return(0x100 - ((MAX78630Def::Proto::Header + MAX78630Def::Proto::Bytes4 + _Address) % 256));

			}

			// Clear Serial Buffer Function
			void Clear_Buffer(void) {

				// Flush UART Buffer
				Serial_Energy->flush();

				// Clear UART Buffer
				while(Serial_Energy->available() > 0) {

					// Read Serial Char
					Serial_Energy->read();

					// Stream Delay
					delayMicroseconds(20);

				}

			}

			// Register Read Function.
			float Register_Pointer_Read(Register _Command) {

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

				// Send Command
				Serial_Energy->write(MAX78630Def::Proto::Header);
				Serial_Energy->write(MAX78630Def::Proto::Bytes7);
				Serial_Energy->write(MAX78630Def::Proto::RWAddr);
				Serial_Energy->write(_Command.Low_Address);
				Serial_Energy->write(_Command.High_Address);
				Serial_Energy->write(MAX78630Def::Proto::Read3Bytes);
				Serial_Energy->write(this->CheckSum(_Command.Low_Address, _Command.High_Address));

				// Command Send Delay
				delay(3);

				// Declare Variable
				uint8_t _Response[6];
				memset(_Response, '\0', 6);
				uint8_t _Response_Order = 0;

				// Read UART Response
				while(Serial_Energy->available() > 0 && _Response_Order < 6) {

					// Read Serial Char
					_Response[_Response_Order] = Serial_Energy->read();

					// Increase Read Order
					_Response_Order++;

					// Stream Delay
					delayMicroseconds(200);

				}

				// Declare Raw Data Variable
				float _Data_SUM = 0.0f;

				// Control Received Data
				if (_Response[0] == MAX78630Def::Proto::Header && _Response[1] == MAX78630Def::Proto::Bytes6 && _Response[5] == this->CheckSum(_Response)) {

					// Combine Read Bytes
					uint32_t _Data_RAW = ((uint32_t)(_Response[4]) << 16 | ((uint32_t)_Response[3]) << 8 | ((uint32_t)_Response[2]));

					// Calculate Response
					if (_Command.Data_Type == 0) {

						// Handle Response (int)
						_Data_SUM = (float)_Data_RAW;

					} else {

						// Sign-extend 24-bit two's complement to 32-bit, then scale
						int32_t _Data_SIGNED = (_Data_RAW & 0x800000U) ? (int32_t)(_Data_RAW | 0xFF000000U) : (int32_t)_Data_RAW;
						_Data_SUM = (float)_Data_SIGNED / (float)(1UL << _Command.Data_Type);

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
				Serial_Energy->write(MAX78630Def::Proto::Header);
				Serial_Energy->write(MAX78630Def::Proto::Bytes10);
				Serial_Energy->write(MAX78630Def::Proto::RWAddr);
				Serial_Energy->write(_Command.Low_Address);
				Serial_Energy->write(_Command.High_Address);
				Serial_Energy->write(MAX78630Def::Proto::Write3Bytes);
				Serial_Energy->write(_Parameter1);
				Serial_Energy->write(_Parameter2);
				Serial_Energy->write(_Parameter3);
				Serial_Energy->write(this->CheckSum(_Command.Low_Address, _Command.High_Address, _Parameter1, _Parameter2, _Parameter3));

				// Command Delay
				delay(5);

				// End Function
				if (Serial_Energy->available() > 0 && Serial_Energy->read() == MAX78630Def::Proto::AckNoData) return(true);

				// End Function
				return(false);

			}

			// Data Converter — float to 24-bit signed fixed-point two's complement
			int32_t FtoS(double _Variable, uint8_t _Data_Type) {

				// Scale by 2^Data_Type, round, mask to 24 bits
				return((int32_t)round(_Variable * (double)(1UL << _Data_Type)) & 0x00FFFFFF);

			}

			// Min/Max register read-and-clear helper
			float MM_Read(const Register _Reg, const uint16_t _Scale) {

				float _Value = this->Register_Pointer_Read(_Reg) * (float)_Scale;
				this->Register_Pointer_Set(_Reg, 0x00);
				return(_Value);

			}

			/* Scale Config Functions */

			// VScale Function
			uint32_t VScale(const uint8_t _Function = MAX78630Def::SET) {

				// Control for Function
				if (_Function == MAX78630Def::SET) {

					// Set Register
					return(static_cast<uint32_t>(this->Register_Pointer_Set(MAX78630Reg::VSCALE, MAX78630Config::VScale)));

				} else if (_Function == MAX78630Def::GET) {

					// Read Register
					return(static_cast<uint32_t>(this->Register_Pointer_Read(MAX78630Reg::VSCALE)));

				} else {

					// End Function
					return(0);

				}

			}

			// IScale Function
			uint32_t IScale(const uint8_t _Function = MAX78630Def::SET) {

				// Control for Function
				if (_Function == MAX78630Def::SET) {

					// Set Register
					return(static_cast<uint32_t>(this->Register_Pointer_Set(MAX78630Reg::IFSCALE, MAX78630Config::IScale)));

				} else if (_Function == MAX78630Def::GET) {

					// Read Register
					return(static_cast<uint32_t>(this->Register_Pointer_Read(MAX78630Reg::IFSCALE)));

				} else {

					// End Function
					return(0);

				}

			}

			// Bucket High Function
			uint32_t Bucket_H(const uint8_t _Function = MAX78630Def::SET) {

				// Control for Function
				if (_Function == MAX78630Def::SET) {

					// Set Register
					return(static_cast<uint32_t>(this->Register_Pointer_Set(MAX78630Reg::BUCKET_H, MAX78630Config::BucketH)));

				} else if (_Function == MAX78630Def::GET) {

					// Read Register
					return(static_cast<uint32_t>(this->Register_Pointer_Read(MAX78630Reg::BUCKET_H)));

				} else {

					// End Function
					return(0);

				}

			}

			// Bucket Low Function
			uint32_t Bucket_L(const uint8_t _Function = MAX78630Def::SET) {

				// Control for Function
				if (_Function == MAX78630Def::SET) {

					// Set Register
					return(static_cast<uint32_t>(this->Register_Pointer_Set(MAX78630Reg::BUCKET_L, MAX78630Config::BucketL)));

				} else if (_Function == MAX78630Def::GET) {

					// Read Register
					return(static_cast<uint32_t>(this->Register_Pointer_Read(MAX78630Reg::BUCKET_L)));

				} else {

					// End Function
					return(0);

				}

			}

			/* Min Max Functions */

			// Set Min-Max Control Address
			bool Set_Min_Max_Mask(const uint8_t _MM_ADDR) {

				// Control for Mask
				if (_MM_ADDR > 8) return(false);

				// Decide Address
				switch (_MM_ADDR) {

					case MAX78630Def::Monitor1: {

						// Set Register
						bool _Set = this->Register_Pointer_Set(MAX78630Reg::MMADDR0, MAX78630Config::Monitor1Type);

						// Clear Register
						this->Min_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor1, MAX78630Config::Monitor1Scale);
						this->Max_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor1, MAX78630Config::Monitor1Scale);

						// End Function
						return(_Set);

					}
					case MAX78630Def::Monitor2: {

						// Set Register
						bool _Set = this->Register_Pointer_Set(MAX78630Reg::MMADDR1, MAX78630Config::Monitor2Type);

						// Clear Register
						this->Min_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor2, MAX78630Config::Monitor2Scale);
						this->Max_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor2, MAX78630Config::Monitor2Scale);

						// End Function
						return(_Set);

					}
					case MAX78630Def::Monitor3: {

						// Set Register
						bool _Set = this->Register_Pointer_Set(MAX78630Reg::MMADDR2, MAX78630Config::Monitor3Type);

						// Clear Register
						this->Min_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor3, MAX78630Config::Monitor3Scale);
						this->Max_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor3, MAX78630Config::Monitor3Scale);

						// End Function
						return(_Set);

					}
					case MAX78630Def::Monitor4: {

						// Set Register
						bool _Set = this->Register_Pointer_Set(MAX78630Reg::MMADDR3, MAX78630Config::Monitor4Type);

						// Clear Register
						this->Min_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor4, MAX78630Config::Monitor4Scale);
						this->Max_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor4, MAX78630Config::Monitor4Scale);

						// End Function
						return(_Set);

					}
					case MAX78630Def::Monitor5: {

						// Set Register
						bool _Set = this->Register_Pointer_Set(MAX78630Reg::MMADDR4, MAX78630Config::Monitor5Type);

						// Clear Register
						this->Min_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor5, MAX78630Config::Monitor5Scale);
						this->Max_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor5, MAX78630Config::Monitor5Scale);

						// End Function
						return(_Set);

					}
					case MAX78630Def::Monitor6: {

						// Set Register
						bool _Set = this->Register_Pointer_Set(MAX78630Reg::MMADDR5, MAX78630Config::Monitor6Type);

						// Clear Register
						this->Min_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor6, MAX78630Config::Monitor6Scale);
						this->Max_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor6, MAX78630Config::Monitor6Scale);

						// End Function
						return(_Set);

					}
					case MAX78630Def::Monitor7: {

						// Set Register
						bool _Set = this->Register_Pointer_Set(MAX78630Reg::MMADDR6, MAX78630Config::Monitor7Type);

						// Clear Register
						this->Min_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor7, MAX78630Config::Monitor7Scale);
						this->Max_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor7, MAX78630Config::Monitor7Scale);

						// End Function
						return(_Set);

					}
					case MAX78630Def::Monitor8: {

						// Set Register
						bool _Set = this->Register_Pointer_Set(MAX78630Reg::MMADDR7, MAX78630Config::Monitor8Type);

						// Clear Register
						this->Min_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor8, MAX78630Config::Monitor8Scale);
						this->Max_Value(MAX78630Def::CLEAR, MAX78630Def::Monitor8, MAX78630Config::Monitor8Scale);

						// End Function
						return(_Set);

					}

				}

				// End Function
				return(false);

			}

			/* Alarm Functions */

			// Alarm Sticky Interrupt Function.
			bool Sticky(const uint8_t _Function = MAX78630Def::GET, const bool _Sticky = false) {

				// Get Current Register
				uint32_t _Current_Sticky = static_cast<uint32_t>(this->Register_Pointer_Read(MAX78630Reg::STICKY));

				// Control for Function
				if (_Function == MAX78630Def::SET) {

					// Set Sticky Bit
					if (_Sticky) {

						// Set Register
						_Current_Sticky |= 0x7FFFFC;

					} else {

						// Set Register
						_Current_Sticky &= 0xFF800003;

					}

					// End Function
					this->Register_Pointer_Set(MAX78630Reg::STICKY, _Current_Sticky);

				}

				// End Function
				return((_Current_Sticky & 0x7FFFFC) != 0);

			}

			// Alarm Mask Function.
			uint32_t Alarm_Mask(const uint8_t _Function, const uint8_t _Mask, const uint8_t _Bit, const bool _Status) {

				// Control for Mask
				if (_Mask == MAX78630Def::Mask_AL1) {

					// Read Register
					uint32_t _MASK = static_cast<uint32_t>(this->Register_Pointer_Read(MAX78630Reg::MASK1));

					// Set Mask bit
					if (_Function == MAX78630Def::SET) {

						// Set Mask bit
						if (_Status) {

							// Set Bit
							bitSet(_MASK, _Bit);

						} else {

							// Clear Bit
							bitClear(_MASK, _Bit);

						}

						// Write MASK1 Register
						this->Register_Pointer_Set(MAX78630Reg::MASK1, _MASK);

						// End Function
						return(_MASK);

					}

					// End Function
					return(_MASK);

				} else if (_Mask == MAX78630Def::Mask_AL2) {

					// Read Register
					uint32_t _MASK = static_cast<uint32_t>(this->Register_Pointer_Read(MAX78630Reg::MASK2));

					// Set Mask bit
					if (_Function == MAX78630Def::SET) {

						// Set Mask bit
						if (_Status) {

							// Set Bit
							bitSet(_MASK, _Bit);

						} else {

							// Clear Bit
							bitClear(_MASK, _Bit);

						}

						// Write MASK2 Register
						this->Register_Pointer_Set(MAX78630Reg::MASK2, _MASK);

						// End Function
						return(_MASK);

					}

					// End Function
					return(_MASK);

				} else {

					// End Function
					return(0);

				}

			}

		// Protected Context
		protected:

			/* IC Utility Functions */

			// Get Device Firmware Function
			uint32_t Firmware(void) {

				// Return Register
				return(static_cast<uint32_t>(this->Register_Pointer_Read(MAX78630Reg::FW_VERSION)));

				// Default Firmware: 0x0004D912

			}

			// Get Device Address Function.
			uint32_t Address(void) {

				// Return Register
				return(static_cast<uint32_t>(this->Register_Pointer_Read(MAX78630Reg::DEVADDR)));

			}

			// Select IC Function
			bool Select_IC(uint8_t _Address) {

				// Send Command
				Serial_Energy->write(MAX78630Def::Proto::Header);			// Header (0xAA)
				Serial_Energy->write(MAX78630Def::Proto::Bytes4);			// Total Sended Byte (0x04)
				Serial_Energy->write(_Address);						// IC Address (0xC1, 0xC2, 0xC3, 0xC4)
				Serial_Energy->write(this->CheckSum(_Address));		// CheckSum (0x__)

				// Command Send Delay
				delay(2);

				// End Function
				if (Serial_Energy->available() > 0 && Serial_Energy->read() == MAX78630Def::Proto::AckNoData) return(true);

				// End Function
				return(false);

			}

			// Baud Rate Function
			uint32_t Baud(const uint8_t _Function, const uint32_t _Baud) {

				// Control for Function
				if (_Function == MAX78630Def::SET) {

					// Set Register
					return(static_cast<uint32_t>(this->Register_Pointer_Set(MAX78630Reg::BAUD, _Baud)));

				} else if (_Function == MAX78630Def::GET) {

					// Read Register
					return(static_cast<uint32_t>(this->Register_Pointer_Read(MAX78630Reg::BAUD)));

				}

			}

			// Get Device Frame Rate Function.
			uint32_t Frame(void) {

				// Return Register
				return(static_cast<uint32_t>(this->Register_Pointer_Read(MAX78630Reg::FRAME)));

			}

			// Get Sample Counter Function
			uint32_t Cycle(void) {

				// Return Register
				return(static_cast<uint32_t>(this->Register_Pointer_Read(MAX78630Reg::CYCLE)));

			}

			// Get Divisor Value Function
			uint32_t Divisor(void) {

				// Return Register
				return(static_cast<uint32_t>(this->Register_Pointer_Read(MAX78630Reg::DIVISOR)));

			}

			// Get Samples per Accumulation Interval Function.
			uint32_t Samples(void) {

				// Return Register
				return(static_cast<uint32_t>(this->Register_Pointer_Read(MAX78630Reg::SAMPLES)));

			}

			// Get System Status Function.
			uint32_t System_Status(void) {

				// Return Register
				return(static_cast<uint32_t>(this->Register_Pointer_Read(MAX78630Reg::SYSSTAT)));

			}

			/* Calibration Functions */

			// Calibration Function
			bool Calibration(const uint8_t _Type, const uint8_t _Phase, const float _Gain, const float _Offset) {

				// Control Input Parameters
				if (_Phase > 3) return(false);

				// Control for Calibration Type
				switch (_Type) {

					// Voltage Calibration
					case MAX78630Def::Voltage: {

						// Control for Phase
						switch (_Phase) {

							// Phase R
							case MAX78630Def::Phase_R: {

								bool _Result_Gain = this->Register_Pointer_Set(MAX78630Reg::V1_GAIN, this->FtoS(_Gain, MAX78630Reg::V1_GAIN.Data_Type));
								bool _Result_Offset = this->Register_Pointer_Set(MAX78630Reg::V1_OFFS, this->FtoS(_Offset, MAX78630Reg::V1_OFFS.Data_Type));

								// End Function
								return(_Result_Gain && _Result_Offset);

							}

							// Phase S
							case MAX78630Def::Phase_S: {

								bool _Result_Gain = this->Register_Pointer_Set(MAX78630Reg::V2_GAIN, this->FtoS(_Gain, MAX78630Reg::V2_GAIN.Data_Type));
								bool _Result_Offset = this->Register_Pointer_Set(MAX78630Reg::V2_OFFS, this->FtoS(_Offset, MAX78630Reg::V2_OFFS.Data_Type));

								// End Function
								return(_Result_Gain && _Result_Offset);

							}

							// Phase T
							case MAX78630Def::Phase_T: {

								bool _Result_Gain = this->Register_Pointer_Set(MAX78630Reg::V3_GAIN, this->FtoS(_Gain, MAX78630Reg::V3_GAIN.Data_Type));
								bool _Result_Offset = this->Register_Pointer_Set(MAX78630Reg::V3_OFFS, this->FtoS(_Offset, MAX78630Reg::V3_OFFS.Data_Type));

								// End Function
								return(_Result_Gain && _Result_Offset);

							}

							// No Phase
							default: {

								// End Function
								return(false);

							}

						}

					}

					// Current Calibration
					case MAX78630Def::Current: {

						// Control for Phase
						switch (_Phase) {

							// Phase R
							case MAX78630Def::Phase_R: {

								bool _Result_Gain = this->Register_Pointer_Set(MAX78630Reg::I1_GAIN, this->FtoS(_Gain, MAX78630Reg::I1_GAIN.Data_Type));
								bool _Result_Offset = this->Register_Pointer_Set(MAX78630Reg::I1_OFFS, this->FtoS(_Offset, MAX78630Reg::I1_OFFS.Data_Type));

								// End Function
								return(_Result_Gain && _Result_Offset);

							}

							// Phase S
							case MAX78630Def::Phase_S: {

								bool _Result_Gain = this->Register_Pointer_Set(MAX78630Reg::I2_GAIN, this->FtoS(_Gain, MAX78630Reg::I2_GAIN.Data_Type));
								bool _Result_Offset = this->Register_Pointer_Set(MAX78630Reg::I2_OFFS, this->FtoS(_Offset, MAX78630Reg::I2_OFFS.Data_Type));

								// End Function
								return(_Result_Gain && _Result_Offset);

							}

							// Phase T
							case MAX78630Def::Phase_T: {

								bool _Result_Gain = this->Register_Pointer_Set(MAX78630Reg::I3_GAIN, this->FtoS(_Gain, MAX78630Reg::I3_GAIN.Data_Type));
								bool _Result_Offset = this->Register_Pointer_Set(MAX78630Reg::I3_OFFS, this->FtoS(_Offset, MAX78630Reg::I3_OFFS.Data_Type));

								// End Function
								return(_Result_Gain && _Result_Offset);

							}

							// No Phase
							default: {

								// End Function
								return(false);

							}

						}

					}

					// Temperature Calibration
					case MAX78630Def::Temperature: {

						bool _Result_Gain = this->Register_Pointer_Set(MAX78630Reg::T_GAIN, this->FtoS(_Gain, MAX78630Reg::T_GAIN.Data_Type));
						bool _Result_Offset = this->Register_Pointer_Set(MAX78630Reg::T_OFFS, this->FtoS(_Offset, MAX78630Reg::T_OFFS.Data_Type));

						// End Function
						return(_Result_Gain && _Result_Offset);

					}

					// No Calibration
					default: {

						// End Function
						return(false);

					}

				}

			}

			// Coefficients Function
			float HPF_COEF(const uint8_t _Function, const uint8_t _Type, float _COEF) {

				// Control for Type
				if (_Type == MAX78630Def::Voltage) {

					// Read Register
					float _Result = this->Register_Pointer_Read(MAX78630Reg::HPF_COEF_V);

					// Set Register
					if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::HPF_COEF_V, this->FtoS(_COEF, MAX78630Reg::HPF_COEF_V.Data_Type));

					// End Function
					return(_Result);

				} else if (_Type == MAX78630Def::Current) {

					// Read Register
					float _Result = this->Register_Pointer_Read(MAX78630Reg::HPF_COEF_I);

					// Set Register
					if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::HPF_COEF_I, this->FtoS(_COEF, MAX78630Reg::HPF_COEF_I.Data_Type));

					// End Function
					return(_Result);

				} else {

					// End Function
					return(0);

				}

			}

			// Offset Function
			float Offset(const uint8_t _Function, const uint8_t _Type, const uint8_t _Phase, float _Offset) {

				// Control for Type
				if (_Type == MAX78630Def::Current) {

					// Control for _Phase
					if (_Phase == MAX78630Def::Phase_R) {

						// Read Register
						float _Result = this->Register_Pointer_Read(MAX78630Reg::IARMS_OFF);

						// Control for Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::IARMS_OFF, this->FtoS(_Offset, MAX78630Reg::IARMS_OFF.Data_Type));

						// End Function
						return(_Result);

					} else if (_Phase == MAX78630Def::Phase_S) {

						// Read Register
						float _Result = this->Register_Pointer_Read(MAX78630Reg::IBRMS_OFF);

						// Control for Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::IBRMS_OFF, this->FtoS(_Offset, MAX78630Reg::IBRMS_OFF.Data_Type));

						// End Function
						return(_Result);

					} else if (_Phase == MAX78630Def::Phase_T) {

						// Read Register
						float _Result = this->Register_Pointer_Read(MAX78630Reg::ICRMS_OFF);

						// Control for Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::ICRMS_OFF, this->FtoS(_Offset, MAX78630Reg::ICRMS_OFF.Data_Type));

						// End Function
						return(_Result);

					} else {

						// End Function
						return(0);

					}

				} else if (_Type == MAX78630Def::Active_Power) {

					// Control for _Phase
					if (_Phase == MAX78630Def::Phase_R) {

						// Read Register
						float _Result = this->Register_Pointer_Read(MAX78630Reg::PA_OFFS);

						// Control for Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::PA_OFFS, this->FtoS(_Offset, MAX78630Reg::PA_OFFS.Data_Type));

						// End Function
						return(_Result);

					} else if (_Phase == MAX78630Def::Phase_S) {

						// Read Register
						float _Result = this->Register_Pointer_Read(MAX78630Reg::PB_OFFS);

						// Control for Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::PB_OFFS, this->FtoS(_Offset, MAX78630Reg::PB_OFFS.Data_Type));

						// End Function
						return(_Result);

					} else if (_Phase == MAX78630Def::Phase_T) {

						// Read Register
						float _Result = this->Register_Pointer_Read(MAX78630Reg::PC_OFFS);

						// Control for Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::PC_OFFS, this->FtoS(_Offset, MAX78630Reg::PC_OFFS.Data_Type));

						// End Function
						return(_Result);

					} else {

						// End Function
						return(0);

					}

				} else if (_Type == MAX78630Def::ReActive_Power) {

					// Control for _Phase
					if (_Phase == MAX78630Def::Phase_R) {

						// Read Register
						float _Result = this->Register_Pointer_Read(MAX78630Reg::QA_OFFS);

						// Control for Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::QA_OFFS, this->FtoS(_Offset, MAX78630Reg::QA_OFFS.Data_Type));

						// End Function
						return(_Result);

					} else if (_Phase == MAX78630Def::Phase_S) {

						// Read Register
						float _Result = this->Register_Pointer_Read(MAX78630Reg::QB_OFFS);

						// Control for Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::QB_OFFS, this->FtoS(_Offset, MAX78630Reg::QB_OFFS.Data_Type));

						// End Function
						return(_Result);

					} else if (_Phase == MAX78630Def::Phase_T) {

						// Read Register
						float _Result = this->Register_Pointer_Read(MAX78630Reg::QC_OFFS);

						// Control for Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::QC_OFFS, this->FtoS(_Offset, MAX78630Reg::QC_OFFS.Data_Type));

						// End Function
						return(_Result);

					} else {

						// End Function
						return(0);

					}

				} else {

					// End Function
					return(0);

				}

			}

			/* Limit Functions */

			// Limit Function
			float Limit(const uint8_t _Function, const uint8_t _Limit_Type, float _Limit_Value) {

				// Decide Type
				switch (_Limit_Type) {

					case MAX78630Def::VRms_Min: {

						// Read Register
						float _VRMS_MIN = this->Register_Pointer_Read(MAX78630Reg::VRMS_MIN) * MAX78630Config::VScale;

						// Control Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::VRMS_MIN, this->FtoS(_Limit_Value / MAX78630Config::VScale, MAX78630Reg::VRMS_MIN.Data_Type));

						// End Function
						return(_VRMS_MIN);

					}
					case MAX78630Def::VRms_Max: {

						// Read Register
						float _VRMS_MAX = this->Register_Pointer_Read(MAX78630Reg::VRMS_MAX) * MAX78630Config::VScale;

						// Control Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::VRMS_MAX, this->FtoS(_Limit_Value / MAX78630Config::VScale, MAX78630Reg::VRMS_MAX.Data_Type));

						// End Function
						return(_VRMS_MAX);

					}
					case MAX78630Def::IRms_Max: {

						// Read Register
						float _IRMS_MAX = this->Register_Pointer_Read(MAX78630Reg::IRMS_MAX) * MAX78630Config::IScale;

						// Control Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::IRMS_MAX, this->FtoS(_Limit_Value / MAX78630Config::IScale, MAX78630Reg::IRMS_MAX.Data_Type));

						// End Function
						return(_IRMS_MAX);

					}
					case MAX78630Def::F_Min: {

						// Read Register
						float _F_MIN = this->Register_Pointer_Read(MAX78630Reg::F_MIN);

						// Control Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::F_MIN, this->FtoS(_Limit_Value, MAX78630Reg::F_MIN.Data_Type));

						// End Function
						return(_F_MIN);

					}
					case MAX78630Def::F_Max: {

						// Read Register
						float _F_MAX = this->Register_Pointer_Read(MAX78630Reg::F_MAX);

						// Control Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::F_MAX, this->FtoS(_Limit_Value, MAX78630Reg::F_MAX.Data_Type));

						// End Function
						return(_F_MAX);

					}
					case MAX78630Def::T_Min: {

						// Read Register
						float _T_MIN = this->Register_Pointer_Read(MAX78630Reg::T_MIN);

						// Control Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::T_MIN, this->FtoS(_Limit_Value, MAX78630Reg::T_MIN.Data_Type));

						// End Function
						return(_T_MIN);

					}
					case MAX78630Def::T_Max: {

						// Read Register
						float _T_MAX = this->Register_Pointer_Read(MAX78630Reg::T_MAX);

						// Control Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::T_MAX, this->FtoS(_Limit_Value, MAX78630Reg::T_MAX.Data_Type));

						// End Function
						return(_T_MAX);

					}
					case MAX78630Def::PF_Min: {

						// Read Register
						float _PF_MIN = this->Register_Pointer_Read(MAX78630Reg::PF_MIN);

						// Control Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::PF_MIN, this->FtoS(_Limit_Value, MAX78630Reg::PF_MIN.Data_Type));

						// End Function
						return(_PF_MIN);

					}
					case MAX78630Def::VSag_Lim: {

						// Read Register
						float _VSAG_LIM = this->Register_Pointer_Read(MAX78630Reg::VSAG_LIM) * MAX78630Config::VScale;

						// Control Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::VSAG_LIM, this->FtoS(_Limit_Value / MAX78630Config::VScale, MAX78630Reg::VSAG_LIM.Data_Type));

						// End Function
						return(_VSAG_LIM);

					}
					case MAX78630Def::VImb_Max: {

						// Read Register
						float _V_IMB_MAX = this->Register_Pointer_Read(MAX78630Reg::V_IMB_MAX);

						// Control Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::V_IMB_MAX, this->FtoS(_Limit_Value, MAX78630Reg::V_IMB_MAX.Data_Type));

						// End Function
						return(_V_IMB_MAX);

					}
					case MAX78630Def::IImb_Max: {

						// Read Register
						float _I_IMB_MAX = this->Register_Pointer_Read(MAX78630Reg::I_IMB_MAX);

						// Control Function
						if (_Function == MAX78630Def::SET) this->Register_Pointer_Set(MAX78630Reg::I_IMB_MAX, this->FtoS(_Limit_Value, MAX78630Reg::I_IMB_MAX.Data_Type));

						// End Function
						return(_I_IMB_MAX);

					}
					default: {

						// End Function
						return(0);

					}

				}

			}

		// Public Context
		public:

			// Constructor
			explicit MAX78630(Stream &_Serial) {

				// Set Serial Port
				Serial_Energy = & _Serial;

			}

			// Begin && Set Start Parameters.
			bool Begin(const uint8_t _Address = 4, const bool _Scale = true, const bool _Bucket = true, const bool _Limit = true, const bool _MinMax = true, const bool _Alarm = true) {

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
					if (this->Firmware() == MAX78630Config::Firmware) {

						// Get Status
						this->Status(MAX78630Def::GET);

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

				// Control for Device
				if (this->Device.Found) {

					// Set Scale
					if (_Scale) {

						// Set Voltage Scale
						this->VScale(MAX78630Def::SET);

						// Set Current Scale
						this->IScale(MAX78630Def::SET);

					}

					// Set Bucket
					if (_Bucket) {

						// Set Bucket Size
						this->Bucket_H(MAX78630Def::SET);
						this->Bucket_L(MAX78630Def::SET);

					}

					// Set Limit
					if (_Limit) {

						// Set Voltage Min Limit
						this->Limit(MAX78630Def::SET, MAX78630Def::VRms_Min, this->Limit_Variables.V_Min);

						// Set Voltage Max Limit
						this->Limit(MAX78630Def::SET, MAX78630Def::VRms_Max, this->Limit_Variables.V_Max);

						// Set Current Max Limit
						this->Limit(MAX78630Def::SET, MAX78630Def::IRms_Max, this->Limit_Variables.I_Max);

						// Set Frequency Min Limit
						this->Limit(MAX78630Def::SET, MAX78630Def::F_Min, this->Limit_Variables.FQ_Min);

						// Set Frequency Max Limit
						this->Limit(MAX78630Def::SET, MAX78630Def::F_Max, this->Limit_Variables.FQ_Max);

						// Set Voltage Imbalance Max Limit
						this->Limit(MAX78630Def::SET, MAX78630Def::VImb_Max, this->Limit_Variables.VImb_Max);

						// Set Current Imbalance Max Limit
						this->Limit(MAX78630Def::SET, MAX78630Def::IImb_Max, this->Limit_Variables.IImb_Max);

						// Set Temperature Min Limit
						this->Limit(MAX78630Def::SET, MAX78630Def::T_Min, this->Limit_Variables.T_Min);

						// Set Temperature Max Limit
						this->Limit(MAX78630Def::SET, MAX78630Def::T_Max, this->Limit_Variables.T_Max);

					}

					// Set Min Max
					if (_MinMax) {

						// Set Min/Max Monitor Address 1
						this->Set_Min_Max_Mask(MAX78630Def::Monitor1);

						// Set Min/Max Monitor Address 2
						this->Set_Min_Max_Mask(MAX78630Def::Monitor2);

						// Set Min/Max Monitor Address 3
						this->Set_Min_Max_Mask(MAX78630Def::Monitor3);

						// Set Min/Max Monitor Address 4
						this->Set_Min_Max_Mask(MAX78630Def::Monitor4);

						// Set Min/Max Monitor Address 5
						this->Set_Min_Max_Mask(MAX78630Def::Monitor5);

						// Set Min/Max Monitor Address 6
						this->Set_Min_Max_Mask(MAX78630Def::Monitor6);

						// Set Min/Max Monitor Address 7
						this->Set_Min_Max_Mask(MAX78630Def::Monitor7);

						// Set Min/Max Monitor Address 8
						this->Set_Min_Max_Mask(MAX78630Def::Monitor8);

					}

					// Set Alarm
					if (_Alarm) {

						// Set Sticky
						this->Sticky(MAX78630Def::SET, MAX78630Config::StatusSticky);

						// Set High Voltage Alarm --> AL1
						this->Alarm_Mask(MAX78630Def::SET, MAX78630Def::Alarm1, MAX78630Def::Bit_OV_VrmsA, true);
						this->Alarm_Mask(MAX78630Def::SET, MAX78630Def::Alarm1, MAX78630Def::Bit_OV_VrmsB, true);
						this->Alarm_Mask(MAX78630Def::SET, MAX78630Def::Alarm1, MAX78630Def::Bit_OV_VrmsC, true);

						// Set Low Voltage Alarm --> AL1
						this->Alarm_Mask(MAX78630Def::SET, MAX78630Def::Alarm1, MAX78630Def::Bit_UN_VrmsA, true);
						this->Alarm_Mask(MAX78630Def::SET, MAX78630Def::Alarm1, MAX78630Def::Bit_UN_VrmsB, true);
						this->Alarm_Mask(MAX78630Def::SET, MAX78630Def::Alarm1, MAX78630Def::Bit_UN_VrmsC, true);

						// Set High Frequency Alarm --> AL1
						this->Alarm_Mask(MAX78630Def::SET, MAX78630Def::Alarm1, MAX78630Def::Bit_OV_Freq, true);

						// Set Low Frequency Alarm --> AL1
						this->Alarm_Mask(MAX78630Def::SET, MAX78630Def::Alarm1, MAX78630Def::Bit_UN_Freq, true);

						// Set Voltage Imbalance Alarm --> AL2
						this->Alarm_Mask(MAX78630Def::SET, MAX78630Def::Alarm2, MAX78630Def::Bit_V_Imbal, true);

						// Set Current Imbalance Alarm --> AL2
						this->Alarm_Mask(MAX78630Def::SET, MAX78630Def::Alarm2, MAX78630Def::Bit_I_Imbal, true);

						// Set Low Power Factor Alarm --> AL2
						this->Alarm_Mask(MAX78630Def::SET, MAX78630Def::Alarm2, MAX78630Def::Bit_UN_PFA, true);
						this->Alarm_Mask(MAX78630Def::SET, MAX78630Def::Alarm2, MAX78630Def::Bit_UN_PFB, true);
						this->Alarm_Mask(MAX78630Def::SET, MAX78630Def::Alarm2, MAX78630Def::Bit_UN_PFC, true);

						// Set High Temperature Alarm --> AL2
						this->Alarm_Mask(MAX78630Def::SET, MAX78630Def::Alarm2, MAX78630Def::Bit_OV_Temp, true);

						// Set Low Temperature Alarm --> AL2
						this->Alarm_Mask(MAX78630Def::SET, MAX78630Def::Alarm2, MAX78630Def::Bit_UN_Temp, true);

					}

				}

				// End Function
				return(this->Device.Found);

			}

			/* IC Utility Functions */

			// Status Function
			uint32_t Status(const uint8_t _Function = MAX78630Def::GET) {

				// Return Register
				this->Device.Status = this->Register_Pointer_Read(MAX78630Reg::STATUS);

				// Control for Function
				if (_Function == MAX78630Def::CLEAR) this->Register_Pointer_Set(MAX78630Reg::STATUS_CLEAR, 0x00800003);

				// End Function
				return(this->Device.Status);

			}

			/* Measurement Functions */

			// Voltage Measurement Function.
			float Voltage(const uint8_t _Phase, const uint8_t _Type) {

				// Decide Phase
				switch (_Phase) {

					// Phase R
					case MAX78630Def::Phase_R: {

						// Decide Type
						switch (_Type) {

							// RMS Voltage
							case MAX78630Def::RMS: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VA_RMS) * MAX78630Config::VScale);


							}

							// Instantaneous Voltage
							case MAX78630Def::Instant: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VA) * MAX78630Config::VScale);


							}

							// Fundamental Voltage
							case MAX78630Def::Fundamental: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VFUND_A) * MAX78630Config::VScale);


							}

							// Harmonic Voltage
							case MAX78630Def::Harmonic: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VHARM_A) * MAX78630Config::VScale);

							}

						}


					}
					case MAX78630Def::Phase_S: {

						// Decide Type
						switch (_Type) {

							// RMS Voltage
							case MAX78630Def::RMS: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VB_RMS) * MAX78630Config::VScale);


							}

							// Instantaneous Voltage
							case MAX78630Def::Instant: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VB) * MAX78630Config::VScale);


							}

							// Fundamental Voltage
							case MAX78630Def::Fundamental: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VFUND_B) * MAX78630Config::VScale);


							}

							// Harmonic Voltage
							case MAX78630Def::Harmonic: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VHARM_B) * MAX78630Config::VScale);


							}

						}


					}
					case MAX78630Def::Phase_T: {

						// Decide Type
						switch (_Type) {

							// RMS Voltage
							case MAX78630Def::RMS: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VC_RMS) * MAX78630Config::VScale);


							}

							// Instantaneous Voltage
							case MAX78630Def::Instant: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VC) * MAX78630Config::VScale);


							}

							// Fundamental Voltage
							case MAX78630Def::Fundamental: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VFUND_C) * MAX78630Config::VScale);


							}

							// Harmonic Voltage
							case MAX78630Def::Harmonic: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VHARM_C) * MAX78630Config::VScale);


							}

						}


					}
					case MAX78630Def::Phase_Avg: {

						// Get Register
						return(this->Register_Pointer_Read(MAX78630Reg::VT_RMS) * MAX78630Config::VScale);


					}

				}

				return(0.0f);

			}

			// Current Measurement Function.
			float Current(const uint8_t _Phase, const uint8_t _Type) {

				// Decide Phase
				switch (_Phase) {

					// Phase R
					case MAX78630Def::Phase_R: {

						// Decide Type
						switch (_Type) {

							// RMS Current
							case MAX78630Def::RMS: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::IA_RMS) * MAX78630Config::IScale);


							}

							// Instantaneous Current
							case MAX78630Def::Instant: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::IA) * MAX78630Config::IScale);


							}

							// Fundamental Current
							case MAX78630Def::Fundamental: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::IFUND_A) * MAX78630Config::IScale);


							}

							// Harmonic Current
							case MAX78630Def::Harmonic: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::IHARM_A) * MAX78630Config::IScale);


							}

							// Peak Current
							case MAX78630Def::Peak: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::IA_PEAK) * MAX78630Config::IScale);


							}

							// Phase Compensation
							case MAX78630Def::Phase_Compensation: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::PHASECOMP1) * MAX78630Config::IScale);


							}

						}


					}

					// Phase S
					case MAX78630Def::Phase_S: {

						// Decide Type
						switch (_Type) {

							// RMS Current
							case MAX78630Def::RMS: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::IB_RMS) * MAX78630Config::IScale);


							}

							// Instantaneous Current
							case MAX78630Def::Instant: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::IB) * MAX78630Config::IScale);


							}

							// Fundamental Current
							case MAX78630Def::Fundamental: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::IFUND_B) * MAX78630Config::IScale);


							}

							// Harmonic Current
							case MAX78630Def::Harmonic: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::IHARM_B) * MAX78630Config::IScale);


							}

							// Peak Current
							case MAX78630Def::Peak: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::IB_PEAK) * MAX78630Config::IScale);


							}

							// Phase Compensation
							case MAX78630Def::Phase_Compensation: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::PHASECOMP2) * MAX78630Config::IScale);


							}

						}


					}

					// Phase T
					case MAX78630Def::Phase_T: {

						// Decide Type
						switch (_Type) {

							// RMS Current
							case MAX78630Def::RMS: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::IC_RMS) * MAX78630Config::IScale);


							}

							// Instantaneous Current
							case MAX78630Def::Instant: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::IC) * MAX78630Config::IScale);


							}

							// Fundamental Current
							case MAX78630Def::Fundamental: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::IFUND_C) * MAX78630Config::IScale);


							}

							// Harmonic Current
							case MAX78630Def::Harmonic: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::IHARM_C) * MAX78630Config::IScale);


							}

							// Peak Current
							case MAX78630Def::Peak: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::IC_PEAK) * MAX78630Config::IScale);


							}

							// Phase Compensation
							case MAX78630Def::Phase_Compensation: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::PHASECOMP3) * MAX78630Config::IScale);


							}

						}


					}

					// Phase Average
					case MAX78630Def::Phase_Avg: {

						// Get Register
						return(this->Register_Pointer_Read(MAX78630Reg::IT_RMS) * MAX78630Config::IScale);


					}

				}

				return(0.0f);

			}

			// Frequency Measurement Function.
			float Frequency(void) {

				// Return Register
				return(this->Register_Pointer_Read(MAX78630Reg::FREQ));

			}

			// Power Factor Measurement Function.
			float Power_Factor(const uint8_t _Phase) {

				// Decide Phase
				switch (_Phase) {

					case MAX78630Def::Phase_R: {

						// Return Register
						return(this->Register_Pointer_Read(MAX78630Reg::PFA));


					}
					case MAX78630Def::Phase_S: {

						// Return Register
						return(this->Register_Pointer_Read(MAX78630Reg::PFB));


					}
					case MAX78630Def::Phase_T: {

						// Return Register
						return(this->Register_Pointer_Read(MAX78630Reg::PFC));


					}
					case MAX78630Def::Phase_Avg: {

						// Return Register
						return(this->Register_Pointer_Read(MAX78630Reg::PF_T));


					}

				}

				return(0.0f);

			}

			// Power Measurement Function.
			float Power(const uint8_t _Phase, const uint8_t _Type) {

				// Decide Phase
				switch (_Phase) {

					case MAX78630Def::Phase_R: {

						// Decide Type
						switch (_Type) {

							case MAX78630Def::Active: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::WATT_A) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::ReActive: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VAR_A) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Apparent: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VA_A) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Fundamental: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::PFUND_A) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Harmonic: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::PHARM_A) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Fund_ReActive: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::QFUND_A) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Harm_ReActive: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::QHARM_A) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Fund_VA: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VAFUND_A) * MAX78630Config::VScale * MAX78630Config::IScale);


							}

						}


					}
					case MAX78630Def::Phase_S: {

						// Decide Type
						switch (_Type) {

							case MAX78630Def::Active: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::WATT_B) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::ReActive: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VAR_B) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Apparent: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VA_B) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Fundamental: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::PFUND_B) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Harmonic: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::PHARM_B) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Fund_ReActive: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::QFUND_B) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Harm_ReActive: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::QHARM_B) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Fund_VA: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VAFUND_B) * MAX78630Config::VScale * MAX78630Config::IScale);


							}

						}


					}
					case MAX78630Def::Phase_T: {

						// Decide Type
						switch (_Type) {

							case MAX78630Def::Active: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::WATT_C) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::ReActive: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VAR_C) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Apparent: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VA_C) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Fundamental: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::PFUND_C) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Harmonic: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::PHARM_C) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Fund_ReActive: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::QFUND_C) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Harm_ReActive: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::QHARM_C) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Fund_VA: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VAFUND_C) * MAX78630Config::VScale * MAX78630Config::IScale);


							}

						}


					}
					case MAX78630Def::Phase_Avg: {

						// Decide Type
						switch (_Type) {

							case MAX78630Def::Active: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::WATT_T) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::ReActive: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VAR_T) * MAX78630Config::VScale * MAX78630Config::IScale);


							}
							case MAX78630Def::Apparent: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VA_T) * MAX78630Config::VScale * MAX78630Config::IScale);


							}

						}


					}

				}

				return(0.0f);

			}

			// Energy Measurement Function.
			float Energy(const uint8_t _Phase, const uint8_t _Type) {

				// Decide Phase
				switch (_Phase) {

					case MAX78630Def::Phase_R: {

						// Decide Type
						switch (_Type) {

							case MAX78630Def::Active_Received: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::WHA_POS));


							}
							case MAX78630Def::Active_Delivered: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::WHA_NEG));


							}
							case MAX78630Def::ReActive_Received: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VARHA_POS));


							}
							case MAX78630Def::ReActive_Delivered: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VARHA_NEG));


							}
							case MAX78630Def::Energy_Reset: {

								// Clear Register
								this->Register_Pointer_Set(MAX78630Reg::WHA_POS, 0x00);
								this->Register_Pointer_Set(MAX78630Reg::VARHA_POS, 0x00);
								this->Register_Pointer_Set(MAX78630Reg::WHA_NEG, 0x00);
								this->Register_Pointer_Set(MAX78630Reg::VARHA_NEG, 0x00);

								// End Function
								return(1);


							}

						}


					}
					case MAX78630Def::Phase_S: {

						// Decide Type
						switch (_Type) {

							case MAX78630Def::Active_Received: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::WHB_POS));


							}
							case MAX78630Def::Active_Delivered: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::WHB_NEG));


							}
							case MAX78630Def::ReActive_Received: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VARHB_POS));


							}
							case MAX78630Def::ReActive_Delivered: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VARHB_NEG));


							}
							case MAX78630Def::Energy_Reset: {

								// Clear Register
								this->Register_Pointer_Set(MAX78630Reg::WHB_POS, 0x00);
								this->Register_Pointer_Set(MAX78630Reg::VARHB_POS, 0x00);
								this->Register_Pointer_Set(MAX78630Reg::WHB_NEG, 0x00);
								this->Register_Pointer_Set(MAX78630Reg::VARHB_NEG, 0x00);

								// End Function
								return(1);


							}

						}


					}
					case MAX78630Def::Phase_T: {

						// Decide Type
						switch (_Type) {

							case MAX78630Def::Active_Received: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::WHC_POS));


							}
							case MAX78630Def::Active_Delivered: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::WHC_NEG));


							}
							case MAX78630Def::ReActive_Received: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VARHC_POS));


							}
							case MAX78630Def::ReActive_Delivered: {

								// Get Register
								return(this->Register_Pointer_Read(MAX78630Reg::VARHC_NEG));


							}
							case MAX78630Def::Energy_Reset: {

								// Clear Register
								this->Register_Pointer_Set(MAX78630Reg::WHC_POS, 0x00);
								this->Register_Pointer_Set(MAX78630Reg::VARHC_POS, 0x00);
								this->Register_Pointer_Set(MAX78630Reg::WHC_NEG, 0x00);
								this->Register_Pointer_Set(MAX78630Reg::VARHC_NEG, 0x00);

								// End Function
								return(1);


							}

						}


					}

				}

				return(0.0f);

			}

			// IC Temperature Measurement Function.
			float Temperature(void) {

				// Return Register
				return(this->Register_Pointer_Read(MAX78630Reg::TEMPC));

			}

			// Set Harmonic Channel Function. TODO: Not Tested.
			uint8_t Harmonic(const uint8_t _Function, uint32_t _Harmonic) {

				// Control for Function
				if (_Function == MAX78630Def::GET) {

					// Read Register
					return(static_cast<uint8_t>(this->Register_Pointer_Read(MAX78630Reg::HARM)));

				} else if (_Function == MAX78630Def::SET) {

					// Set Register
					return(static_cast<uint32_t>(this->Register_Pointer_Set(MAX78630Reg::HARM, _Harmonic)));

				} else {

					// End Function
					return(0);

				}

			}

			/* Min Max Functions */

			// Min Record Value Function
			float Min_Value(const uint8_t _Function, const uint8_t _MM_ADDR, const uint16_t _Scale = 1) {

				if (_Function == MAX78630Def::SET || _MM_ADDR == 0 || _MM_ADDR > 8) return(0.0f);

				static const Register _Regs[8] = {
					MAX78630Reg::MIN0, MAX78630Reg::MIN1, MAX78630Reg::MIN2, MAX78630Reg::MIN3,
					MAX78630Reg::MIN4, MAX78630Reg::MIN5, MAX78630Reg::MIN6, MAX78630Reg::MIN7
				};

				return(this->MM_Read(_Regs[_MM_ADDR - 1], _Scale));

			}

			// Max Record Value Function
			float Max_Value(const uint8_t _Function, const uint8_t _MM_ADDR, const uint16_t _Scale = 1) {

				if (_Function == MAX78630Def::SET || _MM_ADDR == 0 || _MM_ADDR > 8) return(0.0f);

				static const Register _Regs[8] = {
					MAX78630Reg::MAX0, MAX78630Reg::MAX1, MAX78630Reg::MAX2, MAX78630Reg::MAX3,
					MAX78630Reg::MAX4, MAX78630Reg::MAX5, MAX78630Reg::MAX6, MAX78630Reg::MAX7
				};

				return(this->MM_Read(_Regs[_MM_ADDR - 1], _Scale));

			}

			/* Limit Functions */

			// Adjust Limit Parameters Function.
			void Adjust_Limit(void) {

				// Control for High Temperature
				if (this->Alarm(MAX78630Def::Alarm_HighTemp)) {

					// Set Limit
					this->Limit(MAX78630Def::SET, MAX78630Def::T_Max, (this->Limit_Variables.T_Max - MAX78630Config::TempMaxDiff));

				} else {

					// Set Limit
					this->Limit(MAX78630Def::SET, MAX78630Def::T_Max, this->Limit_Variables.T_Max);

				}

				// Control for Low Temperature
				if (this->Alarm(MAX78630Def::Alarm_LowTemp)) {

					// Set Limit
					this->Limit(MAX78630Def::SET, MAX78630Def::T_Min, (this->Limit_Variables.T_Min + MAX78630Config::TempMinDiff));

				} else {

					// Set Limit
					this->Limit(MAX78630Def::SET, MAX78630Def::T_Min, this->Limit_Variables.T_Min);

				}

				// Control for High Voltage
				if (this->Alarm(MAX78630Def::Alarm_HighVoltage)) {

					// Set Limit
					this->Limit(MAX78630Def::SET, MAX78630Def::VRms_Max, (this->Limit_Variables.V_Max - MAX78630Config::VoltageMaxDiff));

				} else {

					// Set Limit
					this->Limit(MAX78630Def::SET, MAX78630Def::VRms_Max, this->Limit_Variables.V_Max);

				}

				// Control for Low Voltage
				if (this->Alarm(MAX78630Def::Alarm_LowVoltage)) {

					// Set Limit
					this->Limit(MAX78630Def::SET, MAX78630Def::VRms_Min, (this->Limit_Variables.V_Min + MAX78630Config::VoltageMinDiff));

				} else {

					// Set Limit
					this->Limit(MAX78630Def::SET, MAX78630Def::VRms_Min, this->Limit_Variables.V_Min);

				}

				// Control for High Frequency
				if (this->Alarm(MAX78630Def::Alarm_HighFreq)) {

					// Set Limit
					this->Limit(MAX78630Def::SET, MAX78630Def::F_Max, (this->Limit_Variables.FQ_Max - MAX78630Config::FreqMaxDiff));

				} else {

					// Set Limit
					this->Limit(MAX78630Def::SET, MAX78630Def::F_Max, this->Limit_Variables.FQ_Max);

				}

				// Control for Low Frequency
				if (this->Alarm(MAX78630Def::Alarm_LowFreq)) {

					// Set Limit
					this->Limit(MAX78630Def::SET, MAX78630Def::F_Min, (this->Limit_Variables.FQ_Min + MAX78630Config::FreqMinDiff));

				} else {

					// Set Limit
					this->Limit(MAX78630Def::SET, MAX78630Def::F_Min, this->Limit_Variables.FQ_Min);

				}

				// Control for Voltage Imbalance
				if (this->Alarm(MAX78630Def::Alarm_VImbal)) {

					// Set Limit
					this->Limit(MAX78630Def::SET, MAX78630Def::VImb_Max, (this->Limit_Variables.VImb_Max + MAX78630Config::VImbMaxDiff));

				} else {

					// Set Limit
					this->Limit(MAX78630Def::SET, MAX78630Def::VImb_Max, this->Limit_Variables.VImb_Max);

				}

				// Control for Current Imbalance
				if (this->Alarm(MAX78630Def::Alarm_IImbal)) {

					// Set Limit
					this->Limit(MAX78630Def::SET, MAX78630Def::IImb_Max, (this->Limit_Variables.IImb_Max + MAX78630Config::IImbMaxDiff));

				} else {

					// Set Limit
					this->Limit(MAX78630Def::SET, MAX78630Def::IImb_Max, this->Limit_Variables.IImb_Max);

				}

			}

			/* Alarm Functions */

			// Get Alarm Status Function.
			bool Alarm(const uint16_t _Alarm_Type) {

				// Get Device Status
				this->Status(MAX78630Def::GET);

				// Control for Alarm Type
				switch (_Alarm_Type) {

					// High Voltage Alarm
					case MAX78630Def::Alarm_HighVoltage : {

						// Control for Over Voltage
						if (bitRead(this->Device.Status, MAX78630Def::Bit_OV_VrmsA) || bitRead(this->Device.Status, MAX78630Def::Bit_OV_VrmsB) || bitRead(this->Device.Status, MAX78630Def::Bit_OV_VrmsC)) return(true);

						// End Switch
						break;

					}

					// Low Voltage Alarm
					case MAX78630Def::Alarm_LowVoltage : {

						// Control for Under Voltage
						if (bitRead(this->Device.Status, MAX78630Def::Bit_UN_VrmsA) || bitRead(this->Device.Status, MAX78630Def::Bit_UN_VrmsB) || bitRead(this->Device.Status, MAX78630Def::Bit_UN_VrmsC)) return(true);

						// End Switch
						break;

					}

					// High Current Alarm
					case MAX78630Def::Alarm_HighCurrent : {

						// Control for Over Current
						if (bitRead(this->Device.Status, MAX78630Def::Bit_OV_IrmsA) || bitRead(this->Device.Status, MAX78630Def::Bit_OV_IrmsB) || bitRead(this->Device.Status, MAX78630Def::Bit_OV_IrmsC)) return(true);

						// End Switch
						break;

					}

					// High Frequency Alarm
					case MAX78630Def::Alarm_HighFreq : {

						// Control for Over Frequency
						if (bitRead(this->Device.Status, MAX78630Def::Bit_OV_Freq)) return(true);

						// End Switch
						break;

					}

					// Low Frequency Alarm
					case MAX78630Def::Alarm_LowFreq : {

						// Control for Under Frequency
						if (bitRead(this->Device.Status, MAX78630Def::Bit_UN_Freq)) return(true);

						// End Switch
						break;

					}

					// Voltage Imbalance Alarm
					case MAX78630Def::Alarm_VImbal : {

						// Control for Voltage Imbalance
						if (bitRead(this->Device.Status, MAX78630Def::Bit_V_Imbal)) return(true);

						// End Switch
						break;

					}

					// Current Imbalance Alarm
					case MAX78630Def::Alarm_IImbal : {

						// Control for Current Imbalance
						if (bitRead(this->Device.Status, MAX78630Def::Bit_I_Imbal)) return(true);

						// End Switch
						break;

					}

					// Low Power Factor Alarm
					case MAX78630Def::Alarm_LowPF : {

						// Control for Low Power Factor
						if (bitRead(this->Device.Status, MAX78630Def::Bit_UN_PFA) || bitRead(this->Device.Status, MAX78630Def::Bit_UN_PFB) || bitRead(this->Device.Status, MAX78630Def::Bit_UN_PFC)) return(true);

						// End Switch
						break;

					}

					// High Temperature Alarm
					case MAX78630Def::Alarm_HighTemp : {

						// Control for Over Temperature
						if (bitRead(this->Device.Status, MAX78630Def::Bit_OV_Temp)) return(true);

						// End Switch
						break;

					}

					// Low Temperature Alarm
					case MAX78630Def::Alarm_LowTemp : {

						// Control for Under Temperature
						if (bitRead(this->Device.Status, MAX78630Def::Bit_UN_Temp)) return(true);

						// End Switch
						break;

					}

				}

				// End Function
				return(false);

			}

	};

#endif /* defined(__MAX78630__) */
