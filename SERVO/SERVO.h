#ifndef SERVO_H_  
#define SERVO_H_  
// Include guards to prevent multiple inclusions of this header file.

#define HAL_TIM_MODULE_ENABLED  
// Enable the TIM (Timer) module in the HAL library for STM32.

#include "stm32f1xx_hal.h"  
// Include the HAL (Hardware Abstraction Layer) header for the STM32F1 series microcontroller.

#define SERVO_NUM  1  
// Define the number of servo motors to be used in the project. Here, only one servo is configured.

typedef struct  
{  
	GPIO_TypeDef * SERVO_GPIO;  // Pointer to the GPIO port (e.g., GPIOA, GPIOB) used for the servo.
	uint16_t       SERVO_PIN;   // GPIO pin number connected to the servo PWM signal.
	TIM_TypeDef*   TIM_Instance; // Pointer to the timer instance controlling the PWM signal.
	uint32_t*      TIM_CCRx;    // Pointer to the timer's compare register for setting the PWM duty cycle.
	uint32_t       PWM_TIM_CH;  // Timer channel used for PWM output.
	uint32_t       TIM_CLK;     // Clock frequency of the timer.
	float          MinPulse;    // Minimum pulse width (in milliseconds) corresponding to 0 degrees.
	float          MaxPulse;    // Maximum pulse width (in milliseconds) corresponding to 180 degrees.
} SERVO_CfgType;  
// Struct to define the configuration for a servo motor. Each servo will have its settings stored in an instance of this struct.

/*-----[ Prototypes For All Functions ]-----*/  
// Function prototypes for controlling the servo motor.

void SERVO_Init(uint16_t au16_SERVO_Instance);  
// Initializes the specified servo motor instance by configuring the GPIO pin, timer, and PWM.

void SERVO_MoveTo(uint16_t au16_SERVO_Instance, float af_Angle);  
// Moves the specified servo motor instance to a given angle (0 to 180 degrees).

void SERVO_RawMove(uint16_t au16_SERVO_Instance, uint16_t au16_Pulse);  
// Directly moves the servo motor using a raw pulse width value, bypassing angle calculations.

uint16_t SERVO_Get_MaxPulse(uint16_t au16_SERVO_Instance);  
// Returns the maximum pulse width value for the specified servo motor instance.

uint16_t SERVO_Get_MinPulse(uint16_t au16_SERVO_Instance);  
// Returns the minimum pulse width value for the specified servo motor instance.

void SERVO_Sweep(uint16_t au16_SERVO_Instance);  
// Sweeps the specified servo motor instance from 0 to 180 degrees and back to 0.

#endif /* SERVO_H_ */  
// End of include guard.
