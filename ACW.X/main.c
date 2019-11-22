#include <xc.h>

// Config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
// ---

// Drivers
#include "Utils.h"
#include "buzzer_driver.h"
#include "lcd_driver.h"
#include "rtc_driver.h"
#include "thermometer_driver.h"
#include "matrix.h"


const int DAYTIME[2] = {6, 30}; // 6:30am
const int NIGHTTIME[2] = {19, 30}; // 7:30pm

int mode = 0; // Store UI state
float lowerThreshold[2] = {5.0, 2.5}; // Temperature heating[0] and alarm[1] thresholds.
float upperThreshold[2] = {25.0, 27.5}; // Temperature cooling[0] and alarm[1] thresholds.

// Initialise default ports.

void InitPorts() {
    //    lcd_Init();
    //    ADCON1 = 0X07; //a port as ordinary i/o.
}

// Initialise each component and set the rtc time.

void InitComponents() {
    rtc_Init();
    matrix_Init();
    //    rtc_SetTime();
    //    rtc_SetTimeComponent(DATE, 0x22); 
    lcd_Init();

}

// Ready the application.

void Init() {
    InitPorts();
    InitComponents();
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

// Display the time on the second row of the LCD.

void DisplayDateAndTime() {
    lcd_PrintString("Date:", 0, 0);
    lcd_PrintString(rtc_GetDateString(), 0, 3);
    lcd_PrintString("Time:", 1, 0);
    lcd_PrintString(rtc_GetTimeString(), 1, 3);
}

void DisplaySettingsScreen() {
    lcd_PrintString("Settings:", 0, 0);
    lcd_PrintString("1:Set date/time", 1, 0);
    lcd_PrintString("2:Set thresholds", 2, 0);
}

void DisplaySetTimeScreen() {
    lcd_Clear();
    while (matrix_Scan() != 'F') {
        lcd_PrintString("Set date/time:", 0, 0);
        lcd_PrintString("Date:", 1, 0);
        lcd_PrintString(rtc_GetDateString(), 1, 3);
        lcd_PrintString("Time:", 2, 0);
        lcd_PrintString(rtc_GetTimeString(), 2, 3);
        lcd_SetCursorPos(1, 0);
    }
}

void DisplaySetThresholdsScreen() {
    lcd_Clear();
    while (matrix_Scan() != 'F') {
        lcd_PrintString("Set thresholds:", 0, 0);
        lcd_PrintString("1:Cooling", 1, 0);
        lcd_PrintString("2:Heating", 2, 0);
        lcd_SetCursorPos(1, 0);
    }
}

// Check/set nighttime (0) or daytime (1) mode

void CheckTime(int i) {
    //int time[4] = rtc_GetTime();

}

// Main operation loop.

void Loop() {
    char input;

    for (;;) {
        //        CheckTemperature(); // Check alarms
        //CheckTime(); // Check daytime/nighttime mode.

        // Render LCD according to current UI state.
        switch (mode) {
            case 0: DisplayDateAndTime();
                break;
            case 1: DisplaySettingsScreen();
                break;
        }

        //        // Check for user input.
        input = matrix_Scan();
        switch (input) {
            case 'F':
                if (mode != 0) lcd_Clear(); // If mode has changed, clear lcd.
                mode = 0;
                break;
            case 'E':
                if (mode != 1) lcd_Clear(); // If mode has changed, clear lcd.
                mode = 1;
                break;

            case '3':
                if (mode == 1) lcd_Clear();
                DisplaySetTimeScreen();
                break;
            case '7':
                DisplaySetThresholdsScreen();
                break;
                //            case '11':
                //                if (mode == 1) lcd_Clear();
                //                lcd_PrintString("Other Screen", 0, 0);
                //                break;
        }
    }
}

void main(void) {
    Init(); // Initialise ports and components.
    Loop();
}
