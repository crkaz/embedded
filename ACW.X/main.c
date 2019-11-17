#include <xc.h>

// Config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
// ---

// Drivers
#include "buzzer_driver.h"
#include "Utils.h"
//#include "lcd_driver.h"

//#include "rtc_driver.h"
//#include "thermometer_driver.h"
// ---
#define psb RA2

void init() {
	ADCON1 = 0X07; //a port as ordinary i/o.
	TRISA = 0X00; //a port as output.
	TRISD = 0X00; //d port as output.
	TRISC = 0x00;
	psb = 1;
}

void main(void) {

	init();
    
    buzzer_init();
    while (1) {
        // ---
        // Code here.
        // Constant alarm.
        buzzer_sound(12500, 5000, 1);
        // ---
    }
}