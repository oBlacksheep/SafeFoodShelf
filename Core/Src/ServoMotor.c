/*Credits to the author for providing the code.
 * Author: Ehab Magdy Abdullah
 * Linkedin: https://www.linkedin.com/in/ehabmagdyy/
 * Youtube : https://www.youtube.com/@EhabMagdyy
 *                                                   */

#include "ServoMotor.h" // Include the servo motor header file

extern TIM_HandleTypeDef htim2; // Declare the external timer handler for Timer 2

// Initialize the servo motor to start at 0 degrees
void ServoMotor_Init(void)
{
	/* Initial position at 0 degree */
	SERVO_MOTOR_TIMER_TYPEDEF->SERVO_MOTOR_TIMER_CCR = 499; // Set initial PWM duty cycle for 0-degree position
	HAL_TIM_PWM_Start(SERVO_MOTOR_TIMER_HANDLER, SERVO_MOTOR_TIMER_CHANNEL); // Start PWM on the defined timer channel
}

// Set the servo motor to a specific angle (0 to 180 degrees)
void ServoMotor_Set_Angle(uint8_t angle)
{
	// Calculate the PWM duty cycle based on the desired angle and set it
	SERVO_MOTOR_TIMER_TYPEDEF->SERVO_MOTOR_TIMER_CCR = 499 + ((float)angle * (2000.0/180)); // Convert angle to duty cycle value
	HAL_TIM_PWM_Start(SERVO_MOTOR_TIMER_HANDLER, SERVO_MOTOR_TIMER_CHANNEL); // Start PWM on the defined timer channel
}
