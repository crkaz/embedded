#include <xc.h>

// Config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)

// Drivers
#include "Utils.h"
#include "buzzer_driver.h"
#include "lcd_driver.h"
#include "rtc_driver.h"
#include "thermometer_driver.h"
#include "matrix.h"
#include "EEP_Driver.h"

const int MAX_SCREEN_INDEX = 2; // 0 == default, 1 == settings, 2 == test.
int mode = 0; // Store UI state.  // 0 == default, 1 == settings, 2 == test.
int changedMode = 0; // For clearing screen on initial redraw.
//
const int DAYTIME[2] = {6, 30}; // 6:30am
const int NIGHTTIME[2] = {19, 30}; // 7:30pm
//
const char DAY_UPPER_THRESH_TEMP = 0x30;
const char DAY_LOWER_THRESH_TEMP = 0x35;
const char DAY_THRESH_ALARM = 0x40;
const char NIGHT_UPPER_THRESH_TEMP = 0x45;
const char NIGHT_LOWER_THRESH_TEMP = 0x50;
const char NIGHT_THRESH_ALARM = 0x55;
const char NO_EEP = 0x00;

float lowerThreshold[2] = {5.0, 2.5}; // Temperature heating[0] and alarm[1] thresholds.
float upperThreshold[2] = {25.0, 27.5}; // Temperature cooling[0] and alarm[1] thresholds.

// Check temperature thresholds and sound alarm or turn heating/cooling on if appropriate.
//
void CheckTemperature() {
        float temp = get_temp();
        if (temp > upperThreshold[1] || temp < lowerThreshold[1]) {
            buzzer_sound(500, 500, 500);
        }
        if (temp > upperThreshold[0]) {
            // Switch cooling on.
        }
        else if (temp < lowerThreshold[0]){
            // Switch heating output on.
        }
}

// Check/set nighttime (0) or daytime (1) mode
//
void CheckTime() {
    int hours = (((int)rtc_GetTimeString()[0] * 10) + (int)rtc_GetTimeString()[0]);
    int minutes = (((int)rtc_GetTimeString()[3] * 10) + (int)rtc_GetTimeString()[4]);
    
    if (hours > DAYTIME[0] && minutes > DAYTIME[1]) {
        
        //NIGHT
        lowerThreshold[0] = strFloat(EEP_Read_String(NIGHT_LOWER_THRESH_TEMP),3 );
        upperThreshold[0] = strFloat(EEP_Read_String(NIGHT_UPPER_THRESH_TEMP),3);
    } else {
        //DAY
        lowerThreshold[0] = strFloat(EEP_Read_String(DAY_LOWER_THRESH_TEMP), 3);
        upperThreshold[0] = strFloat(EEP_Read_String(DAY_UPPER_THRESH_TEMP), 3);
    }
}

void DisplayMainScreen() {
    lcd_PrintString("Date:", 0, 0);
    lcd_PrintString(rtc_GetDateString(), 0, 3);
    lcd_PrintString("Time:", 1, 0);
    lcd_PrintString(rtc_GetTimeString(), 1, 3);
    lcd_PrintString("Temp:", 2, 0);
    lcd_PrintString(calculate_temp(get_temp()), 2, 3);
    lcd_PrintString("Status:", 3, 0);
    lcd_PrintString("OK", 3, 4);
}

void DisplayMenuScreen(char ln1[], char ln2[], char ln3[], char ln4[]) {
    lcd_PrintString(ln1, 0, 0);
    lcd_PrintString(ln2, 1, 0);
    lcd_PrintString(ln3, 2, 0);
    lcd_PrintString(ln4, 3, 0);
}

int ValidateUserInput(int nInputs, char inputs[], float min, float max) {
    const float sigs[] = {10.0, 1.0, 0.0, 0.1}; // 10s 1s . 0.1s e.g. 00.0
    float sum = 0.0;

    for (int i = 0; i < nInputs; ++i) {
        int val = (int)inputs[i];
        sum += (val * sigs[i]);
    }

    if (mode == 113) { // Variable max input if setting day.
        int month = BcdToDec(rtc_GetTimeComponent(MONTH)); // Float to int.
        if (month == 2) { // Handle feb and leap years.
            int year = BcdToDec(rtc_GetTimeComponent(YEAR)); // Float to int.
            if (IsLeapYear(year))
                max = 29.0;
            else
                max = 28.0;
        } else {
            if (month > 6) month += 1; // From Aug, pattern changes.
            max = 30.0 + (month % 2); // Calc max number of days for month. 
        }
    }

    int returnVal = 1;
    lcd_Clear();
    if (sum < min || sum > max) {

        lcd_PrintString("Error: invalid", 0, 0);
        lcd_PrintString("input.", 1, 0);
        lcd_PrintString("Try again...", 3, 0);

        returnVal = 0; // Failed. Try again (redraw this setscreen).
    } else
        lcd_PrintString("Success!", 0, 0);

    // Show validation message for a while.
    Delay(27000);
    Delay(27000);

    lcd_CursorStatus(0);
    lcd_Clear();

    return returnVal; // Success. Return to previous menu.
}

int CheckUserInput(float min, float max, int inpLimit, char addr) {
    int busy = 1;
    char input;
    int nInputs = 0;
    char inputs[4]; // Has to be compile-time constant; don't always need 4.
    int inputsChanged = 0;

    Delay(7000); // Delay initial key press so it isn't carried from menu selection.
    while (busy) { // Wait for user input.
        input = '_';
        while (input == '_') // Make responsive to user input.
            input = matrix_Scan();
        Delay(4500); // Delay key presses.

        switch (input) {
            case 's': // Enter/select.
                // Validate input.
                if (ValidateUserInput(nInputs, inputs, min, max))
                    input = 'x'; // Set input to 'x' to return to menu.
                else
                    return 1; // Try again.
                break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            case '.':
            {
                // Record user input.
                if (nInputs < inpLimit) {
                    inputs[nInputs] = input; // Store for validation.
                    nInputs += 1;
                    inputsChanged = 1;

                    // Optional UX.
                    if (nInputs == inpLimit)
                        lcd_PrintString("Press enter...", 3, 0);
                    else
                        lcd_PrintString(lcd_EMPTY_STRING, 3, 0); // Clear line 2.
                }
                break;
            }
            case 'b': // Backspace.
                if (nInputs > 0) {
                    inputs[nInputs] = ' '; // Clear last input.
                    nInputs--;
                    inputsChanged = 1;

                    // Optional UX.
                    if (nInputs == inpLimit)
                        lcd_PrintString("Press enter...", 3, 0);
                    else
                        lcd_PrintString(lcd_EMPTY_STRING, 3, 0); // Clear line 2.
                }
                break;
        }

        if (input == 'x') { // Cancel/back.
            busy = 0; // Break input loop.
            mode /= 10; // Return to previous screen.
        }

        //        if (input == 's') { // Enter/submit.
        //            // Validate input.
        //            if (ValidateUserInput(nInputs, inputs, min, max))
        //                input = 'x'; // Set input to 'x' to return to menu.
        //            else
        //                return 1; // Try again.
        //        } else if ((input != '_' && input != 'x' && input != '<' && input != '>' && input != '.' && input != 'b') && nInputs < inpLimit) {
        //            // Record user input.
        //            inputs[nInputs] = input; // Store for validation.
        //            nInputs += 1;
        //            inputsChanged = 1;
        //
        //            if (nInputs == inpLimit)
        //                lcd_PrintString("Press enter...", 3, 0);
        //            else
        //                lcd_PrintString(lcd_EMPTY_STRING, 3, 0); // Clear line 2.
        //
        //        } else if (input == 'b' && nInputs > 0) { // Backspace.
        //            inputs[nInputs] = ' '; // Clear last input.
        //            nInputs -= 1;
        //            inputsChanged = 1;
        //
        //            if (nInputs == inpLimit)
        //                lcd_PrintString("Press enter...", 3, 0);
        //            else
        //                lcd_PrintString(lcd_EMPTY_STRING, 3, 0); // Clear line 2.
        //        }

        //        if (input == 'x') { // Cancel/back/success.
        //            busy = 0; // Break input loop.
        //            mode /= 10; // Return to previous screen.
        //        }

        if (inputsChanged) {
            lcd_PrintString(lcd_EMPTY_STRING, 2, 0); // Clear line 2.
            lcd_PrintString("New:", 2, 0);
            lcd_SetCursorPos(2, 2);
            
            for (unsigned char i = 0; i < nInputs; ++i)
                lcd_PrintChar(inputs[i]);
            if (addr != 0x00) {
                EEP_Write_String(addr, inputs);
            }
                
            inputsChanged = 0;
        }
    }
    return 0; // Finish.
}

int DisplaySetScreen(char title[], char *currVal, float min, float max, int inpLimit, char addr) {
    lcd_CursorStatus(1); // Switch cursor on.
    lcd_PrintString(title, 0, 0);
    lcd_PrintString("Current:", 1, 0);
    lcd_PrintString(currVal, 1, 4);
    lcd_PrintString("New:", 2, 0);

    // Wait for user to give valid value or cancel.
    return CheckUserInput(min, max, inpLimit, addr);
}

void Render() {
    switch (mode) {
            // Standby screen.
        case 0: DisplayMainScreen();
            break;

            // Settings screen.
        case 1: DisplayMenuScreen("Settings", "1.Date", "2.Time", "3.Thresholds");
            break;
            // Set DATE screens.
        case 11: DisplayMenuScreen("#Date:", "1.Year", "2.Month", "3.Day");
            break;
        case 111: while (DisplaySetScreen("##Year", BcdToStr(rtc_GetTimeComponent(YEAR)), 0.0, 99.0, 2, NO_EEP));
            break;
        case 112: while (DisplaySetScreen("##Month", BcdToStr(rtc_GetTimeComponent(MONTH)), 1.0, 12.0, 2, NO_EEP));
            break;
        case 113: while (DisplaySetScreen("##Day", BcdToStr(rtc_GetTimeComponent(DATE)), 1.0, 31.0, 2, NO_EEP));
            break;
            // Set TIME screens.
        case 12: DisplayMenuScreen("#Time:", "1.Hour", "2.Min", "3.Sec");
            break;
        case 121: while (DisplaySetScreen("##Hour", BcdToStr(rtc_GetTimeComponent(HOUR)), 0.0, 24.0, 2, NO_EEP));
            break;
        case 122: while (DisplaySetScreen("##Min", BcdToStr(rtc_GetTimeComponent(MIN)), 0.0, 60.0, 2, NO_EEP));
            break;
        case 123: while (DisplaySetScreen("##Sec", BcdToStr(rtc_GetTimeComponent(SEC)), 0.0, 60.0, 2, NO_EEP));
            break;
            // Set THRESHOLDS screens.
        case 13: DisplayMenuScreen("#Thresholds:", "1.Cooling", "2.Heating", "3.Alarm");
            break;
        case 131:
            while (DisplaySetScreen("##Cool. (DAY)", "NI", 0.0, 24.0, 2, DAY_LOWER_THRESH_TEMP)); // Set day thresholds.
            while (DisplaySetScreen("##Cool. (NIGHT)", "NI", 0.0, 24.0, 2, NIGHT_LOWER_THRESH_TEMP)); // Then set night thresholds.
            break;
        case 132:
            while (DisplaySetScreen("##Heat. (DAY)", "NI", 0.0, 60.0, 2, DAY_UPPER_THRESH_TEMP));
            while (DisplaySetScreen("##Heat.  (NIGHT)", "NI", 0.0, 60.0, 2, NIGHT_UPPER_THRESH_TEMP));
            break;
        case 133:
            while (DisplaySetScreen("##Alarm% (DAY)", "NI", 0.0, 60.0, 2, DAY_THRESH_ALARM));
            while (DisplaySetScreen("##Alarm% (NIGHT)", "NI", 0.0, 60.0, 2, NIGHT_THRESH_ALARM));
            break;
    }
}

void Navigate() {
    if (changedMode) {
        Delay(8000); // Don't change screens too fast.
        changedMode = 0;
    }
    char currMode = mode;
    char input = matrix_Scan();
    switch (input) {
        case 'x':
            if (mode > MAX_SCREEN_INDEX) mode /= 10; // Escape setting screen.
            else mode = 0; // Else return home.
            break;
        case '<':
            if (mode < MAX_SCREEN_INDEX + 1) { // Check not in a sub menu.
                mode -= 1; // Go back a screen.
                if (mode == -1) mode = MAX_SCREEN_INDEX; // Cycle to last menu item.
            }
            break;
        case '>':
            if (mode < MAX_SCREEN_INDEX + 1) { // Check not in a sub menu.
                mode += 1; // Go forward a screen.
                if (mode > MAX_SCREEN_INDEX) mode = 0; // Cycle to first menu item.
            }
            break;
        case '1':
        case '2':
        case '3':
        {
            int i = input - 48;
            if (mode <= MAX_SCREEN_INDEX || mode == 11 || mode == 12) mode = mode * 10 + i; // Step into sub menu.
            break;
        }
    }
    if (currMode != mode) {
        lcd_Clear(); // If loading a new screen, clear.
        changedMode = 1;
    }
}

void main(void) {
    // Ready the application.
    rtc_Init();
    matrix_Init();
    buzzer_init();
    //    rtc_SetTime(); // Remove after inital config.
    lcd_Init();

    //Loop
    for (;;) {
        CheckTime(); // Check daytime/nighttime mode.
        CheckTemperature(); // Check alarms
        Render(); // Render LCD according to current UI state.
        Navigate(); // Check for user input to change UI state.
    }
}
