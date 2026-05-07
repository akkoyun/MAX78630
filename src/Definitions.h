#ifndef __MAX78630_Definitions__
#define __MAX78630_Definitions__

	namespace MAX78630Def {

		/* ── SSI Protocol Bytes ───────────────────────────────── */
		namespace Proto {
			constexpr uint8_t Header        = 0xAA;
			constexpr uint8_t ClearAddr     = 0xA0;
			constexpr uint8_t RWAddrLo      = 0xA1;
			constexpr uint8_t RWAddrHi      = 0xA2;
			constexpr uint8_t RWAddr        = 0xA3;
			constexpr uint8_t DeSelect      = 0xC0;
			constexpr uint8_t Select        = 0xCF;
			constexpr uint8_t WriteBytes    = 0xD0;
			constexpr uint8_t Write3Bytes   = 0xD3;
			constexpr uint8_t ReadBytes     = 0xE0;
			constexpr uint8_t Read3Bytes    = 0xE3;

			constexpr uint8_t AckData       = 0xAA;
			constexpr uint8_t AutoReport    = 0xAE;
			constexpr uint8_t AckNoData     = 0xAD;
			constexpr uint8_t Nack          = 0xB0;
			constexpr uint8_t BadCmd        = 0xBC;
			constexpr uint8_t ChkSumBad     = 0xBD;
			constexpr uint8_t BufOverflow   = 0xBF;

			constexpr uint8_t Bytes1        = 0x01;
			constexpr uint8_t Bytes2        = 0x02;
			constexpr uint8_t Bytes3        = 0x03;
			constexpr uint8_t Bytes4        = 0x04;
			constexpr uint8_t Bytes5        = 0x05;
			constexpr uint8_t Bytes6        = 0x06;
			constexpr uint8_t Bytes7        = 0x07;
			constexpr uint8_t Bytes8        = 0x08;
			constexpr uint8_t Bytes9        = 0x09;
			constexpr uint8_t Bytes10       = 0x0A;
		}

		/* ── Function Codes ──────────────────────────────────── */
		constexpr uint8_t CLEAR             = 0;
		constexpr uint8_t GET               = 1;
		constexpr uint8_t SET               = 2;

		/* ── Phase Types ─────────────────────────────────────── */
		constexpr uint8_t Phase_R           = 1;
		constexpr uint8_t Phase_S           = 2;
		constexpr uint8_t Phase_T           = 3;
		constexpr uint8_t Phase_Avg         = 4;
		constexpr uint8_t Phase_Any         = 5;

		/* ── Measurement Types ───────────────────────────────── */
		constexpr uint8_t RMS               = 1;
		constexpr uint8_t Instant           = 2;
		constexpr uint8_t Fundamental       = 3;
		constexpr uint8_t Harmonic          = 4;
		constexpr uint8_t Peak              = 5;
		constexpr uint8_t Active            = 6;
		constexpr uint8_t ReActive          = 7;
		constexpr uint8_t Apparent          = 8;
		constexpr uint8_t Fund_ReActive     = 9;
		constexpr uint8_t Harm_ReActive     = 10;
		constexpr uint8_t Fund_VA           = 11;
		constexpr uint8_t Active_Received   = 12;
		constexpr uint8_t Active_Delivered  = 13;
		constexpr uint8_t ReActive_Received  = 14;
		constexpr uint8_t ReActive_Delivered = 15;
		constexpr uint8_t Energy_Reset      = 16;
		constexpr uint8_t Phase_Compensation = 17;

		/* ── Calibration Types ───────────────────────────────── */
		constexpr uint8_t Voltage           = 1;
		constexpr uint8_t Current           = 2;
		constexpr uint8_t Temperature       = 3;
		constexpr uint8_t Active_Power      = 4;
		constexpr uint8_t ReActive_Power    = 5;

		/* ── Limit Types ─────────────────────────────────────── */
		constexpr uint8_t VRms_Min          = 1;
		constexpr uint8_t VRms_Max          = 2;
		constexpr uint8_t T_Min             = 3;
		constexpr uint8_t T_Max             = 4;
		constexpr uint8_t F_Min             = 5;
		constexpr uint8_t F_Max             = 6;
		constexpr uint8_t IRms_Max          = 7;
		constexpr uint8_t PF_Min            = 8;
		constexpr uint8_t VImb_Max          = 9;
		constexpr uint8_t IImb_Max          = 10;
		constexpr uint8_t VSag_Lim          = 11;

		/* ── Alarm Mask Pins ─────────────────────────────────── */
		constexpr uint8_t Mask_AL1          = 0x01;
		constexpr uint8_t Mask_AL2          = 0x02;

		/* ── Alarm Types ─────────────────────────────────────── */
		constexpr uint16_t Alarm_LowVoltage  = 1;
		constexpr uint16_t Alarm_HighVoltage = 2;
		constexpr uint16_t Alarm_HighCurrent = 3;
		constexpr uint16_t Alarm_LowFreq    = 4;
		constexpr uint16_t Alarm_HighFreq   = 5;
		constexpr uint16_t Alarm_VImbal     = 6;
		constexpr uint16_t Alarm_IImbal     = 7;
		constexpr uint16_t Alarm_LowTemp    = 8;
		constexpr uint16_t Alarm_HighTemp   = 9;
		constexpr uint16_t Alarm_LowPF      = 10;

		/* ── Status Register Bit Positions ───────────────────── */
		constexpr uint8_t Bit_OV_Freq       = 22;
		constexpr uint8_t Bit_UN_Freq       = 21;
		constexpr uint8_t Bit_OV_Temp       = 20;
		constexpr uint8_t Bit_UN_Temp       = 19;
		constexpr uint8_t Bit_OV_VrmsC      = 18;
		constexpr uint8_t Bit_UN_VrmsC      = 17;
		constexpr uint8_t Bit_OV_VrmsB      = 16;
		constexpr uint8_t Bit_UN_VrmsB      = 15;
		constexpr uint8_t Bit_OV_VrmsA      = 14;
		constexpr uint8_t Bit_UN_VrmsA      = 13;
		constexpr uint8_t Bit_UN_PFC        = 12;
		constexpr uint8_t Bit_UN_PFB        = 11;
		constexpr uint8_t Bit_UN_PFA        = 10;
		constexpr uint8_t Bit_OV_IrmsC      = 9;
		constexpr uint8_t Bit_OV_IrmsB      = 8;
		constexpr uint8_t Bit_OV_IrmsA      = 7;
		constexpr uint8_t Bit_VC_Sag        = 6;
		constexpr uint8_t Bit_VB_Sag        = 5;
		constexpr uint8_t Bit_VA_Sag        = 4;
		constexpr uint8_t Bit_V_Imbal       = 3;
		constexpr uint8_t Bit_I_Imbal       = 2;

		/* ── Monitor Numbers ─────────────────────────────────── */
		constexpr uint8_t Monitor1          = 1;
		constexpr uint8_t Monitor2          = 2;
		constexpr uint8_t Monitor3          = 3;
		constexpr uint8_t Monitor4          = 4;
		constexpr uint8_t Monitor5          = 5;
		constexpr uint8_t Monitor6          = 6;
		constexpr uint8_t Monitor7          = 7;
		constexpr uint8_t Monitor8          = 8;

		/* ── Alarm Output Pins ───────────────────────────────── */
		constexpr uint8_t Alarm1            = 1;
		constexpr uint8_t Alarm2            = 2;

	} // namespace MAX78630Def

#endif /* defined(__MAX78630_Definitions__) */
