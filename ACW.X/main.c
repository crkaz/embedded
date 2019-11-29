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


int DAYTIME[2] = {6, 30}; // 6:30am
int NIGHTTIME[2] = {19, 30}; // 7:30pm
//
float lowerThreshold = 0.0; // Temperature heating
float upperThreshold = 60.0; // Temperature cooling
float lastTemp = 0.0;

int alarmChecks = 0;

const int alarmValue = 25; //5 per second
// Check temperature thresholds and sound alarm or turn heating/cooling on if appropriate.
//

unsigned char tempCount = 0;
void CheckTemperature() {
    float temperature = strFloat(calculate_temp(get_temp()));
    
    
    if (alarmChecks == 0 && IsTooHot != 0) {
        while (matrix_Scan() == '_') {
            buzzer_sound(5000, 10000, 1);
        }
        IsTooHot = 0;
        systemsOff();
    }

    if ((IsTooHot == 'Y' && temperature >= lastTemp) || (IsTooHot == 'N' && temperature <= lastTemp)) {
        alarmChecks--;
    }
    
    if (tempCount++ % 5)
        lastTemp = temperature;

    if (IsTooHot == 0) {
        if (temperature < lowerThreshold) {
            heaterOn();
            IsTooHot = 'N';
        } else if (temperature > upperThreshold) {
            coolerOn();
            IsTooHot = 'Y';
        } else {
            return;
        }
        buzzer_sound(1000, 1, 1);
        alarmChecks = alarmValue;
    } else if (temperature < upperThreshold && temperature > lowerThreshold) {
        IsTooHot = 0;
        systemsOff();  
    }
}

// Check/set nighttime (0) or daytime (1) mode
//

void CheckTime() {    
    char EMPTYMEM = 0xFF;
    
    char* eval = EEP_Read_String(NIGHT_LOWER_THRESH_TEMP, 0x00);
    if (eval[0] == EMPTYMEM && eval[1] == EMPTYMEM) {
        ui_Mode = 1;
        return; // Force user into settings screen on first boot.
    }
    
    eval = EEP_Read_String(DAY_LOWER_THRESH_TEMP, 0x00);
    if (eval[0] == EMPTYMEM && eval[1] == EMPTYMEM) {
        ui_Mode = 1;
        return; // Force user into settings screen on first boot.
    }
    
    eval = EEP_Read_String(DAY_END_TIME, 0x00);
    if (eval[0] == EMPTYMEM && eval[1] == EMPTYMEM) {
        ui_Mode = 1;
        return; // Force user into settings screen on first boot.
    }

    int hours = (((int) rtc_GetString(0x00)[0] * 10) + (int) rtc_GetString(0x00)[0]);
    int minutes = (((int) rtc_GetString(0x00)[3] * 10) + (int) rtc_GetString(0x00)[4]);

    DAYTIME[0] = (EEP_Read_String(DAY_START_TIME, 0x00)[0] - '0' * 10) + EEP_Read_String(DAY_START_TIME, 0x00)[1] - '0';
    DAYTIME[1] = (EEP_Read_String(DAY_START_TIME, 0x00)[3] - '0' * 10) + EEP_Read_String(DAY_START_TIME, 0x00)[4] - '0';

    NIGHTTIME[0] = (EEP_Read_String(DAY_END_TIME, 0x01)[0] - '0' * 10) + EEP_Read_String(DAY_END_TIME, 0x01)[1] - '0';
    NIGHTTIME[1] = (EEP_Read_String(DAY_END_TIME, 0x01)[3] - '0' * 10) + EEP_Read_String(DAY_END_TIME, 0x01)[4] - '0';

    
    
    if ((hours >= DAYTIME[0]) && (hours <= NIGHTTIME[0])) { 
        if ((hours == DAYTIME[0] && minutes > DAYTIME[1]) || (hours == NIGHTTIME[0] && minutes < NIGHTTIME[1])) {
            lowerThreshold = strFloat(EEP_Read_String(DAY_LOWER_THRESH_TEMP, 0x00));
            upperThreshold = strFloat(EEP_Read_String(DAY_UPPER_THRESH_TEMP, 0x01));    
            return;
        }
    }
        
    //Night
    lowerThreshold = strFloat(EEP_Read_String(NIGHT_LOWER_THRESH_TEMP, 0x00));
    upperThreshold = strFloat(EEP_Read_String(NIGHT_UPPER_THRESH_TEMP, 0x01));

}

void main(void) {
    // Ready the application.
    rtc_Init();
    matrix_Init();
    buzzer_init();
    //    rtc_SetTime(); // Remove after inital config.
    lcd_Init();

    Delay(500);
    systemsOff();
    
    
    // Perform initial check.
    CheckTime(); // Check daytime/nighttime mode.
        
    //Loop
    for (;;) {
        if (ui_Mode == 0) { // Only perform checks when not setting.
            CheckTime(); // Check daytime/nighttime mode.
            CheckTemperature(); // Check alarms   
        }
        rtc_Update();
        ui_Render(); // Render LCD according to current UI state.
        ui_Navigate(); // Check for user input to change UI state.
    }
}
