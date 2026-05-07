#ifndef __MAX78630_Config__
#define __MAX78630_Config__

	// UART Configuration (hardware object — must stay as #define)
	#ifndef __MAX78630_Serial__
		#define __MAX78630_Serial__ Serial2
	#endif

	namespace MAX78630Config {

		/* ── Device ──────────────────────────────────────────── */
		constexpr uint32_t Firmware         = 0x0004D912;

		/* ── Scale ───────────────────────────────────────────── */
		constexpr uint16_t VScale           = 667;
		constexpr uint16_t IScale           = 7;

		/* ── Energy Bucket ───────────────────────────────────── */
		constexpr uint32_t BucketH          = 0x000821;
		constexpr uint32_t BucketL          = 0xD0F4C2;

		/* ── Alarm ───────────────────────────────────────────── */
		constexpr bool     StatusSticky     = false;

		/* ── Voltage Limits (V) ──────────────────────────────── */
		constexpr float    VoltageMin       = 190.0f;
		constexpr float    VoltageMinDiff   = 10.0f;
		constexpr float    VoltageMax       = 253.0f;
		constexpr float    VoltageMaxDiff   = 10.0f;

		/* ── Current Limits (A) ──────────────────────────────── */
		constexpr float    CurrentMax       = 5.0f;
		constexpr float    CurrentMaxDiff   = 0.5f;

		/* ── Frequency Limits (Hz) ───────────────────────────── */
		constexpr float    FreqMin          = 47.0f;
		constexpr float    FreqMinDiff      = 1.0f;
		constexpr float    FreqMax          = 52.0f;
		constexpr float    FreqMaxDiff      = 1.0f;

		/* ── Temperature Limits (°C) ─────────────────────────── */
		constexpr float    TempMin          = 10.0f;
		constexpr float    TempMinDiff      = 2.0f;
		constexpr float    TempMax          = 35.0f;
		constexpr float    TempMaxDiff      = 2.0f;

		/* ── Imbalance Limits (fraction) ─────────────────────── */
		constexpr float    VImbMax          = 0.06f;
		constexpr float    VImbMaxDiff      = 0.01f;
		constexpr float    IImbMax          = 0.06f;
		constexpr float    IImbMaxDiff      = 0.01f;

		/* ── Voltage Sag ─────────────────────────────────────── */
		constexpr float    VSagLim          = 0.9f;

		/* ── Min/Max Monitor Assignments ─────────────────────── */
		constexpr uint8_t  Monitor1Type     = 0x30;  // VRMS_R
		constexpr uint16_t Monitor1Scale    = VScale;
		constexpr uint8_t  Monitor2Type     = 0x31;  // VRMS_S
		constexpr uint16_t Monitor2Scale    = VScale;
		constexpr uint8_t  Monitor3Type     = 0x32;  // VRMS_T
		constexpr uint16_t Monitor3Scale    = VScale;
		constexpr uint8_t  Monitor4Type     = 0x47;  // IRMS_R
		constexpr uint16_t Monitor4Scale    = IScale;
		constexpr uint8_t  Monitor5Type     = 0x48;  // IRMS_S
		constexpr uint16_t Monitor5Scale    = IScale;
		constexpr uint8_t  Monitor6Type     = 0x49;  // IRMS_T
		constexpr uint16_t Monitor6Scale    = IScale;
		constexpr uint8_t  Monitor7Type     = 0x7A;  // PF_T
		constexpr uint16_t Monitor7Scale    = 1;
		constexpr uint8_t  Monitor8Type     = 0x80;  // FREQ
		constexpr uint16_t Monitor8Scale    = 1;

	} // namespace MAX78630Config

#endif /* defined(__MAX78630_Config__) */
