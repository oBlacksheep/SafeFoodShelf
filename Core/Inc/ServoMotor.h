/*Credits to the author for providing the code.
 * Author: Ehab Magdy Abdullah
 * Linkedin: https://www.linkedin.com/in/ehabmagdyy/
 * Youtube : https://www.youtube.com/@EhabMagdyy
 *                                                   */

#ifndef INC_SERVOMOTOR_H_ // Check if the header file is already included
#define INC_SERVOMOTOR_H_ // Define the header file to avoid re-inclusion


#include "stm32f4xx_hal.h" // Include the STM32 HAL library for hardware abstraction functions

// Define constants related to the servo motor's timer settings
#define SERVO_MOTOR_TIMER_HANDLER		&htim2         // Timer handler for the servo motor (HTIM2)
#define SERVO_MOTOR_TIMER_CHANNEL		TIM_CHANNEL_1  // Timer channel 1 for controlling the servo
#define SERVO_MOTOR_TIMER_TYPEDEF		TIM2           // Define the timer type used (TIM2)
#define SERVO_MOTOR_TIMER_CCR			CCR1

// Function declarations
void ServoMotor_Init(void); // Function to initialize the servo motor
void ServoMotor_Set_Angle(uint8_t angle); // Function to set the servo motor's angle


#endif /* INC_SERVOMOTOR_H_ */ // End of the header guard
