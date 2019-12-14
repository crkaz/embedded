#include <xc.h>

// Config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)

// Drivers
#include "utils.h"
#include "buzzer_driver.h"
#include "lcd_driver.h"
#include "rtc_driver.h"
#include "thermometer_driver.h"
#include "matrix_driver.h"
#include "eep_driver.h"
#include "io_driver.h"
#include "ui.h"


uch dayStart[0x02]; // 6:30am
uch dayEnd[0x02]; // 7:30pm
//
float lowerThreshold = 0.0; // Heating turns on.
float upperThreshold = 0.0; // Cooling turns on.
float lastTemp = 0.0; // Temperature in last iteration.
uch alarmChecks = 0x00; // S
const uch alarmValue = 0x2D; // (45) ~8 iterations per second
uch tempCount = 0x00; // Counter until last temp is recorded (so it isn't updated continually).
char isDay = true; // Day/night flag.
char isTooHot = 0x00; // Hot/cold/ok flag.
uch tempCheckInterval = 0x0A; // How often to record the temperature to monitor cooling/heating alarm.

void CheckTemperature() {
    float temperature = StrToFloat(therm_GetTemp()); // Get the current temperature as a float.

    if (alarmChecks == 0x00 && isTooHot != 0x00) { // If heating/cooling not correcting temperature.
        lcd_Clear();
        lcd_PrintString("Hold to disarm!", 0x00, 0x00); // Display warning.

        // Sound alarm until user input.
        while (matrix_GetInput() == '_') {
            buzzer_sound(5000, 7500, 1);
        }
        
        // Reset status.
        isTooHot = 0x00;
        io_SwitchOff();
        lcd_Clear();
    }

    // If temperature is getting worse, decrement alarmcheck (n checks until alarm again).
    if ((isTooHot == 'Y' && temperature >= lastTemp) || (isTooHot == 'N' && temperature <= lastTemp)) {
        alarmChecks--;
    }

    // Update the "lastTemp" every 10 cycles (every cycle is too quick to detect change).
    if (tempCount++ % tempCheckInterval == 0) {
        lastTemp = temperature;
    }

    // Check system status.
    if (isTooHot == 0x00) {
        if (temperature <= lowerThreshold) {
            isTooHot = 'N'; // No, it is too cold.
            io_TogglePin(0, "HEATING   ");
        } else if (temperature >= upperThreshold) {
            isTooHot = 'Y'; // Yes, it is too hot.
            io_TogglePin(1, "COOLING   ");
        } else {
            // Temperature is okay - manually set status string.
            if (isDay)
                io_Status = "OK (day)";
            else
                io_Status = "OK (night)";
            return;
        }
        buzzer_sound(1000, 1, 1); // Indicate heating or cooling was activated.
        alarmChecks = alarmValue; // Monitor temperature changes until OK status.
    } else if (temperature < upperThreshold && temperature > lowerThreshold) {
        isTooHot = 0x00;
        io_SwitchOff();
    }
}

void CheckTime() {
    // Force user into settings screen on first boot....
    char emptyVal = 0xFF; // Identifier for empty eeprom cell.

    // Check if night thresholds have been configured.
    char* eepVal = eep_ReadString(NIGHT_LOWER_THRESH_TEMP, 0x00); // Get value from eeprom.
    if (eepVal[0x00] == emptyVal && eepVal[0x01] == emptyVal) {
        ui_Mode = 2;
        return; // Force user into settings screen on first boot.
    }

    // Check if day thresholds have been configured.
    eepVal = eep_ReadString(DAY_LOWER_THRESH_TEMP, 0x00);
    if (eepVal[0x00] == emptyVal && eepVal[0x01] == emptyVal) {
        ui_Mode = 2;
        return; // Force user into settings screen on first boot.
    }

    // Check if daytime period has been configured.
    eepVal = eep_ReadString(DAY_END_TIME, 0x00);
    if (eepVal[0x00] == emptyVal && eepVal[0x01] == emptyVal) {
        ui_Mode = 2;
        return; // Force user into settings screen on first boot.
    }
    //...

    // Get hours and minutes from rtc.
    uch hours = ((rtc_GetString(0x00)[0x00] + toInt) * 0x0A) + rtc_GetString(0x00)[0x01] + toInt;
    uch minutes = ((rtc_GetString(0x00)[0x03] + toInt) * 0x0A) + rtc_GetString(0x00)[0x04] + toInt;

    // Get daytime start from eeprom.
    dayStart[0] = ((eep_ReadString(DAY_START_TIME, 0x00)[0x00] + toInt) * 0x0A) + (eep_ReadString(DAY_START_TIME, 0x00)[0x01] + toInt); // Hours.
    dayStart[1] = (eep_ReadString(DAY_START_TIME, 0x00)[0x03] + toInt) * 0x0A; // Minutes.

    // Get daytime end from eeprom.
    dayEnd[0] = ((eep_ReadString(DAY_END_TIME, 0x01)[0x00] + toInt) * 0x0A) + (eep_ReadString(DAY_END_TIME, 0x01)[0x01] + toInt); // Hours.
    dayEnd[1] = (eep_ReadString(DAY_END_TIME, 0x01)[0x03] + toInt) * 0x0A; // Minutes.


    // Determine if it is day or night.
    if (hours > dayStart[0x00] && hours < dayEnd[0x00]) {
        isDay = true;
    } else if ((hours == dayStart[0x00] && minutes > dayStart[0x01]) || (hours == dayEnd[0x00] && minutes < dayEnd[0x01])) {
        isDay = true;
    } else {
        isDay = false;
    }

    // Read thresholds from eeprom.
    if (isDay) {
        lowerThreshold = StrToFloat(eep_ReadString(DAY_LOWER_THRESH_TEMP, 0x00));
        upperThreshold = StrToFloat(eep_ReadString(DAY_UPPER_THRESH_TEMP, 0x01));
    } else { // Is night.
        lowerThreshold = StrToFloat(eep_ReadString(NIGHT_LOWER_THRESH_TEMP, 0x00));
        upperThreshold = StrToFloat(eep_ReadString(NIGHT_UPPER_THRESH_TEMP, 0x01));
    }
}

void main(void) {
    // Ready the application.
    rtc_Init();
    matrix_Init();
    buzzer_init();
    lcd_Init();
    io_Init();

    // Perform initial check to check eeprom values and force user to configure if first boot..
    CheckTime();

    //Loop
    for (;;) {
        if (!ui_Mode) { // Only perform checks when on standby screen.
            CheckTime(); // Check daytime/nighttime mode.
            CheckTemperature(); // Check alarms   
        }
        rtc_Update(); // Update the rtc values.
        ui_Render(); // Render LCD according to current UI state.
        ui_Navigate(); // Check for user input to change UI state.
    }
}
