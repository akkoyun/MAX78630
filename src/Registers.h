#ifndef __MAX78630_Registers__
#define __MAX78630_Registers__

	// IC Register Address Structure
	struct Register {
		const uint8_t High_Address;
		const uint8_t Low_Address;
		const uint8_t Data_Type;
	};

	/*
		MAX78630 Register Map
		─────────────────────────────────────────────────────
		Address encoding  : High_Address = byte_addr >> 8
		                    Low_Address  = byte_addr & 0xFF
		Data_Type         : S.nn → nn   |   INT → 0
		─────────────────────────────────────────────────────
		Source: MAX78630+PPM Data Sheet Rev 0, pp. 58-62
	*/

	namespace MAX78630Reg {

		/* ── System / Control ─────────────────────────────── */
		constexpr Register COMMAND      {0x00, 0x00,  0}; // Selects modes, functions, options (INT)
		constexpr Register CONFIG       {0x00, 0x03,  0}; // Selects input configuration (INT)
		constexpr Register FW_VERSION   {0x00, 0x06,  0}; // Hardware and Firmware version (INT)
		constexpr Register SAMPLES      {0x00, 0x09,  0}; // Min high-rate samples per accumulation (INT)
		constexpr Register DIVISOR      {0x00, 0x0C,  0}; // Actual samples in previous interval (INT)
		constexpr Register CYCLE        {0x00, 0x0F,  0}; // High-rate sample counter (INT)
		constexpr Register FRAME        {0x00, 0x12,  0}; // Low-rate sample counter (INT)

		/* ── Status ───────────────────────────────────────── */
		constexpr Register STATUS       {0x00, 0x15,  0}; // Alarm and device status bits (INT) — READ
		constexpr Register STATUS_CLEAR {0x00, 0x18,  0}; // Reset alarm/status bits (INT) — WRITE
		constexpr Register STATUS_SET   {0x00, 0x1B,  0}; // Set/force alarm/status bits (INT) — WRITE

		/* ── Alarm Masks ──────────────────────────────────── */
		constexpr Register MASK1        {0x00, 0x1E,  0}; // Alarm mask for AL1 pin (INT)
		constexpr Register MASK2        {0x00, 0x21,  0}; // Alarm mask for AL2 pin (INT)
		constexpr Register MASK3        {0x00, 0x24,  0}; // Alarm mask for AL3 pin (INT)
		constexpr Register MASK4        {0x00, 0x27,  0}; // Alarm mask for AL4 pin (INT)
		constexpr Register MASK5        {0x00, 0x2A,  0}; // Alarm mask for AL5 pin (INT)
		constexpr Register STICKY       {0x00, 0x2D,  0}; // Alarm bits held until host clears (INT)

		/* ── Digital I/O ──────────────────────────────────── */
		constexpr Register DIO_STATE    {0x00, 0x30,  0}; // State of DIO pins (INT)
		constexpr Register DIO_DIR      {0x00, 0x33,  0}; // Direction of DIO pins (INT)
		constexpr Register DIO_POL      {0x00, 0x36,  0}; // Polarity of DIO pins (INT)

		/* ── Calibration Control ──────────────────────────── */
		constexpr Register CALCYCS      {0x00, 0x39,  0}; // Number of calibration cycles (INT)
		constexpr Register HPF_COEF_I   {0x00, 0x3C, 23}; // Current HPF coefficient (S.23)
		constexpr Register HPF_COEF_V   {0x00, 0x3F, 23}; // Voltage HPF coefficient (S.23)

		/* ── Phase Compensation ───────────────────────────── */
		constexpr Register PHASECOMP1   {0x00, 0x42, 21}; // Phase compensation AI1 (S.21)
		constexpr Register PHASECOMP2   {0x00, 0x45, 21}; // Phase compensation AI2 (S.21)
		constexpr Register PHASECOMP3   {0x00, 0x48, 21}; // Phase compensation AI3 (S.21)

		/* ── Interface ────────────────────────────────────── */
		constexpr Register HARM         {0x00, 0x4B,  0}; // Harmonic Selector (INT)
		constexpr Register DEVADDR      {0x00, 0x4E,  0}; // I2C and UART device address (INT)
		constexpr Register BAUD         {0x00, 0x51,  0}; // UART baud rate (INT)

		/* ── Gain Calibration ─────────────────────────────── */
		constexpr Register I1_GAIN      {0x00, 0x54, 21}; // Current Gain R (S.21)
		constexpr Register I2_GAIN      {0x00, 0x57, 21}; // Current Gain S (S.21)
		constexpr Register I3_GAIN      {0x00, 0x5A, 21}; // Current Gain T (S.21)
		constexpr Register V1_GAIN      {0x00, 0x5D, 21}; // Voltage Gain R (S.21)
		constexpr Register V2_GAIN      {0x00, 0x60, 21}; // Voltage Gain S (S.21)
		constexpr Register V3_GAIN      {0x00, 0x63, 21}; // Voltage Gain T (S.21)

		/* ── Offset Calibration ───────────────────────────── */
		constexpr Register I1_OFFS      {0x00, 0x66, 23}; // Current Offset R (S.23)
		constexpr Register I2_OFFS      {0x00, 0x69, 23}; // Current Offset S (S.23)
		constexpr Register I3_OFFS      {0x00, 0x6C, 23}; // Current Offset T (S.23)
		constexpr Register V1_OFFS      {0x00, 0x6F, 23}; // Voltage Offset R (S.23)
		constexpr Register V2_OFFS      {0x00, 0x72, 23}; // Voltage Offset S (S.23)
		constexpr Register V3_OFFS      {0x00, 0x75, 23}; // Voltage Offset T (S.23)

		/* ── Temperature Calibration ──────────────────────── */
		constexpr Register T_GAIN       {0x00, 0x78,  0}; // Temperature slope calibration (INT)
		constexpr Register T_OFFS       {0x00, 0x7B,  0}; // Temperature offset calibration (INT)

		/* ── Voltage Sag / Imbalance ──────────────────────── */
		constexpr Register VSAG_INT     {0x00, 0x7E,  0}; // Voltage sag detect interval (INT)
		constexpr Register V_IMB_MAX    {0x00, 0x81, 23}; // Voltage imbalance alarm limit (S.23)
		constexpr Register I_IMB_MAX    {0x00, 0x84, 23}; // Current imbalance alarm limit (S.23)

		/* ── Instantaneous Voltage ────────────────────────── */
		constexpr Register VA           {0x00, 0x87, 23}; // Instantaneous Voltage R (S.23)
		constexpr Register VB           {0x00, 0x8A, 23}; // Instantaneous Voltage S (S.23)
		constexpr Register VC           {0x00, 0x8D, 23}; // Instantaneous Voltage T (S.23)

		/* ── RMS Voltage ──────────────────────────────────── */
		constexpr Register VA_RMS       {0x00, 0x90, 23}; // RMS Voltage R (S.23)
		constexpr Register VB_RMS       {0x00, 0x93, 23}; // RMS Voltage S (S.23)
		constexpr Register VC_RMS       {0x00, 0x96, 23}; // RMS Voltage T (S.23)
		constexpr Register VT_RMS       {0x00, 0x99, 23}; // RMS Voltage Average (S.23)

		/* ── Fundamental / Harmonic Voltage ──────────────── */
		constexpr Register VFUND_A      {0x00, 0x9C, 23}; // Fundamental Voltage R (S.23)
		constexpr Register VFUND_B      {0x00, 0x9F, 23}; // Fundamental Voltage S (S.23)
		constexpr Register VFUND_C      {0x00, 0xA2, 23}; // Fundamental Voltage T (S.23)
		constexpr Register VHARM_A      {0x00, 0xA5, 23}; // Harmonic Voltage R (S.23)
		constexpr Register VHARM_B      {0x00, 0xA8, 23}; // Harmonic Voltage S (S.23)
		constexpr Register VHARM_C      {0x00, 0xAB, 23}; // Harmonic Voltage T (S.23)

		/* ── Voltage Limits ───────────────────────────────── */
		constexpr Register V_TARGET     {0x00, 0xAE, 23}; // Voltage calibration target (S.23)
		constexpr Register VRMS_MIN     {0x00, 0xB1, 23}; // Voltage lower alarm limit (S.23)
		constexpr Register VRMS_MAX     {0x00, 0xB4, 23}; // Voltage upper alarm limit (S.23)
		constexpr Register VSAG_LIM     {0x00, 0xB7, 23}; // RMS Voltage sag threshold (S.23)

		/* ── Instantaneous Current ────────────────────────── */
		constexpr Register IA           {0x00, 0xBA, 23}; // Instantaneous Current R (S.23)
		constexpr Register IB           {0x00, 0xBD, 23}; // Instantaneous Current S (S.23)
		constexpr Register IC           {0x00, 0xC0, 23}; // Instantaneous Current T (S.23)

		/* ── RMS Current Offset ───────────────────────────── */
		constexpr Register IARMS_OFF    {0x00, 0xC3, 23}; // RMS Current dynamic offset R (S.23)
		constexpr Register IBRMS_OFF    {0x00, 0xC6, 23}; // RMS Current dynamic offset S (S.23)
		constexpr Register ICRMS_OFF    {0x00, 0xC9, 23}; // RMS Current dynamic offset T (S.23)

		/* ── Peak Current ─────────────────────────────────── */
		constexpr Register IA_PEAK      {0x00, 0xCC, 23}; // Peak Current R (S.23)
		constexpr Register IB_PEAK      {0x00, 0xCF, 23}; // Peak Current S (S.23)
		constexpr Register IC_PEAK      {0x00, 0xD2, 23}; // Peak Current T (S.23)

		/* ── RMS Current ──────────────────────────────────── */
		constexpr Register IA_RMS       {0x00, 0xD5, 23}; // RMS Current R (S.23)
		constexpr Register IB_RMS       {0x00, 0xD8, 23}; // RMS Current S (S.23)
		constexpr Register IC_RMS       {0x00, 0xDB, 23}; // RMS Current T (S.23)
		constexpr Register IT_RMS       {0x00, 0xDE, 23}; // RMS Current Average (S.23)

		/* ── Fundamental / Harmonic Current ──────────────── */
		constexpr Register IFUND_A      {0x00, 0xE1, 23}; // Fundamental Current R (S.23)
		constexpr Register IFUND_B      {0x00, 0xE4, 23}; // Fundamental Current S (S.23)
		constexpr Register IFUND_C      {0x00, 0xE7, 23}; // Fundamental Current T (S.23)
		constexpr Register IHARM_A      {0x00, 0xEA, 23}; // Harmonic Current R (S.23)
		constexpr Register IHARM_B      {0x00, 0xED, 23}; // Harmonic Current S (S.23)
		constexpr Register IHARM_C      {0x00, 0xF0, 23}; // Harmonic Current T (S.23)

		/* ── Current Limits ───────────────────────────────── */
		constexpr Register IRMS_MAX     {0x00, 0xF3, 23}; // Current upper alarm limit (S.23)
		constexpr Register I_TARGET     {0x00, 0xF6, 23}; // Current calibration target (S.23)

		/* ── Fundamental Reactive Power ───────────────────── */
		constexpr Register QFUND_A      {0x00, 0xF9, 23}; // Fundamental Reactive Power R (S.23)
		constexpr Register QFUND_B      {0x00, 0xFC, 23}; // Fundamental Reactive Power S (S.23)
		constexpr Register QFUND_C      {0x00, 0xFF, 23}; // Fundamental Reactive Power T (S.23)

		/* ── Harmonic Reactive Power ──────────────────────── */
		constexpr Register QHARM_A      {0x01, 0x02, 23}; // Harmonic Reactive Power R (S.23)
		constexpr Register QHARM_B      {0x01, 0x05, 23}; // Harmonic Reactive Power S (S.23)
		constexpr Register QHARM_C      {0x01, 0x08, 23}; // Harmonic Reactive Power T (S.23)

		/* ── Power Offset Registers ───────────────────────── */
		constexpr Register QA_OFFS      {0x01, 0x0B, 23}; // Reactive Power offset R (S.23)
		constexpr Register QB_OFFS      {0x01, 0x0E, 23}; // Reactive Power offset S (S.23)
		constexpr Register QC_OFFS      {0x01, 0x11, 23}; // Reactive Power offset T (S.23)
		constexpr Register PA_OFFS      {0x01, 0x14, 23}; // Active Power offset R (S.23)
		constexpr Register PB_OFFS      {0x01, 0x17, 23}; // Active Power offset S (S.23)
		constexpr Register PC_OFFS      {0x01, 0x1A, 23}; // Active Power offset T (S.23)

		/* ── Active Power ─────────────────────────────────── */
		constexpr Register WATT_A       {0x01, 0x1D, 23}; // Active Power R (S.23)
		constexpr Register WATT_B       {0x01, 0x20, 23}; // Active Power S (S.23)
		constexpr Register WATT_C       {0x01, 0x23, 23}; // Active Power T (S.23)

		/* ── Reactive Power ───────────────────────────────── */
		constexpr Register VAR_A        {0x01, 0x26, 23}; // Reactive Power R (S.23)
		constexpr Register VAR_B        {0x01, 0x29, 23}; // Reactive Power S (S.23)
		constexpr Register VAR_C        {0x01, 0x2C, 23}; // Reactive Power T (S.23)

		/* ── Apparent Power ───────────────────────────────── */
		constexpr Register VA_A         {0x01, 0x2F, 23}; // Apparent Power R (S.23)
		constexpr Register VA_B         {0x01, 0x32, 23}; // Apparent Power S (S.23)
		constexpr Register VA_C         {0x01, 0x35, 23}; // Apparent Power T (S.23)

		/* ── Average Power ────────────────────────────────── */
		constexpr Register WATT_T       {0x01, 0x38, 23}; // Active Power Average (S.23)
		constexpr Register VAR_T        {0x01, 0x3B, 23}; // Reactive Power Average (S.23)
		constexpr Register VA_T         {0x01, 0x3E, 23}; // Apparent Power Average (S.23)

		/* ── Scale Scratch Registers ──────────────────────── */
		constexpr Register IFSCALE      {0x01, 0x41,  0}; // Current full-scale scratch (INT)
		constexpr Register VSCALE       {0x01, 0x44,  0}; // Voltage full-scale scratch (INT)

		/* ── Fundamental Power ────────────────────────────── */
		constexpr Register PFUND_A      {0x01, 0x4A, 23}; // Fundamental Power R (S.23)
		constexpr Register PFUND_B      {0x01, 0x4D, 23}; // Fundamental Power S (S.23)
		constexpr Register PFUND_C      {0x01, 0x50, 23}; // Fundamental Power T (S.23)

		/* ── Harmonic Power ───────────────────────────────── */
		constexpr Register PHARM_A      {0x01, 0x53, 23}; // Harmonic Power R (S.23)
		constexpr Register PHARM_B      {0x01, 0x56, 23}; // Harmonic Power S (S.23)
		constexpr Register PHARM_C      {0x01, 0x59, 23}; // Harmonic Power T (S.23)

		/* ── Fundamental Volt-Amperes ─────────────────────── */
		constexpr Register VAFUND_A     {0x01, 0x5C, 23}; // Fundamental Volt Amperes R (S.23)
		constexpr Register VAFUND_B     {0x01, 0x5F, 23}; // Fundamental Volt Amperes S (S.23)
		constexpr Register VAFUND_C     {0x01, 0x62, 23}; // Fundamental Volt Amperes T (S.23)

		/* ── Power Factor ─────────────────────────────────── */
		constexpr Register PFA          {0x01, 0x65, 22}; // Power Factor R (S.22)
		constexpr Register PFB          {0x01, 0x68, 22}; // Power Factor S (S.22)
		constexpr Register PFC          {0x01, 0x6B, 22}; // Power Factor T (S.22)
		constexpr Register PF_T         {0x01, 0x6E, 22}; // Total Power Factor (S.22)
		constexpr Register PF_MIN       {0x01, 0x71, 22}; // Power Factor lower alarm limit (S.22)

		/* ── Temperature ──────────────────────────────────── */
		constexpr Register TEMPC        {0x01, 0x74, 10}; // Chip Temperature Celsius (S.10)
		constexpr Register T_TARGET     {0x01, 0x77, 10}; // Temperature calibration target (S.10)
		constexpr Register T_MIN        {0x01, 0x7A, 10}; // Temperature alarm lower limit (S.10)
		constexpr Register T_MAX        {0x01, 0x7D, 10}; // Temperature alarm upper limit (S.10)

		/* ── Frequency ────────────────────────────────────── */
		constexpr Register FREQ         {0x01, 0x80, 16}; // Line Frequency (S.16)
		constexpr Register F_MIN        {0x01, 0x83, 16}; // Frequency alarm lower limit (S.16)
		constexpr Register F_MAX        {0x01, 0x86, 16}; // Frequency alarm upper limit (S.16)

		/* ── Min/Max Recorded Values ──────────────────────── */
		// Data_Type matches default MMADDR configuration (V→S.23, I→S.23, PF→S.22, Freq→S.16)
		constexpr Register MIN0         {0x01, 0x89, 23}; // Min Recorded 1 — VRMS_R (S.23)
		constexpr Register MIN1         {0x01, 0x8C, 23}; // Min Recorded 2 — VRMS_S (S.23)
		constexpr Register MIN2         {0x01, 0x8F, 23}; // Min Recorded 3 — VRMS_T (S.23)
		constexpr Register MIN3         {0x01, 0x92, 23}; // Min Recorded 4 — IRMS_R (S.23)
		constexpr Register MIN4         {0x01, 0x95, 23}; // Min Recorded 5 — IRMS_S (S.23)
		constexpr Register MIN5         {0x01, 0x98, 23}; // Min Recorded 6 — IRMS_T (S.23)
		constexpr Register MIN6         {0x01, 0x9B, 22}; // Min Recorded 7 — PF_T   (S.22)
		constexpr Register MIN7         {0x01, 0x9E, 16}; // Min Recorded 8 — FREQ   (S.16)
		constexpr Register MAX0         {0x01, 0xA1, 23}; // Max Recorded 1 — VRMS_R (S.23)
		constexpr Register MAX1         {0x01, 0xA4, 23}; // Max Recorded 2 — VRMS_S (S.23)
		constexpr Register MAX2         {0x01, 0xA7, 23}; // Max Recorded 3 — VRMS_T (S.23)
		constexpr Register MAX3         {0x01, 0xAA, 23}; // Max Recorded 4 — IRMS_R (S.23)
		constexpr Register MAX4         {0x01, 0xAD, 23}; // Max Recorded 5 — IRMS_S (S.23)
		constexpr Register MAX5         {0x01, 0xB0, 23}; // Max Recorded 6 — IRMS_T (S.23)
		constexpr Register MAX6         {0x01, 0xB3, 22}; // Max Recorded 7 — PF_T   (S.22)
		constexpr Register MAX7         {0x01, 0xB6, 16}; // Max Recorded 8 — FREQ   (S.16)

		/* ── Min/Max Monitor Address Config ──────────────── */
		constexpr Register MMADDR0      {0x01, 0xB9,  0}; // Min/Max Monitor address 1 (INT)
		constexpr Register MMADDR1      {0x01, 0xBC,  0}; // Min/Max Monitor address 2 (INT)
		constexpr Register MMADDR2      {0x01, 0xBF,  0}; // Min/Max Monitor address 3 (INT)
		constexpr Register MMADDR3      {0x01, 0xC2,  0}; // Min/Max Monitor address 4 (INT)
		constexpr Register MMADDR4      {0x01, 0xC5,  0}; // Min/Max Monitor address 5 (INT)
		constexpr Register MMADDR5      {0x01, 0xC8,  0}; // Min/Max Monitor address 6 (INT)
		constexpr Register MMADDR6      {0x01, 0xCB,  0}; // Min/Max Monitor address 7 (INT)
		constexpr Register MMADDR7      {0x01, 0xCE,  0}; // Min/Max Monitor address 8 (INT)

		/* ── Energy Bucket ────────────────────────────────── */
		constexpr Register BUCKET_L     {0x01, 0xD1,  0}; // Energy Bucket Size low word (INT)
		constexpr Register BUCKET_H     {0x01, 0xD4,  0}; // Energy Bucket Size high word (INT)

		/* ── Active Energy Counters ───────────────────────── */
		constexpr Register WHA_POS      {0x01, 0xDD,  0}; // Received Active Energy R (INT)
		constexpr Register WHA_NEG      {0x01, 0xE6,  0}; // Delivered Active Energy R (INT)
		constexpr Register WHB_POS      {0x01, 0xEF,  0}; // Received Active Energy S (INT)
		constexpr Register WHB_NEG      {0x01, 0xF8,  0}; // Delivered Active Energy S (INT)
		constexpr Register WHC_POS      {0x02, 0x01,  0}; // Received Active Energy T (INT)
		constexpr Register WHC_NEG      {0x02, 0x0A,  0}; // Delivered Active Energy T (INT)

		/* ── Reactive Energy Counters ─────────────────────── */
		constexpr Register VARHA_POS    {0x02, 0x13,  0}; // Reactive Energy Leading R (INT)
		constexpr Register VARHA_NEG    {0x02, 0x1C,  0}; // Reactive Energy Lagging R (INT)
		constexpr Register VARHB_POS    {0x02, 0x25,  0}; // Reactive Energy Leading S (INT)
		constexpr Register VARHB_NEG    {0x02, 0x2E,  0}; // Reactive Energy Lagging S (INT)
		constexpr Register VARHC_POS    {0x02, 0x37,  0}; // Reactive Energy Leading T (INT)
		constexpr Register VARHC_NEG    {0x02, 0x40,  0}; // Reactive Energy Lagging T (INT)

		/* ── System Status ────────────────────────────────────── */
		constexpr Register SYSSTAT      {0x02, 0x43,  0}; // SPI error status bits (INT) — bit 23 sticky

	} // namespace MAX78630Reg

#endif /* defined(__MAX78630_Registers__) */
