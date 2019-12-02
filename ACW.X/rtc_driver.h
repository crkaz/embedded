// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_RTC_H
#define	XC_RTC_H

// DRIVER DESCRIPTION:

// INCLUDES.
#include <xc.h> // include processor files - each processor file is guarded.  
#include "utils.h"

// DEFINES.

// PUBLIC VARS.

// PUBLIC METHODS.
void rtc_Init(void); // Initialise DS1302.
//void rtc_SetTime(void); // Set all time components with burst mode.
void rtc_SetTimeComponent(char, char); // Set individual time component.
char *rtc_GetString(char isTime);  // Get the date or time in string format.
void rtc_Update(void); // Read the clock with burst mode and update table.

// PRIVATE METHODS.

#endif	/* XC_RTC_H */