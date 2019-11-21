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
//#include "button_driver.h"
#include "buzzer_driver.h"
#include "lcd_driver.h"
#include "rtc_driver.h"
#include "thermometer_driver.h"
// ---

const int DAYTIME[2] = {6, 30}; // 6:30am
const int NIGHTTIME[2] = {19, 30}; // 7:30pm

int mode[2]; // Store UI state e.g. mode[1,0] == settings menu. mode[0,1] == settings>set time.
float lowerThreshold[2] = {5.0, 2.5}; // Temperature heating[0] and alarm[1] thresholds.
float upperThreshold[2] = {25.0, 27.5}; // Temperature cooling[0] and alarm[1] thresholds.

// Initialise default ports.
void InitPorts() {
    //    ADCON1 = 0X07; //a port as ordinary i/o.
}

// Initialise each component and set the rtc time.
void InitComponents() {
    lcd_Init();
    rtc_PortInit();
    rtc_Init();
    rtc_SetTime();
}

// Ready the application.
void Init() {
    InitPorts();
    InitComponents();
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
    lcd_SetCursorPos(1, 1);
    lcd_PrintString(rtc_GetDateString());
    lcd_SetCursorPos(2, 1);
    lcd_PrintString(rtc_GetTimeString());
}

// Check/set nighttime (0) or daytime (1) mode
void CheckTime(int i) {
    int time[4] = rtc_GetTime();
    
}

// Main operation loop.
void Loop() {
    int input;

    for (;;) {
        CheckTemperature(); // Check alarms
        CheckTime(); // Check daytime/nighttime mode.

        switch(mode){
            case 0: DisplayDateAndTime(); break;
            case 1: SetTime(); break;
            case 2: SetThresholds(); break;
            case 3: Test(); break;
        }

        //         input = BTN_GetInput();
        switch (input) {
            case 0: break;
                mode = 0;
            case 1: break;
                mode = 1;
            case 2: break;
                mode = 2;
            case 3: break;
                mode = 3;
            case 4: break;
            case 5: break;
            case 6: break;
            case 7: break;
            case 8: break;
            case 9: break;
            case 10: break;
            case 11: break;
            case 12: break;
            case 13: break;
            case 14: break;
            case 15: break;

        }
    }
}

void main(void) {
    Init(); // Initialise ports and components.
    Loop(); // Main operations.
}
