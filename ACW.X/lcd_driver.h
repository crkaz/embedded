// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_LCD_H
#define	XC_LCD_H

// DRIVER DESCRIPTION:

// INCLUDES.
#include <xc.h> // include processor files - each processor file is guarded.  
#include "utils.h"

// DEFINES.

// PUBLIC VARS.
char lcd_EMPTY_STRING[] = "               \0"; // Empty string for clearing a line.

// PUBLIC METHODS.
//void lcd_CursorStatus(char i); // Switch cursor on (1) or off (0);
void lcd_SetCursorPos(uch lineN, uch pos); // Set the position of the cursor.
void lcd_Init(void); // Initialise component.
void lcd_PrintChar(char character); // Print a single char at the current cursor pos.
void lcd_PrintString(char str[], uch lineN, uch pos); // Print a series of chars, starting from the current cursor pos.
void lcd_Clear(void); // Cler the whole lcd.

// PRIVATE METHODS.

#endif	/* XC_HEADER_TEMPLATE_H */

