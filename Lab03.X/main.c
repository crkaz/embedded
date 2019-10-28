/*
 * File:   main.c
 * Author: CSuser
 *
 * Created on 11 October 2019, 15:17
 */


#include <xc.h>

//begin config

#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit --
// SerialProgramming Enable bit (RB3 is digital I/O, HV on MCLR must --
//be used for programming)

//end config

void main(void) {
    TRISD = 0x00;
    PORTD = 0x55;
    for (;;);
}
