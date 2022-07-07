/**
 * @brief Measurement Structure update
 */
#define Measurement_Frequency
#define Measurement_Voltage_R
#define Measurement_Voltage_S
#define Measurement_Voltage_T
#define Measurement_Current_R
#define Measurement_Current_S
#define Measurement_Current_T
//#define Measurement_PowerFactor_R
//#define Measurement_PowerFactor_S
//#define Measurement_PowerFactor_T
#define Measurement_PowerFactor_A
#define Measurement_Energy_Active
#define Measurement_Energy_ReActive

/**
 * @brief Limit Defaults Definition.
 */
#define Limit_Voltage_Min       (float)192
#define Limit_Voltage_Max       (float)253
#define Limit_Frequency_Min     (float)47
#define Limit_Frequency_Max     (float)52
#define Limit_Voltage_IMB       (float)0.06
#define Limit_Current_IMB		(float)0.06
#define Bucket_High				(uint32_t)0x000821
#define Bucket_Low				(uint32_t)0xD0F4C2

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

    } Energy;

} Measurement;
