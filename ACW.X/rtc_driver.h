#ifndef XC_RTC_H
#define	XC_RTC_H

// DRIVER DESCRIPTION:

// INCLUDES.
#include <xc.h> // include processor files - each processor file is guarded.  
#include "utils.h"

// PRIVATE.
// #define i_o   RB4 // io           
// #define sclk  RB0 // input sync.
// #define rst   RB5 // enable
//
// uch time_rx = 0x30; // Receive reg.
// char rtc_Vals[0x08]; // Datetime table.
// char rtc_StrVals[0x0D]; // Datetime as STRING table.
// char days[] = {"MonTueWedThuFriSatSun"}; // Weekdays array.
//
// char rtc_ReadByte(void); // Read byte from active register (e.g. date, year etc.).
// void rtc_WriteByte(char time_tx); // Write byte to active register (e.g. date, year etc.).
// void rtc_SetDay(void); // Set the weekday (0-6) on the clock.


// PUBLIC.
void rtc_Init(void); // Initialise ports and pins.
void rtc_SetTimeComponent(char addr, char val); // Set individual time component at addr with value, val.
char *rtc_GetString(char isDate);  // Get the date or time in string format. If isDate == 1, return date.
void rtc_Update(void); // Read the clock with burst mode and update value tables (rtc_Vals).

#endif