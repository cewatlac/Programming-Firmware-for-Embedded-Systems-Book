#include "SERVO.h"  
// Includes the header file for the SERVO module, where the `SERVO_CfgType` struct and function prototypes are defined.

const SERVO_CfgType SERVO_CfgParam[SERVO_NUM] =  
// Defines an array of `SERVO_CfgType` structures to hold configuration details for each servo motor.
// Since `SERVO_NUM` is set to 1, this array has a single element.

{
	// Servo Motor 1 Configurations
    {
	    GPIOA,              // The GPIO port for the servo's PWM signal (in this case, GPIOA).
		GPIO_PIN_0,         // The GPIO pin on the selected port (in this case, pin 0).
		TIM2,               // Timer instance used to generate the PWM signal (in this case, TIM2).
		&TIM2->CCR1,        // Pointer to the compare register for timer channel 1.
		TIM_CHANNEL_1,      // Timer channel used for the PWM output (in this case, channel 1 of TIM2).
		72000000,           // Timer clock frequency in Hz (72 MHz for this configuration).
		0.65,               // Minimum pulse width in milliseconds (corresponding to 0 degrees).
		2.3                 // Maximum pulse width in milliseconds (corresponding to 180 degrees).
	}
};
