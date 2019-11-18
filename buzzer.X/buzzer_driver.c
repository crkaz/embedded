//HARDWARE CONFIG:
//SWITCHES: S10b3 ON (others OFF)                                               

#include <xc.h>
#include<pic.h>                                                                                                 

//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

#define  buz  RE1 // Use pin RE1.                                      
#define buz_on() buz = 1 // Set RE1 high.
#define buz_off() buz = 0; // Set RE1 low.

void buzzer_init(void);
void buzzer_sound(int bT, int pT);
void buzzer_alarm(void);
void getButtonInput(void);

void buzzer_init() {
    TRISE1 = 0; // Using pin E1 as output.                                        
    buz_off(); // Don't sound at the start.
    // Just for testing: get button input.
    TRISB0 = 1;
}

// Sound the buzzer. 
void buzzer_sound(int bT, int pT, int reps) {
    // bT = buzzer time (lower == higher pitch).
    // pT = pause time. Use for breaks in continuous buzzing (e.g. alarm).
    // reps = repetitions. Use to repeat tone (e.g. alarm).
    
    for (int i = 0; i < reps; ++i) {
        buz_on();
        for (int j = 0; j < bT; ++j);
        buz_off();
        for (int j = 0; j < pT; ++j);
    }
}

//void buzzer_alarm(int repeat, bT, pT) {
//    const int BUZZER_TIME = 12500;
//    const int PAUSE = 5000;
//
//    for (int i = 0; i < repeat; ++i) {
//        buzzer_sound(BUZZER_TIME, PAUSE);
//    }
//}

void getButtonInput() {
    if (RB0) { // Get input from RB0;
        //        buzzer_sound(5);
        //        buzzer_alarm();
        ALARM = 1;
    } else {
        ALARM = 0;
    }
}

//-------------------------------------------------------                                                       
//main function                                                                                                 

void main() {
    buzzer_init();
    while (1) {

        getButtonInput();
        if (ALARM) buzzer_alarm();

    }
}
