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
		 * @brief IC Register Address Definitions
		 */
		struct Register_Struct {
			Register COMMAND 		{0x00, 		0x00, 		0};			// Selects modes, functions, or options
			Register FW_VERSION 	{0x00, 		0x03, 		0};			// Hardware and firmware version
			Register CONFIG 		{0x00, 		0x06, 		0};			// Selects input configuration
			Register SAMPLES 		{0x00, 		0x09, 		0};			// Minimum high-rate samples per accumulation interval
			Register DIVISOR 		{0x00, 		0x0C, 		0};			// Actual samples in previous accumulation interval
			Register CYCLE 			{0x00, 		0x0F, 		0};			// High-rate sample counter
			Register FRAME 			{0x00, 		0x12, 		0};			// Low-rate sample counter
			Register STATUS 		{0x00, 		0x15, 		0};			// Alarm and device status bits
			Register STATUS_CLEAR 	{0x00, 		0x18, 		0};			// Used to reset alarm/status bits
			Register STATUS_SET 	{0x00, 		0x1B, 		0};			// Used to set/force alarm/status bits
			Register MASK1 			{0x00, 		0x1E, 		0};			// Alarm/status mask for AL1 output pin
			Register MASK2 			{0x00, 		0x21, 		0};			// Alarm/status mask for AL2 output pin
			Register MASK3 			{0x00, 		0x24, 		0};			// Alarm/status mask for AL3 output pin
			Register MASK4 			{0x00, 		0x27, 		0};			// Alarm/status mask for AL4 output pin
			Register MASK5 			{0x00, 		0x2A, 		0};			// Alarm/status mask for AL5 output pin
			Register STICKY 		{0x00, 		0x2D, 		0};			// Alarm/status bits to hold until cleared by host
			Register DIO_STATE 		{0x00, 		0x30, 		0};			// State of DIO pins
			Register DIO_DIR 		{0x00, 		0x33, 		0};			// Direction of DIO pins. 1=Input ; 0=Output
			Register DIO_POL 		{0x00, 		0x36, 		0};			// Polarity of DIO pins. 1=Active High ; 0=Active Low
			Register CALCYCS 		{0x00, 		0x39, 		0};			// Number of calibration cycles to average
			Register HPF_COEF_I 	{0x00, 		0x3C, 		23};		// Current input HPF coefficient. Positive values only
			Register HPF_COEF_V 	{0x00, 		0x3F, 		23};		// Voltage input HPF coefficient. Positive values only
			Register PHASECOMP1 	{0x00, 		0x42, 		21};		// Phase compensation (±4 samples) for AV1 input
			Register PHASECOMP2 	{0x00, 		0x45, 		21};		// Phase compensation (±4 samples) for AV2 input
			Register PHASECOMP3 	{0x00, 		0x48, 		21};		// Phase compensation (±4 samples) for AV3 input
			Register HARM 			{0x00, 		0x4B, 		0};			// Harmonic Selector, default: 1 (fundamental)
			Register DEVADDR 		{0x00, 		0x4E, 		0};			// High order address bits for I2C and UART interfaces
			Register BAUD 			{0x00, 		0x51, 		0};			// Baud rate for UART interface
			Register I1_GAIN 		{0x00, 		0x54, 		21};		// Current Gain Calibration.Positive values only
			Register I2_GAIN 		{0x00, 		0x57, 		21};		// Current Gain Calibration.Positive values only
			Register I3_GAIN 		{0x00, 		0x5A, 		21};		// Current Gain Calibration.Positive values only
			Register V1_GAIN 		{0x00, 		0x5D, 		21};		// Voltage Gain Calibration. Positive values only
			Register V2_GAIN 		{0x00, 		0x60, 		21};		// Voltage Gain Calibration. Positive values only
			Register V3_GAIN 		{0x00, 		0x63, 		21};		// Voltage Gain Calibration. Positive values only
			Register I1_OFFS 		{0x00, 		0x66, 		23};		// Current Offset Calibration
			Register I2_OFFS 		{0x00, 		0x69, 		23};		// Current Offset Calibration
			Register I3_OFFS 		{0x00, 		0x6C, 		23};		// Current Offset Calibration
			Register V1_OFFS 		{0x00, 		0x6F, 		23};		// Voltage Offset Calibration
			Register V2_OFFS 		{0x00, 		0x72, 		23};		// Voltage Offset Calibration
			Register V3_OFFS 		{0x00, 		0x75, 		23};		// Voltage Offset Calibration
			Register T_GAIN 		{0x00, 		0x78, 		0};			// Temperature Slope Calibration
			Register T_OFFS 		{0x00, 		0x7B, 		0};			// Temperature Offset Calibration
			Register VSAG_INT 		{0x00, 		0x7E, 		0};			// Voltage sag detect interval (high-rate samples)
			Register V_IMB_MAX 		{0x00, 		0x81, 		23};		// Voltage imbalance alarm limit. Positive values only
			Register I_IMB_MAX 		{0x00, 		0x84, 		23};		// Current imbalance alarm limit. Positive values only
			Register VA 			{0x00, 		0x87, 		23};		// Instantaneous Voltage
			Register VB 			{0x00, 		0x8A, 		23};		// Instantaneous Voltage
			Register VC 			{0x00, 		0x8D, 		23};		// Instantaneous Voltage
			Register VA_RMS 		{0x00, 		0x90, 		23};		// RMS Voltage
			Register VB_RMS 		{0x00, 		0x93, 		23};		// RMS Voltage
			Register VC_RMS 		{0x00, 		0x96, 		23};		// RMS Voltage
			Register VT_RMS 		{0x00, 		0x99, 		23};		// RMS Voltage average (Total / 3)
			Register VFUND_A 		{0x00, 		0x9C, 		23};		// Fundamental Voltage
			Register VFUND_B 		{0x00, 		0x9F, 		23};		// Fundamental Voltage
			Register VFUND_C 		{0x00, 		0xA2, 		23};		// Fundamental Voltage
			Register VHARM_A 		{0x00, 		0xA5, 		23};		// Harmonic Voltage
			Register VHARM_B 		{0x00, 		0xA8, 		23};		// Harmonic Voltage
			Register VHARM_C 		{0x00, 		0xAB, 		23};		// Harmonic Voltage
			Register V_TARGET 		{0x00, 		0xAE, 		23};		// Calibration Target for Voltages. Positive values only
			Register VRMS_MIN 		{0x00, 		0xB1, 		23};		// Voltage lower alarm limit. Positive values only
			Register VRMS_MAX 		{0x00, 		0xB4, 		23};		// Voltage upper alarm limit. Positive values only
			Register VSAG_LIM 		{0x00, 		0xB7, 		23};		// RMS Voltage Sag threshold. Positive values only
			Register IA 			{0x00, 		0xBA, 		23};		// Instantaneous Current
			Register IB 			{0x00, 		0xBD, 		23};		// Instantaneous Current
			Register IC 			{0x00, 		0xC0, 		23};		// Instantaneous Current
			Register IARMS_OFF 		{0x00, 		0xC3, 		23};		// RMS Current dynamic offset adjust. Positive values only
			Register IBRMS_OFF 		{0x00, 		0xC6, 		23};		// RMS Current dynamic offset adjust. Positive values only
			Register ICRMS_OFF 		{0x00, 		0xC9, 		23};		// RMS Current dynamic offset adjust. Positive values only
			Register IA_PEAK 		{0x00, 		0xCC, 		23};		// Peak Current
			Register IB_PEAK 		{0x00, 		0xCF, 		23};		// Peak Current
			Register IC_PEAK 		{0x00, 		0xD2, 		23};		// Peak Current
			Register IA_RMS 		{0x00, 		0xD5, 		23};		// RMS Current
			Register IB_RMS 		{0x00, 		0xD8, 		23};		// RMS Current
			Register IC_RMS 		{0x00, 		0xDB, 		23};		// RMS Current
			Register IT_RMS 		{0x00, 		0xDE, 		23};		// RMS Current average (Total / 3)
			Register IFUND_A 		{0x00, 		0xE1, 		23};		// Fundamental Current
			Register IFUND_B 		{0x00, 		0xE4, 		23};		// Fundamental Current
			Register IFUND_C 		{0x00, 		0xE7, 		23};		// Fundamental Current
			Register IHARM_A 		{0x00, 		0xEA, 		23};		// Harmonic Current
			Register IHARM_B 		{0x00, 		0xED, 		23};		// Harmonic Current
			Register IHARM_C 		{0x00, 		0xF0, 		23};		// Harmonic Current
			Register IRMS_MAX 		{0x00, 		0xF3, 		23};		// Current upper alarm limit. Positive values only
			Register I_TARGET 		{0x00, 		0xF6, 		23};		// Calibration Target for Currents. Positive values only
			Register QFUND_A 		{0x00, 		0xF9, 		23};		// Fundamental Reactive Power
			Register QFUND_B 		{0x00, 		0xFC, 		23};		// Fundamental Reactive Power
			Register QFUND_C 		{0x00, 		0xFF, 		23};		// Fundamental Reactive Power
			Register QHARM_A 		{0x01, 		0x02, 		23};		// Harmonic Reactive Power
			Register QHARM_B 		{0x01, 		0x05, 		23};		// Harmonic Reactive Power
			Register QHARM_C 		{0x01, 		0x08, 		23};		// Harmonic Reactive Power
			Register QA_OFFS 		{0x01, 		0x0B, 		23};		// Reactive Power dynamic offset adjust. Positive values only
			Register QB_OFFS 		{0x01, 		0x0E, 		23};		// Reactive Power dynamic offset adjust. Positive values only
			Register QC_OFFS 		{0x01, 		0x11, 		23};		// Reactive Power dynamic offset adjust. Positive values only
			Register PA_OFFS 		{0x01, 		0x14, 		23};		// Active Power dynamic offset adjust. Positive values only
			Register PB_OFFS 		{0x01, 		0x17, 		23};		// Active Power dynamic offset adjust. Positive values only
			Register PC_OFFS 		{0x01, 		0x1A, 		23};		// Active Power dynamic offset adjust. Positive values only
			Register WATT_A 		{0x01, 		0x1D, 		23};		// Active Power
			Register WATT_B 		{0x01, 		0x20, 		23};		// Active Power
			Register WATT_C 		{0x01, 		0x23, 		23};		// Active Power
			Register VAR_A 			{0x01, 		0x26, 		23};		// Reactive Power
			Register VAR_B 			{0x01, 		0x29, 		23};		// Reactive Power
			Register VAR_C 			{0x01, 		0x2C, 		23};		// Reactive Power
			Register VA_A 			{0x01, 		0x2F, 		23};		// Apparent Power
			Register VA_B 			{0x01, 		0x32, 		23};		// Apparent Power
			Register VA_C 			{0x01, 		0x35, 		23};		// Apparent Power
			Register WATT_T 		{0x01, 		0x38, 		23};		// Active Power average (Total / 3)
			Register VAR_T 			{0x01, 		0x3B, 		23};		// Reactive Power average (Total / 3)
			Register VA_T 			{0x01, 		0x3E, 		23};		// Apparent Power average (Total / 3)
			Register IFSCALE 		{0x01, 		0x41, 		0};			// Scratch register (see Scaling Registers section)
			Register VSCALE 		{0x01, 		0x44, 		0};			// Scratch register (see Scaling Registers section)
			Register PFUND_A 		{0x01, 		0x4A, 		23};		// Fundamental Power
			Register PFUND_B 		{0x01, 		0x4D, 		23};		// Fundamental Power
			Register PFUND_C 		{0x01, 		0x50, 		23};		// Fundamental Power
			Register PHARM_A 		{0x01, 		0x53, 		23};		// Harmonic Power
			Register PHARM_B 		{0x01, 		0x56, 		23};		// Harmonic Power
			Register PHARM_C 		{0x01, 		0x59, 		23};		// Harmonic Power
			Register VAFUNDA 		{0x01, 		0x5C, 		23};		// Fundamental Volt Amperes
			Register VAFUNDB 		{0x01, 		0x5F, 		23};		// Fundamental Volt Amperes
			Register VAFUNDC 		{0x01, 		0x62, 		23};		// Fundamental Volt Amperes
			Register PFA 			{0x01, 		0x65, 		22};		// Power Factor
			Register PFB 			{0x01, 		0x68, 		22};		// Power Factor
			Register PFC 			{0x01, 		0x6B, 		22};		// Power Factor
			Register PF_T 			{0x01, 		0x6E, 		22};		// T otal Power Factor
			Register PF_MIN 		{0x01, 		0x71, 		22};		// Power Factor lower alarm limit
			Register TEMPC 			{0x01, 		0x74, 		10};		// Chip Temperature (Celsius°)
			Register T_TARGET 		{0x01, 		0x77, 		10};		// Temperature calibration target
			Register T_MIN 			{0x01, 		0x7A, 		10};		// Temperature Alarm Lower Limit
			Register T_MAX 			{0x01, 		0x7D, 		10};		// Temperature Alarm Upper Limit
			Register FREQ 			{0x01, 		0x80, 		16};		// Line Frequency
			Register F_MIN 			{0x01, 		0x83, 		16};		// Frequency Alarm Lower Limit
			Register F_MAX 			{0x01, 		0x86, 		16};		// Frequency Alarm Upper Limit
			Register MIN0 			{0x01, 		0x89, 		23};		// Minimum Recorded Value 1
			Register MIN1 			{0x01, 		0x8C, 		23};		// Minimum Recorded Value 2
			Register MIN2 			{0x01, 		0x8F, 		23};		// Minimum Recorded Value 3
			Register MIN3 			{0x01, 		0x92, 		23};		// Minimum Recorded Value 4
			Register MIN4 			{0x01, 		0x95, 		23};		// Minimum Recorded Value 5
			Register MIN5 			{0x01, 		0x98, 		23};		// Minimum Recorded Value 6
			Register MIN6 			{0x01, 		0x9B, 		0};			// Minimum Recorded Value 7
			Register MIN7 			{0x01, 		0x9E, 		0};			// Minimum Recorded Value 8
			Register MAX0 			{0x01, 		0xA1, 		23};		// Maximum Recorded Value 1
			Register MAX1 			{0x01, 		0xA4, 		23};		// Maximum Recorded Value 2
			Register MAX2 			{0x01, 		0xA7, 		23};		// Maximum Recorded Value 3
			Register MAX3 			{0x01, 		0xAA, 		23};		// Maximum Recorded Value 4
			Register MAX4 			{0x01, 		0xAD, 		23};		// Maximum Recorded Value 5
			Register MAX5 			{0x01, 		0xB0, 		23};		// Maximum Recorded Value 6
			Register MAX6 			{0x01, 		0xB3, 		0};			// Maximum Recorded Value 7
			Register MAX7 			{0x01, 		0xB6, 		0};			// Maximum Recorded Value 8
			Register MMADDR0 		{0x01, 		0xB9, 		0};			// Min/Max Monitor address 1
			Register MMADDR1 		{0x01, 		0xBC, 		0};			// Min/Max Monitor address 2
			Register MMADDR2 		{0x01, 		0xBF, 		0};			// Min/Max Monitor address 3
			Register MMADDR3 		{0x01, 		0xC2, 		0};			// Min/Max Monitor address 4
			Register MMADDR4 		{0x01, 		0xC5, 		0};			// Min/Max Monitor address 5
			Register MMADDR5 		{0x01, 		0xC8, 		0};			// Min/Max Monitor address 6
			Register MMADDR6 		{0x01, 		0xCB, 		0};			// Min/Max Monitor address 7
			Register MMADDR7 		{0x01, 		0xCE, 		0};			// Min/Max Monitor address 8
			Register BUCKET_LOW 	{0x01, 		0xD1, 		0};			// Energy Bucket Size – Low word
			Register BUCKET_HIGH 	{0x01, 		0xD4, 		0};			// Energy Bucket Size – High word
			Register WHA_POS 		{0x01, 		0xDD, 		0};			// Received Active Energy Counter
			Register WHA_NEG 		{0x01, 		0xE6, 		0};			// Delivered Active Energy Counter
			Register WHB_POS 		{0x01, 		0xEF, 		0};			// Received Active Energy Counter
			Register WHB_NEG 		{0x01, 		0xF8, 		0};			// Delivered Active Energy Counter
			Register WHC_POS 		{0x02, 		0x01, 		0};			// Received Active Energy Counter
			Register WHC_NEG 		{0x02, 		0x0A, 		0};			// Delivered Active Energy Counter
			Register VARHA_POS 		{0x02, 		0x13, 		0};			// Reactive Energy Leading Counter
			Register VARHA_NEG 		{0x02, 		0x1C, 		0};			// Reactive Energy Lagging Counter
			Register VARHB_POS 		{0x02, 		0x25, 		0};			// Reactive Energy Leading Counter
			Register VARHB_NEG 		{0x02, 		0x2E, 		0};			// Reactive Energy Lagging Counter
			Register VARHC_POS 		{0x02, 		0x37, 		0};			// Reactive Energy Leading Counter
			Register VARHC_NEG 		{0x02, 		0x40, 		0};			// Reactive Energy Lagging Counter
			Register SYSSTAT 		{0x02, 		0x43, 		0};			// Bit 23 is a sticky register with status of any SPI Errors
		} Registers;

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
				float	Max			= 253;
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

		/**
		 * @brief Set Voltage Scale Function
		 * @version 01.00.00
		 * @param _Voltage_Scale Voltage Scale
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_VScale(const uint32_t _Voltage_Scale) {

			// Set Register
			return(this->Register_Pointer_Set(this->Registers.VSCALE, _Voltage_Scale));

		}

		/**
		 * @brief Set Current Scale Function
		 * @version 01.00.00
		 * @param _Current_Scale Current Scale
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_IScale(const uint32_t _Current_Scale) {

			// Set Register
			return(this->Register_Pointer_Set(this->Registers.IFSCALE, _Current_Scale));

		}

		/**
		 * @brief Bucket Size Set Function.
		 * @version 01.00.00
		 * @param _Bucket_H Bucket High Word
		 * @param _Bucket_L Bucket Low Word
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Write_Bucket(const uint32_t _Bucket_H, const uint32_t _Bucket_L) {

			// Set Command
			bool _Result_LOW = this->Register_Pointer_Set(this->Registers.BUCKET_LOW, _Bucket_L);
			bool _Result_HIGH = this->Register_Pointer_Set(this->Registers.BUCKET_HIGH, _Bucket_H);

			// End Function
			return(_Result_LOW and _Result_HIGH);

		}

		/**
		 * @brief Get System Status Function.
		 * @version 01.00.00
		 * @return uint32_t System Status
		 */
		uint32_t Get_System_Stat(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.SYSSTAT));

		}

		/**
		 * @brief Get Device Frame Rate Function.
		 * @version 01.00.00
		 * @return uint32_t Frame Rate
		 */
		uint32_t Get_Frame(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.FRAME));

		}

		/**
		 * @brief Get Sample Counter Function
		 * @version 01.00.00
		 * @return uint32_t Sample Counter
		 */
		uint32_t Get_Cycle(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.CYCLE));

		}

		/**
		 * @brief Get Divisor Value Function
		 * @version 01.00.00
		 * @return uint32_t Divisor Value
		 */
		uint32_t Get_Divisor(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.DIVISOR));

		}

		/**
		 * @brief Get Samples per Accumulation Interval Function.
		 * @version 01.00.00
		 * @return uint32_t Sample per Accumulation Interval
		 */
		uint32_t Get_Samples(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.SAMPLES));

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

			// Write Registers
			bool _Result_Min = this->Register_Pointer_Set(this->Registers.VRMS_MIN, this->FtoS(_VMin / Config.VScale, this->Registers.VRMS_MIN.Data_Type));
			bool _Result_Max = this->Register_Pointer_Set(this->Registers.VRMS_MAX, this->FtoS(_VMax / Config.VScale, this->Registers.VRMS_MAX.Data_Type));

			// End Function
			return(_Result_Min and _Result_Max);

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

			// Write Registers
			bool _Result_Min = this->Register_Pointer_Set(this->Registers.F_MIN, this->FtoS(_FQMin, this->Registers.F_MIN.Data_Type));
			bool _Result_Max = this->Register_Pointer_Set(this->Registers.F_MAX, this->FtoS(_FQMax, this->Registers.F_MAX.Data_Type));

			// End Function
			return(_Result_Min and _Result_Max);

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

			// Write Registers
			bool _Result_Vimb = this->Register_Pointer_Set(this->Registers.V_IMB_MAX, this->FtoS(_VImb, this->Registers.V_IMB_MAX.Data_Type));
			bool _Result_Iimb = this->Register_Pointer_Set(this->Registers.I_IMB_MAX, this->FtoS(_IImb, this->Registers.I_IMB_MAX.Data_Type));

			// End Function
			return(_Result_Vimb and _Result_Iimb);

		}

		/**
		 * @brief Temperature Limit Set Function.
		 * @version 01.00.00
		 * @param _TMin Minimum Temperature Limit
		 * @param _TMax Maximum Temperature Limit
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_Temperature_Limits(const float _TMin, const float _TMax) {

			// Write Registers
			bool _Result_Min = this->Register_Pointer_Set(this->Registers.T_MIN, this->FtoS(_TMin, this->Registers.T_MIN.Data_Type));
			bool _Result_Max = this->Register_Pointer_Set(this->Registers.T_MAX, this->FtoS(_TMax, this->Registers.T_MAX.Data_Type));

			// End Function
			return(_Result_Min and _Result_Max);

		}

		/**
		 * @brief Current Limit Set Function.
		 * @version 01.00.00
		 * @param _TMax Maximum Current Limit
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_Current_Limits(const float _IMax) {

			// Write Registers
			bool _Result_Max = this->Register_Pointer_Set(this->Registers.IRMS_MAX, this->FtoS(_IMax / Config.IScale, this->Registers.IRMS_MAX.Data_Type));

			// End Function
			return(_Result_Max);

		}






		/**
		 * @brief Set Harmonic Channel Function.
		 * @version 01.00.00
		 * @param _Harmonic Harmonic Channel
		 * @return uint8_t 
		 * TODO: Not Tested.
		 */
		uint8_t Set_Harmonic(uint32_t _Harmonic) {

			// Declare Variable
			uint32_t _Result = 0;

			// Decide Action
			if (_Harmonic == 0) {
				
				// Read Register
				_Result = this->Register_Pointer_Read(this->Registers.HARM);

			} else {

				// Set Register
				if (Register_Pointer_Set(this->Registers.HARM, _Harmonic)) {
					
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
		 * @brief Voltage High Pass Filter Coefficient Function.
		 * @version 01.00.00
		 * @param _COEF Coefficient Value
		 * @return float Coefficient Value
		 * TODO: Not Tested.
		 */
		float Voltage_HPF_COEF(float _COEF) {

			// Declare Variable
			float _Result = 0;

			// Decide Workflow
			if (_COEF == -999) {

				// Read Register
				_Result = this->Register_Pointer_Read(this->Registers.HPF_COEF_V); // Measure Phase R

			} else {

				// Set Register
				_Result = this->Register_Pointer_Set(this->Registers.HPF_COEF_V, this->FtoS(_COEF, 23)); // Measure Phase R

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

			// Declare Variable
			float _Result = 0;

			// Decide Workflow
			if (_COEF == -999) {

				// Read Register
				_Result = this->Register_Pointer_Read(this->Registers.HPF_COEF_I); // Measure Phase R

			} else {

				// Set Register
				_Result = this->Register_Pointer_Set(this->Registers.HPF_COEF_I, this->FtoS(_COEF, 23)); // Measure Phase R

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

			// Declare Variable
			float _Result = 0;

			// Decide Workflow
			if (_Offset == -999) {

				// Read Register
				if (Phase == 'R') _Result = this->Register_Pointer_Read(this->Registers.IARMS_OFF); // Measure Phase R
				if (Phase == 'S') _Result = this->Register_Pointer_Read(this->Registers.IBRMS_OFF); // Measure Phase S
				if (Phase == 'T') _Result = this->Register_Pointer_Read(this->Registers.ICRMS_OFF); // Measure Phase T

			} else {

				// Set Register
				if (Phase == 'R') _Result = this->Register_Pointer_Set(this->Registers.IARMS_OFF, this->FtoS(_Offset, 23)); // Measure Phase R
				if (Phase == 'S') _Result = this->Register_Pointer_Set(this->Registers.IBRMS_OFF, this->FtoS(_Offset, 23)); // Measure Phase S
				if (Phase == 'T') _Result = this->Register_Pointer_Set(this->Registers.ICRMS_OFF, this->FtoS(_Offset, 23)); // Measure Phase T

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

			// Declare Variable
			float _Result = 0;

			// Decide Workflow
			if (_Offset == -999) {

				// Read Register
				if (Phase == 'R') _Result = this->Register_Pointer_Read(this->Registers.PA_OFFS); // Measure Phase R
				if (Phase == 'S') _Result = this->Register_Pointer_Read(this->Registers.PB_OFFS); // Measure Phase S
				if (Phase == 'T') _Result = this->Register_Pointer_Read(this->Registers.PC_OFFS); // Measure Phase T

			} else {

				// Set Register
				if (Phase == 'R') _Result = this->Register_Pointer_Set(this->Registers.PA_OFFS, this->FtoS(_Offset, 23)); // Measure Phase R
				if (Phase == 'S') _Result = this->Register_Pointer_Set(this->Registers.PB_OFFS, this->FtoS(_Offset, 23)); // Measure Phase S
				if (Phase == 'T') _Result = this->Register_Pointer_Set(this->Registers.PC_OFFS, this->FtoS(_Offset, 23)); // Measure Phase T

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

			// Declare Variable
			float _Result = 0;

			// Decide Workflow
			if (_Offset == -999) {

				// Read Register
				if (Phase == 'R') _Result = this->Register_Pointer_Read(this->Registers.QA_OFFS); // Measure Phase R
				if (Phase == 'S') _Result = this->Register_Pointer_Read(this->Registers.QB_OFFS); // Measure Phase S
				if (Phase == 'T') _Result = this->Register_Pointer_Read(this->Registers.QC_OFFS); // Measure Phase T

			} else {

				// Set Register
				if (Phase == 'R') _Result = this->Register_Pointer_Set(this->Registers.QA_OFFS, this->FtoS(_Offset, 23)); // Measure Phase R
				if (Phase == 'S') _Result = this->Register_Pointer_Set(this->Registers.QB_OFFS, this->FtoS(_Offset, 23)); // Measure Phase S
				if (Phase == 'T') _Result = this->Register_Pointer_Set(this->Registers.QC_OFFS, this->FtoS(_Offset, 23)); // Measure Phase T

			}

			// End Function
			return(_Result);

		}

		/**
		 * @brief Set Sticky Function
		 * @version 01.00.00
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_Sticky(const uint32_t _Sticky) {

			// Return Result
			return(this->Register_Pointer_Set(this->Registers.STICKY, _Sticky));

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

			// Set Limits
			this->Set_Voltage_Limits(this->Limit.Voltage.Min, this->Limit.Voltage.Max);
			this->Set_Frequency_Limits(this->Limit.Frequency.Min, this->Limit.Frequency.Max);
			this->Set_Imbalance_Limits(this->Limit.Voltage.Imbalance, this->Limit.Current.Imbalance);
			this->Set_Temperature_Limits(this->Limit.Temperature.Min, this->Limit.Temperature.Max);
			this->Set_Current_Limits(this->Limit.Current.Max);

			// Bucket Set Command
			this->Write_Bucket(this->Config.Bucket_High, this->Config.Bucket_Low);

			// Set Sticky
			this->Set_Sticky(0x800003);

			// Set Min Max Address
			this->Set_Min_Max_Address(1, 0x30); this->Clear_Min_Value(1); this->Clear_Max_Value(1);
			this->Set_Min_Max_Address(2, 0x31); this->Clear_Min_Value(2); this->Clear_Max_Value(2);
			this->Set_Min_Max_Address(3, 0x32); this->Clear_Min_Value(3); this->Clear_Max_Value(3);
			this->Set_Min_Max_Address(4, 0x47); this->Clear_Min_Value(4); this->Clear_Max_Value(4);
			this->Set_Min_Max_Address(5, 0x48); this->Clear_Min_Value(5); this->Clear_Max_Value(5);
			this->Set_Min_Max_Address(6, 0x49); this->Clear_Min_Value(6); this->Clear_Max_Value(6);

		}

		/**
		 * @brief Get Device Address Function.
		 * @version 01.00.00
		 * @return uint32_t Device Address
		 */
		uint32_t Get_Device_Address(void) {
			
			// Return Register
			return(this->Register_Pointer_Read(this->Registers.DEVADDR));

		}

		/**
		 * @brief Get Device Firmware Function
		 * @version 01.00.00
		 * @return uint32_t Device Firmware.
		 */
		uint32_t Get_Firmware(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.FW_VERSION));

		}

		/**
		 * @brief Get Communication Baud Function.
		 * @version 01.00.00
		 * @return uint32_t 
		 */
		uint32_t Get_Baud(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.BAUD));

		}

		/**
		 * @brief Get VSCALE Function.
		 * @version 01.00.00
		 * @return uint32_t 
		 */
		uint32_t Get_VSCale(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.VSCALE));

		}

		/**
		 * @brief Get ISCALE Function.
		 * @version 01.00.00
		 * @return uint32_t 
		 */
		uint32_t Get_ISCale(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.IFSCALE));

		}

		/**
		 * @brief Set Baud Function
		 * @version 01.00.00
		 * @param _Baud Communication Baud Rate
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Set_Baud(const uint32_t _Baud) {

			// Return Result
			return(this->Register_Pointer_Set(this->Registers.BAUD, _Baud));

		}

		/**
		 * @brief Voltage Measurement Function.
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__, __Phase_Avg__
		 * @param _Type __RMS__, __Instant__, __Fundamental__, __Harmonic__
		 * @return float Voltage Measurement [Volt]
		 */
		float Voltage(const uint8_t _Phase, const uint8_t _Type) {

			// Phase Select
			if (_Phase == __Phase_R__) {

				// Measurement Type Select
				if (_Type == __RMS__) 				return(this->Register_Pointer_Read(this->Registers.VA_RMS) * this->Config.VScale);
				else if (_Type == __Instant__) 		return(this->Register_Pointer_Read(this->Registers.VA) * this->Config.VScale);
				else if (_Type == __Fundamental__) 	return(this->Register_Pointer_Read(this->Registers.VFUND_A) * this->Config.VScale);
				else if (_Type == __Harmonic__) 	return(this->Register_Pointer_Read(this->Registers.VHARM_A) * this->Config.VScale);

			} else if (_Phase == __Phase_S__) {

				// Measurement Type Select
				if (_Type == __RMS__) 				return(this->Register_Pointer_Read(this->Registers.VB_RMS) * this->Config.VScale);
				else if (_Type == __Instant__)		return(this->Register_Pointer_Read(this->Registers.VB) * this->Config.VScale);
				else if (_Type == __Fundamental__)	return(this->Register_Pointer_Read(this->Registers.VFUND_B) * this->Config.VScale);
				else if (_Type == __Harmonic__)		return(this->Register_Pointer_Read(this->Registers.VHARM_B) * this->Config.VScale);

			} else if (_Phase == __Phase_T__) {

				// Measurement Type Select
				if (_Type == __RMS__)				return(this->Register_Pointer_Read(this->Registers.VC_RMS) * this->Config.VScale);
				else if (_Type == __Instant__)		return(this->Register_Pointer_Read(this->Registers.VC) * this->Config.VScale);
				else if (_Type == __Fundamental__) 	return(this->Register_Pointer_Read(this->Registers.VFUND_C) * this->Config.VScale);
				else if (_Type == __Harmonic__) 	return(this->Register_Pointer_Read(this->Registers.VHARM_C) * this->Config.VScale);

			} else if (_Phase == __Phase_Avg__) {

				// Measurement Type Select
				if (_Type == __RMS__)				return(this->Register_Pointer_Read(this->Registers.VT_RMS) * this->Config.VScale);

			} else {

				// End Function
				return(0);

			}

		}

		/**
		 * @brief Frequency Measurement Function.
		 * @version 01.00.00
		 * @return float Frequency Measurement [Hz]
		 */
		float Frequency(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.FREQ));

		}

		/**
		 * @brief Current Measurement Function.
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__, __Phase_Avg__
		 * @param _Type __RMS__, __Instant__, __Fundamental__, __Harmonic__, __Peak__, __Phase_Compensation__
		 * @return float Current Measurement [Amper]
		 */
		float Current(const uint8_t _Phase, const uint8_t _Type) {

			// Phase Select
			if (_Phase == __Phase_R__) {

				// Measurement Type Select
				if (_Type == __RMS__) 						return(this->Register_Pointer_Read(this->Registers.IA_RMS) * this->Config.IScale);
				else if (_Type == __Instant__) 				return(this->Register_Pointer_Read(this->Registers.IA) * this->Config.IScale);
				else if (_Type == __Fundamental__)			return(this->Register_Pointer_Read(this->Registers.IFUND_A) * this->Config.IScale);
				else if (_Type == __Harmonic__)				return(this->Register_Pointer_Read(this->Registers.IHARM_A) * this->Config.IScale);
				else if (_Type == __Peak__)					return(this->Register_Pointer_Read(this->Registers.IA_PEAK) * this->Config.IScale);
				else if (_Type == __Phase_Compensation__)	return(this->Register_Pointer_Read(this->Registers.PHASECOMP1) * this->Config.IScale);

			} else if (_Phase == __Phase_S__) {

				// Measurement Type Select
				if (_Type == __RMS__) 						return(this->Register_Pointer_Read(this->Registers.IB_RMS) * this->Config.IScale);
				else if (_Type == __Instant__)				return(this->Register_Pointer_Read(this->Registers.IB) * this->Config.IScale);
				else if (_Type == __Fundamental__) 			return(this->Register_Pointer_Read(this->Registers.IFUND_B) * this->Config.IScale);
				else if (_Type == __Harmonic__) 			return(this->Register_Pointer_Read(this->Registers.IHARM_B) * this->Config.IScale);
				else if (_Type == __Peak__) 				return(this->Register_Pointer_Read(this->Registers.IB_PEAK) * this->Config.IScale);
				else if (_Type == __Phase_Compensation__) 	return(this->Register_Pointer_Read(this->Registers.PHASECOMP2) * this->Config.IScale);

			} else if (_Phase == __Phase_T__) {

				// Measurement Type Select
				if (_Type == __RMS__) 						return(this->Register_Pointer_Read(this->Registers.IC_RMS) * this->Config.IScale);
				else if (_Type == __Instant__) 				return(this->Register_Pointer_Read(this->Registers.IC) * this->Config.IScale);
				else if (_Type == __Fundamental__) 			return(this->Register_Pointer_Read(this->Registers.IFUND_C) * this->Config.IScale);
				else if (_Type == __Harmonic__) 			return(this->Register_Pointer_Read(this->Registers.IHARM_C) * this->Config.IScale);
				else if (_Type == __Peak__) 				return(this->Register_Pointer_Read(this->Registers.IC_PEAK) * this->Config.IScale);
				else if (_Type == __Phase_Compensation__) 	return(this->Register_Pointer_Read(this->Registers.PHASECOMP3) * this->Config.IScale);

			} else if (_Phase == __Phase_Avg__) {

				// Measurement Type Select
				if (_Type == __RMS__) 						return(this->Register_Pointer_Read(this->Registers.IT_RMS) * this->Config.IScale);

			} else {

				// End Function
				return(0);

			}

		}

		/**
		 * @brief Power Measurement Function.
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__, __Phase_Avg__
		 * @param _Type __Active__, __ReActive__, __Apparent__, __Fundamental__, __Harmonic__, __Fund_ReActive__, __Harm_ReActive__, __Fund_VA__
		 * @return float Power Measurement [Watt]
		 */
		float Power(const uint8_t _Phase, const uint8_t _Type) {

			// Phase Select
			if (_Phase == __Phase_R__) {

				// Measurement Type Select
				if (_Type == __Active__) 				return(this->Register_Pointer_Read(this->Registers.WATT_A) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __ReActive__) 		return(this->Register_Pointer_Read(this->Registers.VAR_A) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Apparent__) 		return(this->Register_Pointer_Read(this->Registers.VA_A) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Fundamental__) 		return(this->Register_Pointer_Read(this->Registers.PFUND_A) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Harmonic__) 		return(this->Register_Pointer_Read(this->Registers.PHARM_A) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Fund_ReActive__) 	return(this->Register_Pointer_Read(this->Registers.QFUND_A) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Harm_ReActive__) 	return(this->Register_Pointer_Read(this->Registers.QHARM_A) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Fund_VA__) 			return(this->Register_Pointer_Read(this->Registers.VAFUNDA) * this->Config.VScale * this->Config.IScale);

			} else if (_Phase == __Phase_S__) {

				// Measurement Type Select
				if (_Type == __Active__) 				return(this->Register_Pointer_Read(this->Registers.WATT_B) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __ReActive__) 		return(this->Register_Pointer_Read(this->Registers.VAR_B) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Apparent__) 		return(this->Register_Pointer_Read(this->Registers.VA_B) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Fundamental__) 		return(this->Register_Pointer_Read(this->Registers.PFUND_B) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Harmonic__) 		return(this->Register_Pointer_Read(this->Registers.PHARM_B) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Fund_ReActive__) 	return(this->Register_Pointer_Read(this->Registers.QFUND_B) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Harm_ReActive__) 	return(this->Register_Pointer_Read(this->Registers.QHARM_B) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Fund_VA__) 			return(this->Register_Pointer_Read(this->Registers.VAFUNDB) * this->Config.VScale * this->Config.IScale);

			} else if (_Phase == __Phase_T__) {

				// Measurement Type Select
				if (_Type == __Active__) 				return(this->Register_Pointer_Read(this->Registers.WATT_C) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __ReActive__) 		return(this->Register_Pointer_Read(this->Registers.VAR_C) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Apparent__) 		return(this->Register_Pointer_Read(this->Registers.VA_C) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Fundamental__) 		return(this->Register_Pointer_Read(this->Registers.PFUND_C) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Harmonic__) 		return(this->Register_Pointer_Read(this->Registers.PHARM_C) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Fund_ReActive__) 	return(this->Register_Pointer_Read(this->Registers.QFUND_C) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Harm_ReActive__) 	return(this->Register_Pointer_Read(this->Registers.QHARM_C) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Fund_VA__) 			return(this->Register_Pointer_Read(this->Registers.VAFUNDC) * this->Config.VScale * this->Config.IScale);

			} else if (_Phase == __Phase_Avg__) {

				// Measurement Type Select
				if (_Type == __Active__) 				return(this->Register_Pointer_Read(this->Registers.WATT_T) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __ReActive__) 		return(this->Register_Pointer_Read(this->Registers.VAR_T) * this->Config.VScale * this->Config.IScale);
				else if (_Type == __Apparent__) 		return(this->Register_Pointer_Read(this->Registers.VA_T) * this->Config.VScale * this->Config.IScale);

			}

		}

		/**
		 * @brief Energy Measurement Function.
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__
		 * @param _Type __Active_Received__, __Active_Delivered__, __ReActive_Received__, __ReActive_Delivered__, __Energy_Reset__
		 * @return float Energy Measurement [Watt/Hour]
		 */
		float Energy(const uint8_t _Phase, const uint8_t _Type) {

			// Phase Select
			if (_Phase == __Phase_R__) {

				// Measurement Type Select
				if (_Type == __Active_Received__) 			return(this->Register_Pointer_Read(this->Registers.WHA_POS));
				else if (_Type == __Active_Delivered__) 	return(this->Register_Pointer_Read(this->Registers.WHA_NEG));
				else if (_Type == __ReActive_Received__)	return(this->Register_Pointer_Read(this->Registers.VARHA_POS));
				else if (_Type == __ReActive_Delivered__) 	return(this->Register_Pointer_Read(this->Registers.VARHA_NEG));
				else if (_Type == __Energy_Reset__) {

					// Clear Register
					Register_Pointer_Set(this->Registers.WHA_POS, 0x00);
					Register_Pointer_Set(this->Registers.VARHA_POS, 0x00);
					Register_Pointer_Set(this->Registers.WHA_NEG, 0x00);
					Register_Pointer_Set(this->Registers.VARHA_NEG, 0x00);

					// End Function
					return(1);

				}

			} else if (_Phase == __Phase_S__) {

				// Measurement Type Select
				if (_Type == __Active_Received__) 			return(this->Register_Pointer_Read(this->Registers.WHB_POS));
				else if (_Type == __Active_Delivered__) 	return(this->Register_Pointer_Read(this->Registers.WHB_NEG));
				else if (_Type == __ReActive_Received__) 	return(this->Register_Pointer_Read(this->Registers.VARHB_POS));
				else if (_Type == __ReActive_Delivered__) 	return(this->Register_Pointer_Read(this->Registers.VARHB_NEG));
				else if (_Type == __Energy_Reset__) {

					// Clear Register
					Register_Pointer_Set(this->Registers.WHB_POS, 0x00);
					Register_Pointer_Set(this->Registers.VARHB_POS, 0x00);
					Register_Pointer_Set(this->Registers.WHB_NEG, 0x00);
					Register_Pointer_Set(this->Registers.VARHB_NEG, 0x00);

					// End Function
					return(1);
					
				}

			} else if (_Phase == __Phase_T__) {

				// Measurement Type Select
				if (_Type == __Active_Received__) 			return(this->Register_Pointer_Read(this->Registers.WHC_POS));
				else if (_Type == __Active_Delivered__) 	return(this->Register_Pointer_Read(this->Registers.WHC_NEG));
				else if (_Type == __ReActive_Received__)	return(this->Register_Pointer_Read(this->Registers.VARHC_POS));
				else if (_Type == __ReActive_Delivered__) 	return(this->Register_Pointer_Read(this->Registers.VARHC_NEG));
				else if (_Type == __Energy_Reset__) {

					// Clear Register
					Register_Pointer_Set(this->Registers.WHC_POS, 0x00);
					Register_Pointer_Set(this->Registers.VARHC_POS, 0x00);
					Register_Pointer_Set(this->Registers.WHC_NEG, 0x00);
					Register_Pointer_Set(this->Registers.VARHC_NEG, 0x00);

					// End Function
					return(1);
					
				}

			}

		}

		/**
		 * @brief Power Factor Measurement Function.
		 * @version 01.00.00
		 * @param _Phase __Phase_R__, __Phase_S__, __Phase_T__, __Phase_Avg__
		 * @return float Power Factor Measurement [Cos fi]
		 */
		float Power_Factor(const uint8_t _Phase) {

			// Phase Select
			if (_Phase == __Phase_R__) 			return(this->Register_Pointer_Read(this->Registers.PFA));
			else if (_Phase == __Phase_S__) 	return(this->Register_Pointer_Read(this->Registers.PFB));
			else if (_Phase == __Phase_T__) 	return(this->Register_Pointer_Read(this->Registers.PFC));
			else if (_Phase == __Phase_Avg__) 	return(this->Register_Pointer_Read(this->Registers.PF_T));

		}

		/**
		 * @brief IC Temperature Measurement Function.
		 * @version 01.00.00
		 * @return float IC Temperature Measurement [C]
		 */
		float Temperature(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.TEMPC));

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








		/**
		 * @brief Voltage Imbalance Limit Read Function.
		 * @version 01.00.00
		 * @return float Voltage Imbalance Limit Value.
		 */
		float Read_Voltage_Imbalance_Max_Limit(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.V_IMB_MAX));

		}

		/**
		 * @brief Current Imbalance Limit Read Function.
		 * @version 01.00.00
		 * @return float Current Imbalance Limit Value.
		 */
		float Read_Current_Imbalance_Max_Limit(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.I_IMB_MAX));

		}

		/**
		 * @brief Voltage Min Limit Read Function.
		 * @version 01.00.00
		 * @return float Minimum Voltage Limit Value.
		 */
		float Read_Voltage_Min_Limit(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.VRMS_MIN) * Config.VScale);

		}

		/**
		 * @brief Voltage Max Limit Read Function.
		 * @version 01.00.00
		 * @return float Maximum Voltage Limit Value.
		 */
		float Read_Voltage_Max_Limit(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.VRMS_MAX) * Config.VScale);

		}

		/**
		 * @brief Current Max Limit Read Function.
		 * @version 01.00.00
		 * @return float Maximum Current Limit Value.
		 */
		float Read_Current_Max_Limit(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.IRMS_MAX) * Config.IScale);

		}

		/**
		 * @brief Get Voltage SAG Limit Function.
		 * @version 01.00.00
		 * @return float Voltage SAG
		 */
		float Get_Voltage_SAG_Limit(void) {
			
			// Return Register
			return(this->Register_Pointer_Read(this->Registers.VSAG_LIM) * Config.VScale);

		}

		/**
		 * @brief Frequency Min Limit Read Function.
		 * @version 01.00.00
		 * @return float Minimum Frequency Limit Value.
		 */
		float Read_Frequency_Min_Limit(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.F_MIN));

		}

		/**
		 * @brief Frequency Max Limit Read Function.
		 * @version 01.00.00
		 * @return float Maximum Frequency Limit Value.
		 */
		float Read_Frequency_Max_Limit(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.F_MAX));

		}

		/**
		 * @brief Temperature Min Limit Read Function.
		 * @version 01.00.00
		 * @return float Minimum Temperature Limit Value.
		 */
		float Read_Temperature_Min_Limit(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.T_MIN));

		}

		/**
		 * @brief Temperature Max Limit Read Function.
		 * @version 01.00.00
		 * @return float Maximum Temperature Limit Value.
		 */
		float Read_Temperature_Max_Limit(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.T_MAX));

		}

		/**
		 * @brief PowerFactor Min Limit Read Function.
		 * @version 01.00.00
		 * @return float Minimum PowerFactor Limit Value.
		 */
		float Read_PowerFactor_Min_Limit(void) {

			// Return Register
			return(this->Register_Pointer_Read(this->Registers.PF_MIN));

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
			if (_MM_ADDR == 1) _Result = this->Register_Pointer_Set(this->Registers.MMADDR0, _Mask);
			if (_MM_ADDR == 2) _Result = this->Register_Pointer_Set(this->Registers.MMADDR1, _Mask);
			if (_MM_ADDR == 3) _Result = this->Register_Pointer_Set(this->Registers.MMADDR2, _Mask);
			if (_MM_ADDR == 4) _Result = this->Register_Pointer_Set(this->Registers.MMADDR3, _Mask);
			if (_MM_ADDR == 5) _Result = this->Register_Pointer_Set(this->Registers.MMADDR4, _Mask);
			if (_MM_ADDR == 6) _Result = this->Register_Pointer_Set(this->Registers.MMADDR5, _Mask);
			if (_MM_ADDR == 7) _Result = this->Register_Pointer_Set(this->Registers.MMADDR6, _Mask);
			if (_MM_ADDR == 8) _Result = this->Register_Pointer_Set(this->Registers.MMADDR7, _Mask);

			// End Function
			return(_Result);

		}

		/**
		 * @brief Get Min Value From Address
		 * @version 01.00.00
		 * @param _MM_ADDR Address Channel
		 * @return float Minimum Value
		 */
		float Get_Min_Value(const uint8_t _MM_ADDR) {

			if (_MM_ADDR == 1) return(this->Register_Pointer_Read(this->Registers.MIN0) * Config.VScale); 
			else if (_MM_ADDR == 2) return(this->Register_Pointer_Read(this->Registers.MIN1) * Config.VScale);
			else if (_MM_ADDR == 3) return(this->Register_Pointer_Read(this->Registers.MIN2) * Config.VScale);
			else if (_MM_ADDR == 4) return(this->Register_Pointer_Read(this->Registers.MIN3) * Config.IScale);
			else if (_MM_ADDR == 5) return(this->Register_Pointer_Read(this->Registers.MIN4) * Config.IScale);
			else if (_MM_ADDR == 6) return(this->Register_Pointer_Read(this->Registers.MIN5) * Config.IScale);
			else if (_MM_ADDR == 7) return(this->Register_Pointer_Read(this->Registers.MIN6));
			else if (_MM_ADDR == 8) return(this->Register_Pointer_Read(this->Registers.MIN7));
			else return(0);

		}

		/**
		 * @brief Get Max Value From Address
		 * @version 01.00.00
		 * @param _MM_ADDR Address Channel
		 * @return float Maximum Value
		 */
		float Get_Max_Value(const uint8_t _MM_ADDR) {

			if (_MM_ADDR == 1) {

				return(this->Register_Pointer_Read(this->Registers.MAX0) * Config.VScale);

			} else if (_MM_ADDR == 2) {

				return(this->Register_Pointer_Read(this->Registers.MAX1) * Config.VScale);

			} else if (_MM_ADDR == 3) {

				return(this->Register_Pointer_Read(this->Registers.MAX2) * Config.VScale);

			} else if (_MM_ADDR == 4) {

				return(this->Register_Pointer_Read(this->Registers.MAX3) * Config.IScale);

			} else if (_MM_ADDR == 5) {

				return(this->Register_Pointer_Read(this->Registers.MAX4) * Config.IScale);

			} else if (_MM_ADDR == 6) {

				return(this->Register_Pointer_Read(this->Registers.MAX5) * Config.IScale);

			} else if (_MM_ADDR == 7) {

				return(this->Register_Pointer_Read(this->Registers.MAX6));

			} else if (_MM_ADDR == 8) {

				return(this->Register_Pointer_Read(this->Registers.MAX7));

			} else {

				return(0);

			}

		}

		/**
		 * @brief Clear Min Value From Address
		 * @version 01.00.00
		 * @param _MM_ADDR Address Channel
		 * @return true Function Succeed
		 * @return false Function Fails
		 */
		bool Clear_Min_Value(const uint8_t _MM_ADDR) {

			if (_MM_ADDR == 1) Register_Pointer_Set(this->Registers.MIN0, 0x00);
			else if (_MM_ADDR == 2) Register_Pointer_Set(this->Registers.MIN1, 0x00);
			else if (_MM_ADDR == 3) Register_Pointer_Set(this->Registers.MIN2, 0x00);
			else if (_MM_ADDR == 4) Register_Pointer_Set(this->Registers.MIN3, 0x00);
			else if (_MM_ADDR == 5) Register_Pointer_Set(this->Registers.MIN4, 0x00);
			else if (_MM_ADDR == 6) Register_Pointer_Set(this->Registers.MIN5, 0x00);
			else if (_MM_ADDR == 7) Register_Pointer_Set(this->Registers.MIN6, 0x00);
			else if (_MM_ADDR == 8) Register_Pointer_Set(this->Registers.MIN7, 0x00);

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
		bool Clear_Max_Value(const uint8_t _MM_ADDR) {

			if (_MM_ADDR == 1) Register_Pointer_Set(this->Registers.MAX0, 0x00);
			else if (_MM_ADDR == 2) Register_Pointer_Set(this->Registers.MAX1, 0x00);
			else if (_MM_ADDR == 3) Register_Pointer_Set(this->Registers.MAX2, 0x00);
			else if (_MM_ADDR == 4) Register_Pointer_Set(this->Registers.MAX3, 0x00);
			else if (_MM_ADDR == 5) Register_Pointer_Set(this->Registers.MAX4, 0x00);
			else if (_MM_ADDR == 6) Register_Pointer_Set(this->Registers.MAX5, 0x00);
			else if (_MM_ADDR == 7) Register_Pointer_Set(this->Registers.MAX6, 0x00);
			else if (_MM_ADDR == 8) Register_Pointer_Set(this->Registers.MAX7, 0x00);

			// End Function
			return(true);

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

				bool _Result_Gain = this->Register_Pointer_Set(this->Registers.V1_GAIN, this->FtoS(_Gain, this->Registers.V1_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = this->Register_Pointer_Set(this->Registers.V1_OFFS, this->FtoS(_Offset, this->Registers.V1_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// Phase S Calibration
			if (_Phase == __Phase_S__) {

				bool _Result_Gain = this->Register_Pointer_Set(this->Registers.V2_GAIN, this->FtoS(_Gain, this->Registers.V2_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = this->Register_Pointer_Set(this->Registers.V2_OFFS, this->FtoS(_Offset, this->Registers.V2_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// Phase T Calibration
			if (_Phase == __Phase_T__) {

				bool _Result_Gain = this->Register_Pointer_Set(this->Registers.V3_GAIN, this->FtoS(_Gain, this->Registers.V3_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = this->Register_Pointer_Set(this->Registers.V3_OFFS, this->FtoS(_Offset, this->Registers.V3_GAIN.Data_Type)); // Write Offset

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

				bool _Result_Gain = this->Register_Pointer_Set(this->Registers.I1_GAIN, this->FtoS(_Gain, this->Registers.I1_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = this->Register_Pointer_Set(this->Registers.I1_OFFS, this->FtoS(_Offset, this->Registers.I1_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// Phase S Calibration
			if (_Phase == __Phase_S__) {

				bool _Result_Gain = this->Register_Pointer_Set(this->Registers.I2_GAIN, this->FtoS(_Gain, this->Registers.I2_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = this->Register_Pointer_Set(this->Registers.I2_OFFS, this->FtoS(_Offset, this->Registers.I2_GAIN.Data_Type)); // Write Offset

				// End Function
				return(_Result_Gain and _Result_Offset);

			}

			// Phase T Calibration
			if (_Phase == __Phase_T__) {

				bool _Result_Gain = this->Register_Pointer_Set(this->Registers.I3_GAIN, this->FtoS(_Gain, this->Registers.I3_GAIN.Data_Type)); // Write Gain
				bool _Result_Offset = this->Register_Pointer_Set(this->Registers.I3_OFFS, this->FtoS(_Offset, this->Registers.I3_GAIN.Data_Type)); // Write Offset

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

			bool _Result_Gain = this->Register_Pointer_Set(this->Registers.T_GAIN, this->FtoS(_Gain, this->Registers.T_GAIN.Data_Type)); // Write Gain
			bool _Result_Offset = this->Register_Pointer_Set(this->Registers.T_OFFS, this->FtoS(_Offset, this->Registers.T_GAIN.Data_Type)); // Write Offset

			// End Function
			return(_Result_Gain and _Result_Offset);

		}

		/**
		 * @brief Limit Parameters Control Function.
		 * @version 01.00.00
		 */
		void Control_Limits(void) {

			// Read Status Register
			uint32_t _Last_Status = this->Register_Pointer_Read(this->Registers.STATUS);

			// Control IIMB
			if (bitRead(_Last_Status, 2) ? this->Status.Imbalance.I_IMBAL = true : this->Status.Imbalance.I_IMBAL = false);
			
			// Control VIMB
			if (bitRead(_Last_Status, 3) ? this->Status.Imbalance.V_IMBAL = true : this->Status.Imbalance.V_IMBAL = false);

			// Control VSAG
			if (bitRead(_Last_Status, 4) ? this->Status.SAG.VA_SAG = true : this->Status.SAG.VA_SAG = false);
			if (bitRead(_Last_Status, 5) ? this->Status.SAG.VB_SAG = true : this->Status.SAG.VB_SAG = false);
			if (bitRead(_Last_Status, 6) ? this->Status.SAG.VC_SAG = true : this->Status.SAG.VC_SAG = false);

			// Control Current
			if (bitRead(_Last_Status, 7) ? this->Status.Current.OV_IRMSA = true : this->Status.Current.OV_IRMSA = false);
			if (bitRead(_Last_Status, 8) ? this->Status.Current.OV_IRMSB = true : this->Status.Current.OV_IRMSB = false);
			if (bitRead(_Last_Status, 9) ? this->Status.Current.OV_IRMSC = true : this->Status.Current.OV_IRMSC = false);

			// Control Power Factor
			if (bitRead(_Last_Status, 10) ? this->Status.PowerFactor.UN_PFA = true : this->Status.PowerFactor.UN_PFA = false);
			if (bitRead(_Last_Status, 11) ? this->Status.PowerFactor.UN_PFB = true : this->Status.PowerFactor.UN_PFB = false);
			if (bitRead(_Last_Status, 12) ? this->Status.PowerFactor.UN_PFC = true : this->Status.PowerFactor.UN_PFC = false);

			// Control Voltage
			if (bitRead(_Last_Status, 13) ? this->Status.Voltage.UN_VRMSA = true : this->Status.Voltage.UN_VRMSA = false);
			if (bitRead(_Last_Status, 14) ? this->Status.Voltage.OV_VRMSA = true : this->Status.Voltage.OV_VRMSA = false);
			if (bitRead(_Last_Status, 15) ? this->Status.Voltage.UN_VRMSB = true : this->Status.Voltage.UN_VRMSB = false);
			if (bitRead(_Last_Status, 16) ? this->Status.Voltage.OV_VRMSB = true : this->Status.Voltage.OV_VRMSB = false);
			if (bitRead(_Last_Status, 17) ? this->Status.Voltage.UN_VRMSC = true : this->Status.Voltage.UN_VRMSC = false);
			if (bitRead(_Last_Status, 18) ? this->Status.Voltage.OV_VRMSC = true : this->Status.Voltage.OV_VRMSC = false);

			// Control Temperature
			if (bitRead(_Last_Status, 19) ? this->Status.Temperature.UN_Temp = true : this->Status.Temperature.UN_Temp = false);
			if (bitRead(_Last_Status, 20) ? this->Status.Temperature.OV_Temp = true : this->Status.Temperature.OV_Temp = false);

			// Control Frequency
			if (bitRead(_Last_Status, 21) ? this->Status.Frequency.UN_Freq = true : this->Status.Frequency.UN_Freq = false);
			if (bitRead(_Last_Status, 22) ? this->Status.Frequency.OV_Freq = true : this->Status.Frequency.OV_Freq = false);

		}
	
		/**
		 * @brief Clear IC Status Register.
		 * @version 01.00.00
		 */
		void Clear_Status(void) {

			// Clear Status Register
			Register_Pointer_Set(this->Registers.STATUS, 0x0000);

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