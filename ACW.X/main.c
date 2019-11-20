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
#include "lcd_driver.h"
#include "thermometer_driver.h"

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

	lcd_init();

	delay(10000);
    while (1) {
		int temp = get_temp();
		char* tempa = calculate_temp(temp);
		writeIntArray(tempa);
		writechar(' ');
		delay(10000);
    }
}