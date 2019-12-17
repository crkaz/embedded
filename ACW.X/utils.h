// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_Utils_H
#define	XC_Utils_H

// DRIVER DESCRIPTION:

// INCLUDES.
#include <xc.h>

// DEFINES.
#define uch unsigned char // Shorthand for unsigned char.
#define true 0x01 // Makeshift boolean.
#define false 0x00 // Makeshift boolean.
#define toInt -0x30 // Add (+) to chars to convert to ints.
#define eol '\0' // String terminate char.

// PUBLIC VARS.
//- Precise timings for DelayT.
uch t503us[0x02] = {0x02, 0x46};
uch t430us[0x02] = {0x02, 0x3C};
uch t70us[0x02] = {0x02, 0x08};
uch t63us[0x02] = {0x02, 0x07};

// PUBLIC METHODS.
void Delay(int n); // Delay n cycles.
void DelayT(uch t[]); // Delay a specific time given by a pre-calcuated, char array.
uch StrLen(char*); // Get length of a string.
char* BcdToStr(char); // Convert BCD (binary coded decimal) to string.
char StrToBcd(char str[]); // Convert string to BCD (binary coded decimal).
uch IsLeapYear(int yr); // Return whether n
float StrToFloat(char str[]); // Convert a string with format ##.# to float representation.
uch BcdToDec(char bcd); // Convert a BCD (binary coded decimal) to decimal.

// PRIVATE METHODS.

#endif	/* XC_Utils_H */

