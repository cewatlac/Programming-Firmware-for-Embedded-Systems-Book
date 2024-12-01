#include "SERVO.h"
// Include the header file for servo control, which contains the required declarations.

#include "SERVO_cfg.h"
// Include the configuration file for servo motor settings and parameters.

#include "../../util/DWT_Delay.h"
// Include a utility file for delay functions based on the DWT (Data Watchpoint and Trace) timer.

typedef struct
{
    uint16_t Period_Min; // Minimum pulse width corresponding to 0 degrees.
    uint16_t Period_Max; // Maximum pulse width corresponding to 180 degrees.
} SERVO_info;

static SERVO_info gs_SERVO_info[SERVO_NUM] = {0};
// Array to store the minimum and maximum pulse information for each servo motor instance.

void SERVO_Init(uint16_t au16_SERVO_Instance)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0}; // Struct for configuring GPIO pins.
    TIM_ClockConfigTypeDef sClockSourceConfig = {0}; // Struct for timer clock source configuration.
    TIM_MasterConfigTypeDef sMasterConfig = {0}; // Struct for timer master configuration.
    TIM_OC_InitTypeDef sConfigOC = {0}; // Struct for timer output compare configuration.
    TIM_HandleTypeDef htim; // Timer handle for managing the timer instance.
    uint32_t PSC_Value = 0; // Prescaler value for timer configuration.
    uint32_t ARR_Value = 0; // Auto-reload value for timer configuration.

    DWT_Delay_Init();
    // Initialize the DWT delay utility for precise timing.

    /*--------[ Configure The Servo PWM GPIO Pin ]-------*/
    // Enable the clock for the GPIO port used by the servo and configure the GPIO pin.
    if (SERVO_CfgParam[au16_SERVO_Instance].SERVO_GPIO == GPIOA)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE(); // Enable clock for GPIOA.
    }
    else if (SERVO_CfgParam[au16_SERVO_Instance].SERVO_GPIO == GPIOB)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE(); // Enable clock for GPIOB.
    }
    else if (SERVO_CfgParam[au16_SERVO_Instance].SERVO_GPIO == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE(); // Enable clock for GPIOC.
    }
    else if (SERVO_CfgParam[au16_SERVO_Instance].SERVO_GPIO == GPIOD)
    {
        __HAL_RCC_GPIOD_CLK_ENABLE(); // Enable clock for GPIOD.
    }
    else if (SERVO_CfgParam[au16_SERVO_Instance].SERVO_GPIO == GPIOE)
    {
        __HAL_RCC_GPIOE_CLK_ENABLE(); // Enable clock for GPIOE.
    }

    GPIO_InitStruct.Pin = SERVO_CfgParam[au16_SERVO_Instance].SERVO_PIN;
    // Configure the specific pin for PWM output.
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // Set mode as alternate function, push-pull.
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Set GPIO speed to low frequency.
    HAL_GPIO_Init(SERVO_CfgParam[au16_SERVO_Instance].SERVO_GPIO, &GPIO_InitStruct);
    // Initialize the GPIO pin with the above settings.

    /*--------[ Calculate The PSC & ARR Values To Maximize PWM Resolution ]-------*/
    // Calculate timer prescaler and auto-reload values for 50 Hz PWM signal.
    PSC_Value = (uint32_t)(SERVO_CfgParam[au16_SERVO_Instance].TIM_CLK / 3276800.0);
    ARR_Value = (uint32_t)((SERVO_CfgParam[au16_SERVO_Instance].TIM_CLK / (50.0 * (PSC_Value + 1.0))) - 1.0);

    /*--------[ Configure The Servo PWM Timer Channel ]-------*/
    // Enable the timer clock and configure the timer for PWM generation.
    if (SERVO_CfgParam[au16_SERVO_Instance].TIM_Instance == TIM1)
    {
        __HAL_RCC_TIM1_CLK_ENABLE(); // Enable clock for TIM1.
    }
    else if (SERVO_CfgParam[au16_SERVO_Instance].TIM_Instance == TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE(); // Enable clock for TIM2.
    }
    else if (SERVO_CfgParam[au16_SERVO_Instance].TIM_Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE(); // Enable clock for TIM3.
    }
    else if (SERVO_CfgParam[au16_SERVO_Instance].TIM_Instance == TIM4)
    {
        __HAL_RCC_TIM4_CLK_ENABLE(); // Enable clock for TIM4.
    }

    htim.Instance = SERVO_CfgParam[au16_SERVO_Instance].TIM_Instance;
    // Set the timer instance for the servo motor.
    htim.Init.Prescaler = PSC_Value; // Set the calculated prescaler value.
    htim.Init.CounterMode = TIM_COUNTERMODE_UP; // Set counter mode to up-counting.
    htim.Init.Period = ARR_Value; // Set the calculated auto-reload value.
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // No clock division.
    htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // Enable auto-reload preload.

    HAL_TIM_Base_Init(&htim); // Initialize the timer base.
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL; // Use internal clock source.
    HAL_TIM_ConfigClockSource(&htim, &sClockSourceConfig); // Configure the clock source.
    HAL_TIM_PWM_Init(&htim); // Initialize the timer for PWM mode.
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET; // No trigger output.
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE; // Disable master-slave mode.
    HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig); // Configure synchronization.
    sConfigOC.OCMode = TIM_OCMODE_PWM1; // Set PWM mode 1.
    sConfigOC.Pulse = 0; // Initialize with zero pulse width.
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH; // Set active high polarity.
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE; // Disable fast mode.
    HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, SERVO_CfgParam[au16_SERVO_Instance].PWM_TIM_CH);
    // Configure the timer output channel for PWM.

    /*--------[ Calculate & Save The Servo Pulse Information ]-------*/
    // Calculate the minimum and maximum pulse widths for the servo motor.
    gs_SERVO_info[au16_SERVO_Instance].Period_Min = (uint16_t)(ARR_Value * (SERVO_CfgParam[au16_SERVO_Instance].MinPulse / 20.0));
    gs_SERVO_info[au16_SERVO_Instance].Period_Max = (uint16_t)(ARR_Value * (SERVO_CfgParam[au16_SERVO_Instance].MaxPulse / 20.0));

    /*--------[ Start The PWM Channel ]-------*/
    HAL_TIM_PWM_Start(&htim, SERVO_CfgParam[au16_SERVO_Instance].PWM_TIM_CH);
    // Start the PWM signal generation on the configured timer channel.
}

void SERVO_MoveTo(uint16_t au16_SERVO_Instance, float af_Angle)
{
    uint16_t au16_Pulse = 0;

    // Calculate the pulse width for the desired angle (scaled between 0 and 180 degrees).
    au16_Pulse = ((af_Angle * (gs_SERVO_info[au16_SERVO_Instance].Period_Max - gs_SERVO_info[au16_SERVO_Instance].Period_Min)) / 180.0)
                 + gs_SERVO_info[au16_SERVO_Instance].Period_Min;

    *(SERVO_CfgParam[au16_SERVO_Instance].TIM_CCRx) = au16_Pulse;
    // Set the timer compare register with the calculated pulse width.
}

void SERVO_RawMove(uint16_t au16_SERVO_Instance, uint16_t au16_Pulse)
{
    // Move the servo using a raw pulse width if it's within the valid range.
    if (au16_Pulse <= gs_SERVO_info[au16_SERVO_Instance].Period_Max && au16_Pulse >= gs_SERVO_info[au16_SERVO_Instance].Period_Min)
    {
        *(SERVO_CfgParam[au16_SERVO_Instance].TIM_CCRx) = au16_Pulse;
    }
}

uint16_t SERVO_Get_MaxPulse(uint16_t au16_SERVO_Instance)
{
    return (gs_SERVO_info[au16_SERVO_Instance].Period_Max);
    // Return the maximum pulse width for the servo.
}

uint16_t SERVO_Get_MinPulse(uint16_t au16_SERVO_Instance)
{
    return (gs_SERVO_info[au16_SERVO_Instance].Period_Min);
    // Return the minimum pulse width for the servo.
}

void SERVO_Sweep(uint16_t au16_SERVO_Instance)
{
    uint8_t au8_Angle = 0;

    SERVO_MoveTo(au16_SERVO_Instance, 0);
    // Move the servo to 0 degrees.

    DWT_Delay_ms(250); // Wait for 250 ms.
    while (au8_Angle < 180)
    {
        SERVO_MoveTo(au16_SERVO_Instance, au8_Angle++);
        // Incrementally move the servo to 180 degrees.
        DWT_Delay_ms(5); // Wait for 5 ms between steps.
    }
    DWT_Delay_ms(250); // Wait for 250 ms.
    while (au8_Angle > 0)
    {
        SERVO_MoveTo(au16_SERVO_Instance, au8_Angle--);
        // Incrementally move the servo back to 0 degrees.
        DWT_Delay_ms(5); // Wait for 5 ms between steps.
    }
}
