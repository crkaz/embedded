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
    lcd_Init();
    //    rtc_PortInit();
    rtc_Init();
    //    rtc_SetTime();
}

// Ready the application.

void Init() {
    InitPorts();
    InitComponents();
}

void init() {
    ADCON1 = 0X07; //a port as ordinary i/o.
    TRISA = 0X00; //a port as output.
    TRISD = 0X00; //d port as output.
    TRISC = 0x00;
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
    lcd_Clear();
    lcd_PrintString("Date:", 0, 0);
    lcd_PrintString(rtc_GetDateString(), 0, 3);
    lcd_PrintString("Time:", 1, 0);
    lcd_PrintString(rtc_GetTimeString(), 1, 3);
    Delay(100); // Stop flicker.
}

void DisplaySettingsScreen() {
    lcd_Clear();
    lcd_PrintString("Settings:", 0, 0);
    lcd_PrintString("1: Set date/time", 1, 0);
    lcd_PrintString("2: Set thresholds", 2, 0);
    lcd_PrintString("3: Set date and time", 3, 0);
    Delay(100); // Stop flicker.
}

void DisplaySetTimeScreen(){
    lcd_Clear();
    lcd_PrintString("Set date and time:", 1, 0);
    lcd_PrintString("Date:", 1, 0);
    lcd_PrintString(rtc_GetDateString(), 0, 3);
    lcd_PrintString("Time:", 2, 0);
    lcd_PrintString(rtc_GetTimeString(), 1, 3);
    Delay(100); // Stop flicker.
}

// Check/set nighttime (0) or daytime (1) mode

void CheckTime(int i) {
    //int time[4] = rtc_GetTime();

}

// Main operation loop.

void Loop() {
    int input;

    for (;;) {
//        CheckTemperature(); // Check alarms
        //CheckTime(); // Check daytime/nighttime mode.

        // Render LCD according to current UI state.
        switch (mode) {
            case 'F': DisplayDateAndTime();
                break;
            case 'E': DisplaySettingsScreen();
            
//            case 1: SetTime(); break;
//            case 2: SetThresholds(); break;
//            case 3: Test(); break;
        }

        // Check for user input.
        input = matrix_Scan();
        switch (input) {
            case 'F': mode = 0; break;
            case 'E': mode = 1; break;
            
            case '3': 
                if (mode == 1){
                    DisplaySetTimeScreen();
                }
                break;
            case '7': mode = 3; break;
            case '11': mode = 4; break;
        }
    }
}

void main(void) {
    Init(); // Initialise ports and components.
    rtc_SetTime(); // Set time to default values.
    Loop();
}
