#include <xc.h>
//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial
//Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for
//programming)
//end config
const int NUMS[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x83, 0xF8, 0x80, 0x98};
const int LEDS[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF};

void delay(int t) {
    for (unsigned i = 0; i < t; ++i);
}

void showDigit_7Seg(int digit, int pos) {
    TRISA = TRISD = 0x00; // Set to output.
    PORTA = 0xFF; // Switch all off.
    PORTD = 0X00; // Switch all off.
    PORTD = NUMS[digit];
    PORTA = LEDS[pos];
}

int handleButtonPress() {
    TRISB = 0xFF; // Set to input.
    ADCON1 = 0x07; // Set all pins to digital.
    int x = 0;
    switch (PORTB) {
        case 0x01: x = 1;
            break;
        case 0x02: x = 2;
            break;
        case 0x04: x = 3;
            break;
        case 0x08: x = 4;
            break;
    }

//    switch (PORTA) {
//        case 0x01: x = 5;
//            break;
//        case 0x02: x = 6;
//            break;
//        case 0x04: x = 7;
//            break;
//        case 0x08: x = 8;
//            break;
//    }
    
    return x;
}

void main() {
    int digit = 0;

    for (;;) {
        digit = handleButtonPress();
        delay(200);
        if (digit > 0) {
            showDigit_7Seg(digit, 1); // Show number on nth+1 LED.
        }
    }
}