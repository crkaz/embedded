#include <xc.h>

//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

//#define rs RA5
//#define rw RA4
//#define e RA3
//#define psb RA2
#define nop() asm("nop")

//delay(2,70); //delay 503us
//delay(2,8); //delay 70us
//delay(2,60); //delay 430us
//delay(2,7); //delay 63us
//NOP(); //delay 1us 
void delay(char x, char y)
{
 char z;
 do{
 z=y;
 do{;}while(--z);
 }while(--x);
}

void main(void) {
    return;
}
