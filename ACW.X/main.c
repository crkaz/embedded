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

const int MAX_SCREEN_INDEX = 2; // 0 == default, 1 == settings, 2 == test.
int mode = 0; // Store UI state.  // 0 == default, 1 == settings, 2 == test.
int changedMode = 0; // For clearing screen on initial redraw.

const int DAYTIME[2] = {6, 30}; // 6:30am
const int NIGHTTIME[2] = {19, 30}; // 7:30pm

float lowerThreshold[2] = {5.0, 2.5}; // Temperature heating[0] and alarm[1] thresholds.
float upperThreshold[2] = {25.0, 27.5}; // Temperature cooling[0] and alarm[1] thresholds.

// Ready the application.

void Init() {
    rtc_Init();
    matrix_Init();
    buzzer_init();
    //    rtc_SetTime(); // Remove after inital config.
    lcd_Init();
    //    Delay(1000);
    //    lcd_Init(); // Ensure lcd switches on at start.
}

// Check temperature thresholds and sound alarm or turn heating/cooling on if appropriate.

void CheckTemperature() {
    //    float temp = temp_GetTemp();
    //    if (temp > upperThreshold[1] || temp < lowerThreshold[1]) {
    //        buz_Sound(500, 500, 500);
    //    }
    //    if (temp > upperThreshold[0]
    //        // Switch cooling on.
    //    }
    //    else if (temp < lowerThreshold[0]){
    //        // Switch heating output on.
    //    }
}

// Check/set nighttime (0) or daytime (1) mode

void CheckTime(int i) {
    //int time[4] = rtc_GetTime();

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
        int val = inputs[i] - '0';
        sum = sum + (val * sigs[i]);
    }

    if (sum < min || sum > max)
        return 0;

    return 1;
}

void CheckUserInput(float min, float max, int inpLimit) {
    int busy = 1;
    char input;
    int nInputs = 0;
    char inputs[4]; // Has to be compile-time constant; don't always need 4.

    while (busy) { // Wait for user input.
        Delay(6500); // Delay key presses.
        input = matrix_Scan();

        if (input == 's') { // Enter/submit.
            // Validate input.
            if (ValidateUserInput(nInputs, inputs, min, max)) {
                lcd_PrintString(lcd_EMPTY, 3, 0); // Clear line
                lcd_PrintString("Success!", 3, 0);
            } else {
                //            lcd_PrintString(lcd_EMPTY, 3, 0) // Clear line
                lcd_PrintString("Err: invalid inp", 3, 0);
            }
            Delay(25000); // Display message for while.
            Delay(25000); // Display message for while.

            input = 'x'; // Set input to 'x' to return to menu.
        } else if ((input != '_' && input != 'x' && input != '<' && input != '>' && input != '.') && nInputs < inpLimit) {
            lcd_PrintChar(input);
            inputs[nInputs] = input; // Store for validation.
            nInputs += 1;

            if (nInputs == inpLimit)
                lcd_PrintString("Press enter...", 3, 0);
        }
        if (input == 'x') { // Cancel/back.
            busy = 0; // Break input loop.
            mode /= 10; // Return to previous screen.
        }
    }
}

void DisplaySetScreen(char title[], char *currVal, float min, float max, int inpLimit) {
    lcd_CursorStatus(1); // Switch cursor on.
    lcd_PrintString(title, 0, 0);
    lcd_PrintString("Curr:", 1, 0);
    lcd_PrintString(currVal, 1, 3);
    lcd_PrintString("New:", 2, 0);
    lcd_SetCursorPos(2, 2);

    // Wait for user to give valid value or cancel.
    CheckUserInput(min, max, inpLimit);

    lcd_CursorStatus(0);
    lcd_Clear();
    //    WriteCmd(0x38); // 8 bits 2 lines 5*7 mode. / Set function
}

void Render() {
    switch (mode) {
        case 0:
            DisplayMainScreen();
            break;

        case 1:
            DisplayMenuScreen("Settings:", "1.Date", "2.Time", "3.Thresholds");
            break;
        case 11:
            //            DisplaySetScreen("#Date", rtc_GetDateString(), 0.0, 0.0);
            DisplayMenuScreen("#Date:", "1.Year", "2.Month", "3.Day");
            break;
        case 111:
            DisplaySetScreen("##Year", rtc_GetTimeComponentAsString(YEAR), 0.0, 99.0, 2);
            break;
        case 112:
            DisplaySetScreen("##Month", rtc_GetTimeComponentAsString(MONTH), 1.0, 12.0, 2);
            break;
        case 113:
            DisplaySetScreen("##Day", rtc_GetTimeComponentAsString(DATE), 1.0, 31.0, 2);
            break;
        case 12:
            DisplaySetScreen("#Time", rtc_GetTimeString(), 0.0, 0.0, 2);
            break;
        case 13:
            lcd_PrintString("NOT IMPLEMENTED", 0, 0);
            break;

        case 2:
            DisplayMenuScreen("Test:", "1.Cooling", "2.Heating", "3.Alarm");
            break;
        case 21:
            lcd_PrintString("NOT IMPLEMENTED", 0, 0);
            break;
        case 22:
            lcd_PrintString("NOT IMPLEMENTED", 0, 0);
            break;
        case 23:
            lcd_PrintString("NOT IMPLEMENTED", 0, 0);
            break;
    }
}

void Navigate() {
    if (changedMode) {
        Delay(8000); // Don't change screens too fast.
        changedMode = 0;
    }
    char currMode = mode;
    switch (matrix_Scan()) {
        case 'x':
            if (mode > MAX_SCREEN_INDEX) mode /= 10; // Escape setting screen.
            else mode = 0; // Else return home.
            break;
        case '<':
            if (mode < MAX_SCREEN_INDEX + 1) { // Check not in a sub menu.
                mode -= 1; // Go back a screen.
                if (mode == -1) mode = MAX_SCREEN_INDEX;
            }
            break;
        case '>':
            if (mode < MAX_SCREEN_INDEX + 1) { // Check not in a sub menu.
                mode += 1; // Go forward a screen.
                if (mode > MAX_SCREEN_INDEX) mode = 0;
            }
            break;
        case '1':
            if (mode <= MAX_SCREEN_INDEX) mode = mode * 10 + 1; // Step into sub menu.
            else if (mode == 11) mode = mode * 10 + 1;
            break;
        case '2':
            if (mode <= MAX_SCREEN_INDEX) mode = mode * 10 + 2;
            else if (mode == 11) mode = mode * 10 + 2;
            break;
        case '3':
            if (mode <= MAX_SCREEN_INDEX) mode = mode * 10 + 3;
            else if (mode == 11) mode = mode * 10 + 3;
            break;
    }
    if (currMode != mode) {
        lcd_Clear(); // If loading a new screen, clear.
        changedMode = 1;
    }
}

// Main operation loop.

void Loop() {
    for (;;) {
        //        CheckTemperature(); // Check alarms
        //CheckTime(); // Check daytime/nighttime mode.
        Render(); // Render LCD according to current UI state.
        Navigate(); // Check for user input to change UI state.
    }
}

void main(void) {
    Init(); // Initialise ports and components.
    Loop();
}
