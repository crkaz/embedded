#include <xc.h>
//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config
#define rs RA1
#define rw RA2
#define e RA3
void init(); //I/O init
void lcd_init(); //LCD init
void writecmd(char x); //display one byte
void writechar(char x); //LCD display setting.
void delay(); //delay
//I/O init

void init() {
    ADCON1 = 0X07; //a port as ordinary i/o.
    TRISA = 0X00; //a port as output.
    TRISD = 0X00; //d port as output.
}
//---------------------------------------
//LCD init

void lcd_init() {
    writecmd(0x0e); //display on,cursor on,blink on.
    writecmd(0x1); //clr screen
    writecmd(0x38); // 8 bits 2 lines 5*7 mode.
}
//--------------------------------------
//write a byte to lcd.

void writechar(char x) {

    rs = 1; //is data not command.
    rw = 0; //is write not read.
    PORTD = x; //data send to PORTD
    e = 0; //pull low enable signal.
    delay(6000); //for a while.
    e = 1; //pull high to build the rising edge.
}
//--------------------------------------
//lcd display setting

void writecmd(char x) {
    rs = 0; //is command not data
    rw = 0; //is write not read.
    PORTD = x;
    e = 0; //pull low enable signal.
    delay(6000); //for a while.
    e = 1; //pull high to build the rising edge
}
//--------------------------------------
//delay

void delay(int t) {
    int i;
    for (i = 0; i < t; i++);
}
//---------------------------------------

int strlen(char a[]) {
    int len = 0;
    while (a[len])
        len++;
    return len;
}

void writeString(char str[]) {
    for (int i = 0; i < strlen(str); ++i) {
        writechar(str[i]);
    }
}

void setCursorPos(int lineN, int pos) {
    const int ln1 = 0x80;
    const int ln2 = 0xC0;
    int addr;

    if (lineN == 1) {
        // Write to line 1.
        addr = ln1 + pos;
        writecmd(addr);
    } else if (lineN == 2) {
        // Write to line 2.
        addr = ln2 + pos;
        writecmd(addr);
    }
}
//main

void main() {
    while (1) {
        init(); //I/O init
        lcd_init(); //LCD init

        char *str = "World";
        setCursorPos(2, 3);
        writeString(str);

        char *str2 = "Hello";
        setCursorPos(1, 1);
        writeString(str2);

    }
}