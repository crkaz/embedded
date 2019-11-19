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

int mode = 0; // 0 == , 1 == , 2 == ,

// Initialise default ports.
void InitPorts() {
//    ADCON1 = 0X07; //a port as ordinary i/o.
}

// Initialise each component and set the rtc time.
void InitComponents() {
    lcd_Init();
    rtc_port_init();
    rtc_init();
    set_time();
}

// Ready the application.
void Init() {
    InitPorts();
    InitComponents();
}


// Check temperature thresholds and sound alarm if appropriate.
void CheckTemperature(){
    lcd_SetCursorPos(1, 1);
    lcd_Clear();
    lcd_PrintString(get_time_as_string());
}

// Display the time on the second row of the LCD.
void DisplayTime(){
    lcd_SetCursorPos(2, 1);
    lcd_Clear();
    lcd_PrintString(get_time_as_string());
}
// Set nighttime (0) or daytime (1) mode
void SetMode(int i){
}

// Main operation loop.
void Loop() {
    int input;

    for (;;) {
        // input = BTN_GetInput();
        switch (input) {
            case 0: break;
            case 1: break;
            case 2: break;
            case 3: break;
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
