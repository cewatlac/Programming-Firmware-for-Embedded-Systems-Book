#include "main.h"
// Includes the main header file, which contains the declarations for HAL functions and the necessary configurations.

void SystemClock_Config(void);
// Declares the function to configure the system clock. It sets up the clock sources and frequencies.

static void MX_GPIO_Init(void);
// Declares the function to initialize the GPIO (General Purpose Input/Output) pins.

int main(void)
// The main function, the entry point of the program.
{
  HAL_Init();
  // Initializes the HAL (Hardware Abstraction Layer) library, setting up SysTick and configuring the interrupt priorities.

  SystemClock_Config();
  // Calls the function to configure the system clock as defined in `SystemClock_Config`.

  MX_GPIO_Init();
  // Calls the function to initialize the GPIO pins as defined in `MX_GPIO_Init`.

  while (1)
  // Infinite loop for the main application logic.
  {
      // If Touch is Detected
      if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10))
      // Reads the state of GPIO pin PB10. If it is HIGH (logic 1), the condition is true.
      {
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
          // Turns ON the LED connected to GPIO pin PB12 by setting it to HIGH.
      }
      else
      {
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
          // Turns OFF the LED connected to GPIO pin PB12 by setting it to LOW.
      }
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
// Configures the system clock settings, including the oscillator source, clock multipliers, and dividers.
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  // Creates a structure to hold the oscillator configuration settings and initializes it to zero.

  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  // Creates a structure to hold the clock configuration settings and initializes it to zero.

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  // Specifies that the High-Speed External (HSE) oscillator will be used.

  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  // Enables the HSE oscillator.

  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  // Sets the prescaler value for the HSE to divide the input frequency by 1.

  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  // Enables the High-Speed Internal (HSI) oscillator.

  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  // Enables the Phase-Locked Loop (PLL).

  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  // Configures the PLL to use the HSE as its input source.

  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  // Sets the PLL multiplication factor to 9 to increase the frequency.

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  // Configures the oscillators and checks for errors. If an error occurs, the Error_Handler function is called.
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  // Specifies the clocks to configure: HCLK, SYSCLK, PCLK1, and PCLK2.

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  // Sets the PLL output as the system clock source.

  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  // Sets the AHB clock divider to 1 (no division).

  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  // Sets the APB1 clock divider to 2.

  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  // Sets the APB2 clock divider to 1 (no division).

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  // Configures the CPU and bus clocks and checks for errors. If an error occurs, the Error_Handler function is called.
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
// Configures the GPIO pins used in the program.
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  // Creates a structure to hold the GPIO configuration settings and initializes it to zero.

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  // Enables the clock for GPIO port D.

  __HAL_RCC_GPIOB_CLK_ENABLE();
  // Enables the clock for GPIO port B.

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  // Sets the initial state of GPIO pin PB12 (LED) to LOW (OFF).

  /*Configure GPIO pin : PB10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  // Specifies that pin PB10 will be configured.

  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  // Configures PB10 as an input pin.

  GPIO_InitStruct.Pull = GPIO_NOPULL;
  // Configures PB10 with no pull-up or pull-down resistors.

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  // Initializes GPIO pin PB10 with the specified settings.

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  // Specifies that pin PB12 will be configured.

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  // Configures PB12 as a push-pull output pin.

  GPIO_InitStruct.Pull = GPIO_NOPULL;
  // Configures PB12 with no pull-up or pull-down resistors.

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  // Sets the output speed for PB12 to low frequency.

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  // Initializes GPIO pin PB12 with the specified settings.
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
// Handles errors by disabling interrupts and entering an infinite loop.
{
  __disable_irq();
  // Disables all interrupts.

  while (1)
  {
    // Stays in this infinite loop to indicate an error.
  }
}
