// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_UI_H
#define	XC_UI_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "Utils.h"
#include "thermometer_driver.h"
#include "rtc_driver.h"
#include "buzzer_driver.h"
#include "matrix.h"
#include "lcd_driver.h"
#include "EEP_Driver.h"

char DAY_LOWER_THRESH_TEMP = 0x00;
char DAY_UPPER_THRESH_TEMP = 0x10;
char NIGHT_LOWER_THRESH_TEMP = 0x20;
char NIGHT_UPPER_THRESH_TEMP = 0x30;
char DAY_START_TIME = 0x40;
char DAY_END_TIME = 0x50;

int ui_Mode = 0;
char IsTooHot = 0;
char IsDay = 0x01;
void ui_DisplayStandby(void);
void ui_DisplayMenu(char ln1[], char ln2[], char ln3[], char ln4[]);
void ui_DisplaySetDateTime(char isDate);
void ui_DisplaySetDaytime(void);
void ui_DisplaySetThresholds(char isNight);
void ui_Navigate(void);
void ui_Render(void);
char ui_ValidateInput(char inputs[]);
char ui_GetInput(char seperator, char addr);
#endif	/* XC_UI_H */
