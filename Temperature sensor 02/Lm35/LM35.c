#include "LM35.h"                // Include the LM35 sensor driver header
#include "LM35_cfg.h"             // Include the LM35 configuration header
#include "../../util/DWT_Delay.h" // Include a delay utility header for the DWT (Data Watchpoint and Trace) delay function

static ADC_HandleTypeDef hadc = {0};        // Define the ADC handle to manage ADC configuration
static ADC_ChannelConfTypeDef sConfig = {0}; // Define the ADC channel configuration
static uint8_t calibrated = 0;               // Flag to indicate whether ADC calibration has been done

// LM35 sensor initialization function
void LM35_Init(uint16_t LM35_Instance)
{
    // Enable the clock for the GPIO port based on where the LM35 sensor is connected
    if(LM35_CfgParam[LM35_Instance].LM35_GPIO == GPIOA)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();  // Enable clock for GPIOA
    }
    else if(LM35_CfgParam[LM35_Instance].LM35_GPIO == GPIOB)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();  // Enable clock for GPIOB
    }
    else if(LM35_CfgParam[LM35_Instance].LM35_GPIO == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();  // Enable clock for GPIOC
    }
    else if (LM35_CfgParam[LM35_Instance].LM35_GPIO == GPIOD)
    {
        __HAL_RCC_GPIOD_CLK_ENABLE();  // Enable clock for GPIOD
    }

    // Configure the ADC instance for the LM35 sensor
    hadc.Instance = LM35_CfgParam[LM35_Instance].ADC_Instance;
    hadc.Init.ScanConvMode = ADC_SCAN_DISABLE; // Single channel conversion mode
    hadc.Init.ContinuousConvMode = DISABLE;    // Disable continuous conversion
    hadc.Init.DiscontinuousConvMode = DISABLE; // Disable discontinuous conversion
    hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START; // Software trigger for ADC conversion
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT; // Right data alignment (default)
    hadc.Init.NbrOfConversion = 1;             // Only one channel conversion
    HAL_ADC_Init(&hadc);                      // Initialize the ADC

    // Configure the ADC channel for the LM35 sensor
    sConfig.Channel = LM35_CfgParam[LM35_Instance].ADC_CH;
    sConfig.Rank = ADC_REGULAR_RANK_1;         // Set channel rank (first channel)
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5; // Set ADC sampling time
    HAL_ADC_ConfigChannel(&hadc, &sConfig);   // Apply the configuration to the ADC channel

    // Perform calibration only once (the first time initialization)
    if(calibrated == 0)
    {
        HAL_ADCEx_Calibration_Start(&hadc);  // Start ADC calibration
        calibrated = 1;                      // Set the calibrated flag to avoid future recalibrations
    }

    // Initialize the delay function
    DWT_Delay_Init();  // Initialize the DWT delay functionality (for delay functions)
}

// LM35 sensor reading function
float LM35_Read(uint16_t LM35_Instance)
{
    uint32_t AD_RES;        // Variable to store ADC conversion result
    float Temperature = 0;  // Variable to store the calculated temperature

    // Reconfigure the ADC channel for the specific sensor instance
    sConfig.Channel = LM35_CfgParam[LM35_Instance].ADC_CH;
    HAL_ADC_ConfigChannel(&hadc, &sConfig);

    // Start the ADC conversion process
    HAL_ADC_Start(&hadc);
    // Wait for the ADC conversion to complete
    HAL_ADC_PollForConversion(&hadc, 1);

    // Retrieve the conversion result from the ADC
    AD_RES = HAL_ADC_GetValue(&hadc);

    // Convert the ADC result to temperature in Celsius
    // The formula assumes the LM35 produces a voltage that is 10mV per degree Celsius
    // and the ADC resolution is 3.3V / 4096 steps = 0.000805664V per step
    Temperature = (AD_RES * 0.0805664); // Convert ADC value to temperature

    return Temperature;  // Return the calculated temperature
}
