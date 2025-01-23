/*Credits to the author for providing the code.
 * Author: Jihoon Lee (eziya)
 * Linkedin: https://www.linkedin.com/in/jihoon-lee-05718299/
 * Github Profile: https://github.com/eziya
 *                                                   */
#ifndef LIQUIDCRYSTAL_I2C_H_ // Check if the header file is already included
#define LIQUIDCRYSTAL_I2C_H_ // Define the header file to avoid re-inclusion

#include "stm32f4xx_hal.h" // Include STM32 HAL library for hardware abstraction layer

/* Command Definitions for LCD Operations */
#define LCD_CLEARDISPLAY 0x01    // Command to clear the display
#define LCD_RETURNHOME 0x02      // Command to return the cursor to the home position
#define LCD_ENTRYMODESET 0x04    // Command to set the entry mode (text direction)
#define LCD_DISPLAYCONTROL 0x08  // Command to control the display (on/off, cursor, blink)
#define LCD_CURSORSHIFT 0x10     // Command to shift the cursor
#define LCD_FUNCTIONSET 0x20     // Command to set the display function (data length, lines, etc.)
#define LCD_SETCGRAMADDR 0x40    // Command to set CGRAM address
#define LCD_SETDDRAMADDR 0x80    // Command to set DDRAM address

/* Entry Mode */
#define LCD_ENTRYRIGHT 0x00   // Set entry mode to right-to-left
#define LCD_ENTRYLEFT 0x02    // Set entry mode to left-to-right
#define LCD_ENTRYSHIFTINCREMENT 0x01   // Shift text right
#define LCD_ENTRYSHIFTDECREMENT 0x00   // Shift text left

/* Display On/Off */
#define LCD_DISPLAYON 0x04     // Turn on the display
#define LCD_DISPLAYOFF 0x00    // Turn off the display
#define LCD_CURSORON 0x02      // Turn on the cursor
#define LCD_CURSOROFF 0x00     // Turn off the cursor
#define LCD_BLINKON 0x01       // Turn on the cursor blink
#define LCD_BLINKOFF 0x00      // Turn off the cursor blink

/* Cursor Shift */
#define LCD_DISPLAYMOVE 0x08   // Move the display
#define LCD_CURSORMOVE 0x00    // Move the cursor
#define LCD_MOVERIGHT 0x04     // Move the cursor/ display right
#define LCD_MOVELEFT 0x00      // Move the cursor/ display left

/* Function Set */
#define LCD_8BITMODE 0x10      // Use 8-bit data mode
#define LCD_4BITMODE 0x00      // Use 4-bit data mode
#define LCD_2LINE 0x08         // Set display to 2 lines
#define LCD_1LINE 0x00         // Set display to 1 line
#define LCD_5x10DOTS 0x04      // Use 5x10 dot font
#define LCD_5x8DOTS 0x00       // Use 5x8 dot font

/* Backlight */
#define LCD_BACKLIGHT 0x08     // Turn on backlight
#define LCD_NOBACKLIGHT 0x00   // Turn off backlight

/* Enable Bit */
#define ENABLE 0x04

/* Read Write Bit */
#define RW 0x0

/* Register Select Bit */
#define RS 0x01

/* Device I2C Address */
#define DEVICE_ADDR     (0x27 << 1)

// Function Prototypes for LCD operations
void LCD_Init(uint8_t rows);   // Initialize the LCD with specified row count
void LCD_Clear();              // Clear the LCD display
void LCD_Home();               // Return cursor to the home position
void LCD_NoDisplay();          // Turn off the display
void LCD_Display();            // Turn on the display
void LCD_NoBlink();            // Disable blinking cursor
void LCD_Blink();              // Enable blinking cursor
void LCD_NoCursor();           // Disable the cursor
void LCD_Cursor();             // Enable the cursor
void LCD_ScrollDisplayLeft();  // Scroll the display left
void LCD_ScrollDisplayRight(); // Scroll the display right
void LCD_PrintLeft();          // Print text left-aligned
void LCD_PrintRight();         // Print text right-aligned
void LCD_LeftToRight();        // Set text direction from left to right
void LCD_RightToLeft();        // Set text direction from right to left
void LCD_ShiftIncrement();     // Increment text shift direction
void LCD_ShiftDecrement();     // Decrement text shift direction
void LCD_NoBacklight();        // Turn off the backlight
void LCD_Backlight();          // Turn on the backlight
void LCD_AutoScroll();         // Enable automatic scrolling
void LCD_NoAutoScroll();       // Disable automatic scrolling
void LCD_CreateSpecialChar(uint8_t, uint8_t[]); // Create a custom character for the display
void LCD_PrintSpecialChar(uint8_t); // Print a custom character on the display
void LCD_SetCursor(uint8_t, uint8_t); // Set the cursor position (row, column)
void LCD_SetBacklight(uint8_t new_val); // Set the backlight (on/off)
void LCD_LoadCustomCharacter(uint8_t char_num, uint8_t *rows); // Load custom character data into memory
void LCD_PrintStr(const char[]); // Print a string on the LCD

#endif /* LIQUIDCRYSTAL_I2C_H_ */  // End of header guard
