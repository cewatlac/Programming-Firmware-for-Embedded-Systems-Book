#include "main.h"

#define CAPTOUCH_SENSORS	2
#define TOUCH_THRESHOLD		3000
#define RAW_BUFFER_LEN		10

typedef struct {
	GPIO_TypeDef* DriveGPIO;
	uint16_t DrivePIN;
	GPIO_TypeDef* SenseGPIO;
	uint16_t SensePIN;
	uint32_t CapTouch_Raw[RAW_BUFFER_LEN];
} CapTouchSensor_s;

TIM_HandleTypeDef htim2;
CapTouchSensor_s MyTouchSensors[CAPTOUCH_SENSORS];
uint8_t CapTouch_Buffer_Index = 0;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void CapTouch_Sample();
static void CapTouch_Handler();
static uint8_t CapTouch_Filtered_State(uint8_t);

int main(void)
{
  // CapTouch Sensor1 Pins
  MyTouchSensors[0].DriveGPIO = GPIOB;
  MyTouchSensors[0].DrivePIN = GPIO_PIN_10;
  MyTouchSensors[0].SenseGPIO = GPIOA;
  MyTouchSensors[0].SensePIN = GPIO_PIN_5;
  // CapTouch Sensor2 Pins
  MyTouchSensors[1].DriveGPIO = GPIOB;
  MyTouchSensors[1].DrivePIN = GPIO_PIN_11;
  MyTouchSensors[1].SenseGPIO = GPIOA;
  MyTouchSensors[1].SensePIN = GPIO_PIN_6;
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM2_Init();

  while (1)
  {
	  CapTouch_Sample();
	  CapTouch_Handler();
	  HAL_Delay(1);
  }
}

static void CapTouch_Sample()
{
	// Sample All CapTouch Sensors
	for(uint8_t i=0; i<CAPTOUCH_SENSORS; i++)
	{
		TIM2->CNT = 0;
		HAL_GPIO_WritePin(MyTouchSensors[i].DriveGPIO, MyTouchSensors[i].DrivePIN, GPIO_PIN_SET);
		HAL_TIM_Base_Start(&htim2);
		while(!HAL_GPIO_ReadPin(MyTouchSensors[i].SenseGPIO, MyTouchSensors[i].SensePIN));
		MyTouchSensors[i].CapTouch_Raw[CapTouch_Buffer_Index] = TIM2->CNT;
		HAL_TIM_Base_Stop(&htim2);
		HAL_GPIO_WritePin(MyTouchSensors[i].DriveGPIO, MyTouchSensors[i].DrivePIN, GPIO_PIN_RESET);
	}
	// Update The Buffer Index
	CapTouch_Buffer_Index++;
	if(CapTouch_Buffer_Index == RAW_BUFFER_LEN){
		CapTouch_Buffer_Index = 0;
	}
}

static uint8_t CapTouch_Filtered_State(uint8_t CapTouch_SensorIndex)
{
	for(uint8_t i=0; i<RAW_BUFFER_LEN; i++)
	{
		if(MyTouchSensors[CapTouch_SensorIndex].CapTouch_Raw[i] >= TOUCH_THRESHOLD){
			return 1;
		}
	}
	return 0;
}

static void CapTouch_Handler()
{
	if(CapTouch_Filtered_State(0)){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // LED1 ON
	}
	else{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // LED1 OFF
	}
	if(CapTouch_Filtered_State(1)){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // LED2 ON
	}
	else{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // LED2 OFF
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 24;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA5 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB10 PB11 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
