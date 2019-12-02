// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_UI_H
#define	XC_UI_H

// DRIVER DESCRIPTION:

// INCLUDES.
#include <xc.h>
#include "utils.h"
#include "thermometer_driver.h"
#include "rtc_driver.h"
#include "buzzer_driver.h"
#include "matrix_driver.h"
#include "lcd_driver.h"
#include "eep_driver.h"
#include "io_driver.h"

// DEFINES.

// PUBLIC VARS.
const char DAY_LOWER_THRESH_TEMP = 0x00;
const char DAY_UPPER_THRESH_TEMP = 0x10;
const char NIGHT_LOWER_THRESH_TEMP = 0x20;
const char NIGHT_UPPER_THRESH_TEMP = 0x30;
const char DAY_START_TIME = 0x40;
const char DAY_END_TIME = 0x50;
int ui_Mode = 0;

// PUBLIC METHODS.
void ui_Navigate(void); // Determine UI state.
void ui_Render(void); // Render UI state.

// PRIVATE METHODS.
//void ui_DisplayStandby(void); // Renders the standby screen.
//void ui_DisplayMenu(char ln1[], char ln2[], char ln3[], char ln4[]); // Renders a title and 3 menu options.
//void ui_DisplaySetDateTime(char isDate); // Renders the screen to set time.
//void ui_DisplaySetDaytime(void); // Renders the screen to set the daytime start and finish.
//void ui_DisplaySetThresholds(char isNight); // Renders the screen to set the lower (heating) and upper (cooling) temperature thresholds.
//uch ui_ValidateInput(char inputs[]); // Validates a user input based on the current UI state.
//char ui_GetInput(char seperator, char addr); // Gets and formats input from the user, and sets the use the values to update the system.

#endif	/* XC_UI_H */
