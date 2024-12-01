#ifndef LM35_H_  // Ensure the file is included only once in the compilation process
#define LM35_H_

// Enable the ADC module to use it for the LM35 sensor
#define HAL_ADC_MODULE_ENABLED

#include "stm32f1xx_hal.h"  // Include STM32 HAL library header for STM32F1 series microcontrollers

// Define the number of LM35 sensors to be used in the project
#define SENSORS_NUM  1  // In this case, only one LM35 sensor is used

// Define a structure to hold the configuration for each LM35 sensor
typedef struct
{
    GPIO_TypeDef * LM35_GPIO;  // Pointer to the GPIO port where the LM35 sensor is connected
    ADC_TypeDef*   ADC_Instance;  // Pointer to the ADC instance (e.g., ADC1, ADC2) used to read the sensor
    uint32_t       ADC_CH;  // The ADC channel number used by the LM35 sensor
} LM35_CfgType;

// Function prototypes for LM35 sensor initialization and data reading
void LM35_Init(uint16_t LM35_Instance);  // Initializes the LM35 sensor instance (e.g., configuring GPIO, ADC)
float LM35_Read(uint16_t LM35_Instance);  // Reads the temperature value from the LM35 sensor and returns it as a float

#endif /* LM35_H_ */  // End the header guard
