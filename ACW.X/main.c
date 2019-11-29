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
#include "IO_driver.h"
#include "ui.h"


const int DAYTIME[2] = {6, 30}; // 6:30am
const int NIGHTTIME[2] = {19, 30}; // 7:30pm
//
float lowerThreshold = 0.0; // Temperature heating
float upperThreshold = 60.0; // Temperature cooling
float lastTemp = 0.0;
char IsTooHot = 0;
int alarmChecks = 0;

const int alarmValue = 25; //5 per second
// Check temperature thresholds and sound alarm or turn heating/cooling on if appropriate.
//

void CheckTemperature() {
    float temperature = strFloat(calculate_temp(get_temp()), 4);
    
    if (alarmChecks == 0 && IsTooHot != 0) {
        while (matrix_Scan() == '_') {
            buzzer_sound(5000,10000, 1);
        }
        Delay(5000);
        IsTooHot = 0;
    }
    
    if ((IsTooHot == 'Y' && temperature >= lastTemp) || (IsTooHot == 'N' && temperature <= lastTemp)) {
        alarmChecks--;
    }
    
    lastTemp = temperature;

    if (IsTooHot == 0) {
        if (temperature <= lowerThreshold) {
             IsTooHot = 'N';
        } else if (temperature >= upperThreshold) {
             IsTooHot = 'Y';
        } else {
            return;
        }
        buzzer_sound(1000, 1, 1);
        alarmChecks = alarmValue;
    }  
}

// Check/set nighttime (0) or daytime (1) mode
//
//
//void CheckTime() {
//    int hours = (((int) rtc_GetTimeString()[0] * 10) + (int) rtc_GetTimeString()[0]);
//    int minutes = (((int) rtc_GetTimeString()[3] * 10) + (int) rtc_GetTimeString()[4]);
//
//    
//    char* Eval = EEP_Read_String(NIGHT_LOWER_THRESH_TEMP);
//    char EMPTYMEM[2] = {0xFF, 0xFF};
//    if (Eval[0] == EMPTYMEM[0] && Eval[1] == EMPTYMEM[1]) {
//        mode = 13;
//        return;
//    }
//    
//    if (hours > DAYTIME[0] && minutes > DAYTIME[1]) {
//        //NIGHT
//        lowerThreshold = strFloat(EEP_Read_String(NIGHT_LOWER_THRESH_TEMP), 4);
//        upperThreshold = strFloat(EEP_Read_String(NIGHT_UPPER_THRESH_TEMP), 4);
//    } else {
//        //DAY
//        lowerThreshold = strFloat(EEP_Read_String(DAY_LOWER_THRESH_TEMP), 4);
//        upperThreshold = strFloat(EEP_Read_String(DAY_UPPER_THRESH_TEMP), 4);
//    }
//}


void main(void) {
    // Ready the application.
    rtc_Init();
    matrix_Init();
    buzzer_init();
    //    rtc_SetTime(); // Remove after inital config.
    lcd_Init();
    //systemsOff();
      
//    CheckTime(); // Check daytime/nighttime mode.
//    CheckTemperature(); // Check alarms   
//    
    //Loop
    for (;;) {
//        if (mode == 0) {
//            CheckTime(); // Check daytime/nighttime mode.
//            CheckTemperature(); // Check alarms   
//        }
        rtc_Update();
        ui_Render(); // Render LCD according to current UI state.
        ui_Navigate(); // Check for user input to change UI state.
    }
}
