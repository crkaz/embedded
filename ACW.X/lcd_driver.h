#ifndef XC_LCD_H
#define	XC_LCD_H

// DRIVER DESCRIPTION:
// Interface with the 128x64 LCD component.

// INCLUDES.
#include <xc.h> // include processor files - each processor file is guarded.  
#include "utils.h"

// PRIVATE.
//#define rs RA5 // Command or data.
//#define rw RA4 // Read or write.
//#define e RA3 // Enable.
//#define psb RA2 // Parallel or serial.

// void lcd_WriteCmd(char command); // Write a command byte to the component.


// PUBLIC.
char lcd_EMPTY_STRING[] = "               \0"; // Empty string for clearing a line.

void lcd_SetCursorPos(uch lineN, uch pos); // Set the position of the cursor, where lineN == the line number and pos == the cursor pos.
void lcd_Init(void); // Initialise ports and pins.
void lcd_PrintChar(char character); // Print a single char at the current cursor pos.
void lcd_PrintString(char str[], uch lineN, uch pos); // Print a series of chars from the current cursor pos.
void lcd_Clear(void); // Clear the whole lcd. 

#endif
