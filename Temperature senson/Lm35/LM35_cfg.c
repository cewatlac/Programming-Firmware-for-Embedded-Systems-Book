#include "LM35.h"  // Include the LM35 sensor driver header file for interfacing with the LM35 temperature sensor

// Define the configuration parameters for the LM35 sensor(s)
const LM35_CfgType LM35_CfgParam[SENSORS_NUM] =
{
    // Configuration for the first LM35 sensor
    {
        GPIOA,              // The GPIO port where the sensor is connected (GPIO port A)
        ADC1,               // The ADC peripheral used to read the sensor value (ADC1)
        ADC_CHANNEL_7       // The ADC channel number for the sensor (Channel 7)
    }
};
