/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// Includes the library for controlling an LCD display via I2C.
#include "liquidcrystal_i2c.h"

// Includes the standard string library for string manipulation functions.
#include <string.h>

// Includes the library for controlling a servo motor.
#include "ServoMotor.h"

// ARM Cortex-M4 core header file.
//Includes functions like NVIC_SystemReset(), used for performing a system reset.
#include "core_cm4.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// Initialize GPIO structure for keypad configuration
GPIO_InitTypeDef GPIO_InitStructPrivate = {0};

// Variables to track timing for debounce or other timing logic
uint32_t previousMillis = 0;
uint32_t currentMillis = 0;

// Variable to store the pressed key value
uint8_t keyPressed = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Display_WelcomeMessage()
{
	// Message to display on the LCD screen
	const char welcome_message[] = "Hello! Welcome (^-^) ... Press # to begin.  ";

	while(1)
	{
		LCD_Clear(); //Clear the display on LCD
		LCD_SetCursor(0, 0); //Set message of first row
		LCD_PrintStr(welcome_message); //display welcome message on LCD

		for (int x = 0; x < 50; x++)
		{
			LCD_ScrollDisplayLeft();  //shift the display message to the left
			HAL_Delay(300);

			// Check if the # key is pressed
			if (keyPressed == 35) // ASCII value of '#')
			{
				keyPressed = 0; // Reset keyPressed to avoid repeated detection
				return;
			}
		}
	}
}

char Display_SelectionMode()
{
	// Message to display on the LCD screen
	const char selection_display[] = "Select C or D.";
	const char selection_display2[] = "'C'=Customer      'D'=Deliverer      ";
	char buffer[17]; // Temporary buffer for 16 characters + null terminator
	char selected_mode = '\0'; // Initialize with no selection

	LCD_Clear(); //Clear the display on LCD
	LCD_SetCursor(0, 0); // Set message display to the first row
	LCD_PrintStr(selection_display);

	int length = strlen(selection_display2);
	while (selected_mode == '\0') // Keep looping until a C or D key is pressed
	{
		for (int x = 0; x < length; x++)
		{
			// Copy a portion of the text to the buffer
			for (int i = 0; i < 16; i++)
			{
				buffer[i] = selection_display2[(x + i) % length]; // Wrap text around
			}
			buffer[16] = '\0'; // Null-terminate the buffer

			// Display the scrolling second row
			LCD_SetCursor(0, 1); // Set message display to the 2nd row
			LCD_PrintStr(buffer);

			HAL_Delay(300); // Delay for smooth scrolling

			// Check if 'C' or 'D' key is pressed
			if (keyPressed == 67) // ASCII value of 'C'
			{
				selected_mode = 'C';
				keyPressed = 0; // Reset keyPressed to avoid repeated detection
				break; // Exit the loop once 'C' is pressed
			}
			else if (keyPressed == 68) // ASCII value of 'D'
			{
				selected_mode = 'D';
				keyPressed = 0; // Reset keyPressed to avoid repeated detection
				break; // Exit the loop once 'D' is pressed
			}
		}
	}
	return selected_mode;
}

char* Deliverer_SetKey(char* Deliverer_passcode)
{
	char buffer[17];
	int passcode_index = 0; // Index for passcode array

	// Message to display on the LCD screen
	const char DeliverMode_display[] = "Deliverer Mode";
	const char DeliverSetQ_display[] = "Please set a 4-key passcode:    ";

	LCD_Clear(); //Clear the display on LCD
	LCD_SetCursor(1, 0); // Set message display to the first row at centre
	LCD_PrintStr(DeliverMode_display);
	HAL_Delay(2000);

    LCD_Clear(); //Clear the display on LCD
    LCD_SetCursor(0, 0); // Set message display to the first row

	int DelSet_length = strlen(DeliverSetQ_display); //Get the length of the text

	while (passcode_index < 4)
	{
		// Scroll the "Enter 4-key passcode:" while waiting for input
		for (int x = 0; x < DelSet_length; x++)
		{
			// Display scrolling text
			for (int i = 0; i < 16; i++)
			{
				buffer[i] = DeliverSetQ_display[(x + i) % DelSet_length];
			}
			buffer[16] = '\0'; // Null-terminate the string

			LCD_SetCursor(0, 0); // Set message display to the first row
			LCD_PrintStr(buffer); // Display scrolling text

			// If a key is pressed, break scrolling
			if (keyPressed != 0)
			{
				if (keyPressed >= 48 && keyPressed <= 57) // Accept only accept numbers 0-9
				{
					// Store the pressed key
					Deliverer_passcode[passcode_index] = keyPressed;

					// Display the pressed key on 2nd row
					LCD_SetCursor(passcode_index, 1);
					LCD_PrintSpecialChar(keyPressed); // Show the actual number pressed

					passcode_index++;
				}
				keyPressed = 0; // Reset keyPressed
				break; // Break scrolling to update immediately
			}

			HAL_Delay(300); // Adjust scrolling speed
		}
	}


	Deliverer_passcode[4] = '\0'; // Null-terminate the passcode string
	return Deliverer_passcode;
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  LCD_Init(2); // Initialize the LCD display with 2 rows
  ServoMotor_Init(); // Initialize the servo motor for operation
  HAL_Delay(1000); // Wait for 1 second to ensure proper initialization

  //initialise keypad columns  as outputs and set them high
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1); //Column 4 = PA15 as high
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1); //Column 3 = PB3 as high
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1); //Column 2 = PB4 as high
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1); //Column 1 = PB13 as high

  //initial state of servo motor
  ServoMotor_Set_Angle(90); //set angle of servo motor at 90 degrees (door lock)
  HAL_Delay(2000);

  //initial state of LED
  //Green_LED ON = Available (no food inside)
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // Green_LED ON
  HAL_Delay(1000);

  //Red_LED OFF. If red LED ON means there's food inside)
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // Red_LED OFF
  HAL_Delay(1000);

  char Result_SetPasscode[5] = {0}; // Store the passcode set by the deliverer
  char Customer_passcode[5] = {0}; // Store the passcode set by the customer
  int t = 0; // Track the number of incorrect attempts

  while(1) //infinite loop
  {
	  //Display welcome message on LCD
	  Display_WelcomeMessage();

  	  // Display selection screen and return selection choice after # is pressed (inside Display_WelcomeMessage function)
  	  char selected_mode = Display_SelectionMode();

  	  if (selected_mode == 'C') //CustomerMode;
  	  {
  		  // Message to display on the LCD screen
  		  const char CustomerCode_display[] = "Please enter the 4-key passcode:    ";
			char cust_buffer[17];
			int cust_passcode_index = 0; // Index for passcode array

			LCD_Clear(); //Clear the display on LCD
			LCD_SetCursor(1, 0); // Set message display to the first row at centre
			LCD_PrintStr("Customer Mode");
			HAL_Delay(2000); //Display for 2 seconds

			LCD_Clear(); //Clear the display on LCD

			t = 0; //reset retry counter

			while (t < 3) // Allow up to 3 attempts
			{
				LCD_SetCursor(0,0);
				int CusSet_length = strlen(CustomerCode_display); //get length of text
				while (cust_passcode_index < 4)
				{
					// Scroll the "Please Enter the 4-key passcode:" while waiting for input
					for (int x = 0; x < CusSet_length; x++)
					{
						// Display scrolling text
						for (int i = 0; i < 16; i++)
						{
							cust_buffer[i] = CustomerCode_display[(x + i) % CusSet_length];
						}
						cust_buffer[16] = '\0'; // Null-terminate the string

						LCD_SetCursor(0, 0); // Set message display to the first row
						LCD_PrintStr(cust_buffer); // Display scrolling text

						// If a key is pressed, break scrolling
						if (keyPressed != 0)
						{
							if (keyPressed >= 48 && keyPressed <= 57) // Accept only accept numbers 0-9
							{
								// Store the pressed key
								Customer_passcode[cust_passcode_index] = keyPressed;

								// Display the pressed key on 2nd row
								LCD_SetCursor(cust_passcode_index, 1); //display the keypressed according the the column
								LCD_PrintSpecialChar(keyPressed); // Show the actual number pressed

								cust_passcode_index++;
							}
							keyPressed = 0; // Reset keyPressed
							break; // Break scrolling to update immediately
						}

						HAL_Delay(300); // Adjust scrolling speed
					}
				}


			Customer_passcode[4] = '\0'; // Null-terminate the passcode string

			//compare the customer entered passcode and the deliverer set peasscode
			if (strcmp(Customer_passcode, Result_SetPasscode) != 0) //if wrong passcode entered
			{
				LCD_Clear(); //Clear the display on LCD
				LCD_SetCursor(0, 0); // Set message display to the first row
				LCD_PrintStr("Invalid passcode!");
				HAL_Delay(2000);
				t+=1;
				cust_passcode_index = 0; // Reset passcode entry
			}
			else //if correct passcode entered
			{
				LCD_Clear(); //Clear the display on LCD
				LCD_SetCursor(0, 0);// Set message display to the first row
				LCD_PrintStr("Passcode reset!!"); //display passcode reset
				HAL_Delay(2000);
				Result_SetPasscode[0] = '\0'; // Clear previous deliverer passcode

				LCD_Clear(); //Clear the display on LCD
				LCD_SetCursor(3,0); // Set message display to the first row at centre
				LCD_PrintStr("Door open.");
				HAL_Delay(2000); //Display for 2 seconds

				//motor turns to unlock
				ServoMotor_Set_Angle(0); //set angle of servo motor at 0 degrees (door unlock)
				HAL_Delay(2000);

				//turn off Red_LED
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // Turn off Red_LED
				HAL_Delay(1000);

				LCD_Clear(); //Clear the display on LCD
				LCD_SetCursor(0,0); // Set message display to the first row
				LCD_PrintStr("Please take out");
				LCD_SetCursor(0,1);
				LCD_PrintStr("your food."); // Set message display to the 2nd row
				HAL_Delay(5000); //need make it longer maybe

				LCD_Clear(); //Clear the display on LCD
				LCD_SetCursor(0,0); // Set message display to the first row
				LCD_PrintStr("Close the door &");
				LCD_SetCursor(0,1); // Set message display to the 2nd row
				LCD_PrintStr("press # to lock.");

				// Wait for user to press '#'
				while (1)
				{
					if (keyPressed == 35) // ASCII value of '#'
					{
						//motor turns to lock
						ServoMotor_Set_Angle(90); //set angle of servo motor at 90 degrees (door lock)
						HAL_Delay(2000);

						//display of LCD
						LCD_Clear(); //Clear the display on LCD
						LCD_SetCursor(2, 0); // Set message display to the first row at centre
						LCD_PrintStr("Thank You!!!");
						LCD_SetCursor(0, 1); // Set message display to the 2nd row
						LCD_PrintStr("Have a nice day.");
						HAL_Delay(2000);

						keyPressed = 0; // Reset keyPressed

						//Green_LED ON = Available (No food inside)
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // Turn on Green_LED
						HAL_Delay(1000);
						break; // Exit the loop after confirmation
					}
					HAL_Delay(100); // Polling delay
				}
				break;
			}
		}
		//loops back to beginning
	}
	else if (selected_mode == 'D') //Deliver mode
	{
		char passcode[5]; // Array to store the passcode

		Result_SetPasscode[0] = '\0'; // Clear previous passcode

		Deliverer_SetKey(passcode);
		strcpy(Result_SetPasscode, passcode); // Store the new passcode

		// Display the entered passcode and ask for confirmation
		LCD_Clear(); //Clear the display on LCD
		LCD_SetCursor(0, 0); // Set message display to the first row
		LCD_PrintStr("Passcode:");
		LCD_SetCursor(11, 0); // Set message display to the first row beside "Passcode:"
		LCD_PrintStr(Result_SetPasscode);

		LCD_SetCursor(0, 1); // Set message display to the 2nd row
		LCD_PrintStr("Confirm? Press #");

		// Wait for user to press '#'
		while (1)
		{
			if (keyPressed == 35) // ASCII value of '#'
			{
				LCD_Clear(); //Clear the display on LCD
				LCD_SetCursor(0, 0); // Set message display to the first row
				LCD_PrintStr("Passcode Confirmed!");
				HAL_Delay(2000);
				keyPressed = 0; // Reset keyPressed
				break; // Exit the loop after confirmation
			}
			HAL_Delay(100); // Polling delay
		}

		LCD_Clear(); //Clear the display on LCD
		LCD_SetCursor(3,0); // Set message display to the first row at centre
		LCD_PrintStr("Door open.");
		HAL_Delay(2000);

		//Motor turns unlock
		ServoMotor_Set_Angle(0); //set angle of servo motor at 0 degrees (door unlock)
		HAL_Delay(2000);

		//Green_LED OFF
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // Turn off Green_LED
		HAL_Delay(1000);

		LCD_Clear(); //Clear the display on LCD
		LCD_SetCursor(2,0); //display on first row
		LCD_PrintStr("Please insert");
		LCD_SetCursor(0,1); //display on second row
		LCD_PrintStr("the food inside.");
		HAL_Delay(5000);

		LCD_Clear(); //Clear the display on LCD after 5 seconds of previous display
		LCD_SetCursor(0,0); //display on first row
		LCD_PrintStr("Close the door &");
		LCD_SetCursor(0,1); //display on second row
		LCD_PrintStr("press # to lock.");

		// Wait for user to press '#'
		while (1)
		{
			if (keyPressed == 35) // ASCII value of '#'
			{
				//motor turns to lock
				ServoMotor_Set_Angle(90); //set angle of servo motor at 90 degrees (door lock)
				HAL_Delay(2000);

				//display on LCD
				LCD_Clear();
				LCD_SetCursor(2, 0); // Set message display to the first row at centre
				LCD_PrintStr("Thank You!!!");
				LCD_SetCursor(0, 1); // Set message display to the 2nd row
				LCD_PrintStr("Have a nice day.");
				HAL_Delay(2000);

				keyPressed = 0; // Reset keyPressed

				//Red_LED ON = Not available (Food inside)
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // Turn on Red_LED
				HAL_Delay(1000);
				break; // Exit the loop after confirmation
			}
			HAL_Delay(100); // Polling delay
		}
		//loops back to beginning
	}
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

/* USER CODE BEGIN 4 */
// Callback function for GPIO external interrupt (keypad settings)
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	currentMillis = HAL_GetTick(); // Get the current system tick count

	// Check if 10ms have passed since the last interrupt to debounce the keypad
	if (currentMillis - previousMillis > 10)
	{
		// Configure GPIO pins PB6, PB7, PB12, PB14 as input pins for reading keypad rows
		GPIO_InitStructPrivate.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_12|GPIO_PIN_14;
		GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT; // Set pins as input
		GPIO_InitStructPrivate.Pull = GPIO_NOPULL; // No pull-up or pull-down resistors
		GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW; // Low frequency for stable input
		HAL_GPIO_Init(GPIOB, &GPIO_InitStructPrivate); // Initialize the pins

		// Set Column 4 high and Columns 1-3 low for scanning the keypad
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1); // Set Column 4 high
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0); // Set Column 3 low
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0); // Set Column 2 low
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0); // Set Column 1 low

		// Check which row is active and set the corresponding key value
		if(GPIO_Pin == GPIO_PIN_6 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6))
		{
			keyPressed = 68; //ASCII value of D
		}
		else if(GPIO_Pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7))
		{
			keyPressed = 67; //ASCII value of C
		}
		else if(GPIO_Pin == GPIO_PIN_12 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12))
		{
			keyPressed = 66; //ASCII value of B
		}
		else if(GPIO_Pin == GPIO_PIN_14 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14))
		{
			keyPressed = 65; //ASCII value of A
			NVIC_SystemReset(); // Trigger a system reset
		}

		// Set Column 3 high and Columns 4 and 1-2 low for scanning the keypad
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0); // Set Column 4 low
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1); // Set Column 3 high
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0); // Set Column 2 low
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0); // Set Column 1 low

		// Check which row is active and set the corresponding key value
		if(GPIO_Pin == GPIO_PIN_6 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6))
		{
			keyPressed = 35; //ASCII value of #
		}
		else if(GPIO_Pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7))
		{
			keyPressed = 57; //ASCII value of 9
		}
		else if(GPIO_Pin == GPIO_PIN_12 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12))
		{
			keyPressed = 54; //ASCII value of 6
		}
		else if(GPIO_Pin == GPIO_PIN_14 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14))
		{
			keyPressed = 51; //ASCII value of 3
		}

		// Set Column 2 high and Columns 3-4 and 1 low for scanning the keypad
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0); // Set Column 4 low
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0); // Set Column 3 low
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1); // Set Column 2 high
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0); // Set Column 1 low

		// Check which row is active and set the corresponding key value
		if(GPIO_Pin == GPIO_PIN_6 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6))
		{
			keyPressed = 48; //ASCII value of 0
		}
		else if(GPIO_Pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7))
		{
			keyPressed = 56; //ASCII value of 8
		}
		else if(GPIO_Pin == GPIO_PIN_12 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12))
		{
			keyPressed = 53; //ASCII value of 5
		}
		else if(GPIO_Pin == GPIO_PIN_14 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14))
		{
			keyPressed = 50; //ASCII value of 2
		}

		// Set Column 1 high and Columns 2-4 low for scanning the keypad
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0); // Set Column 4 low
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0); // Set Column 3 low
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0); // Set Column 2 low
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1); // Set Column 1 high

		// Check which row is active and set the corresponding key value
		if(GPIO_Pin == GPIO_PIN_6 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6))
		{
			keyPressed = 42; //ASCII value of *
		}
		else if(GPIO_Pin == GPIO_PIN_7 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7))
		{
			keyPressed = 55; //ASCII value of 7
		}
		else if(GPIO_Pin == GPIO_PIN_12 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12))
		{
			keyPressed = 52; //ASCII value of 4
		}
		else if(GPIO_Pin == GPIO_PIN_14 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14))
		{
			keyPressed = 49; //ASCII value of 1
		}

		// Set all columns (1-4) back to high to reset the keypad for the next scan
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1);

		// Reconfigure GPIO pins PB6, PB7, PB12, PB14 to EXTI (external interrupt) mode
		GPIO_InitStructPrivate.Mode = GPIO_MODE_IT_RISING; // Interrupt on rising edge
		GPIO_InitStructPrivate.Pull = GPIO_PULLDOWN;  // Enable pull-down resistors
		HAL_GPIO_Init(GPIOB, &GPIO_InitStructPrivate);

		// Update previousMillis with the current tick to manage debounce timing
		previousMillis = currentMillis;
	}
}
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
