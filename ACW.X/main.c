/*
 * File:   main.c
 * Author: Zak
 *
 * Created on 04 November 2019, 15:32
 */

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
#include "lcd_driver.h"
#include "rtc_driver.h"
#include "thermometer_driver.h"
// ---

void main(void) {
    while (1) {
        // ---
        
        // Code here.
        
        // ---
    }
}
