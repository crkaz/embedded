#include <xc.h>
//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

#define rs RA5
#define rw RA4
#define e RA3
#define psb RA2
#define dq RE0
#define dq_dir TRISE0
#define set_dq_high() dq_dir = 1
#define set_dq_low() dq = 0; dq_dir = 0

#define clk RB0;
#define io RB4;
#define rst RB5;

// Timings for delay2 args. Cannot be const.
int t503us[2] = {2, 70};
int t430us[2] = {2, 60};
int t70us[2] = {2, 8};
int t63us[2] = {2, 7};
int tUKus[2] = {1, 70};

//
void init(); //I/O init
void delay(); //delay
void delay2(int arr[]);

//I/O init

void init() {
    ADCON1 = 0X07; //a port as ordinary i/o.
    TRISA = 0X00; //a port as output.
    TRISD = 0X00; //d port as output.
    psb = 1;
}

int strlen(char a[]) {
    int len = 0;
    while (a[len])
        len++;
    return len;
}

void delay(int t) {
    for (int i = 0; i < t; ++i);
}

void delay2(int t[]) {
    char x = t[0];
    char y = t[1];

    char z;
    do {
        z = y;
        do {
            ;
        } while (--z);
    } while (--x);
}

void writecmd_clk(char x){
    // [  7  ][  6   ][  5  ][  4  ][  3  ][  2  ][  1  ][  0  ]
    // [  1  ][RAM/CK][  A4 ][  A3 ][  A2 ][  A1 ][  A0 ][ RD/W]
    // Command byte is input starting with LSb ([0]).

}

void writechar_clk(char x){
}

void writestring_clk(char a[]){
    
}

//main

void main() {
    init(); //call system initialize function                                                                                                                                 
    lcd_init();

    while (1) {
        display_temp(); //call display function
    }
}