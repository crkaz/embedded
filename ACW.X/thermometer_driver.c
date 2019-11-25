#include "thermometer_driver.h"

void resetThermometer(void); // Privatised.
void write_byte(unsigned char val); // Privatised.
unsigned char read_byte(); // Privatised.

unsigned char TLV = 0; //temperature high byte                     
unsigned char THV = 0; //temperature low byte   
//char temperature[8]; //Stores the temperature
char temperature[6]; //Stores the temperature

void write_byte(unsigned char val) {
    unsigned char i;
    unsigned char temp;

    // Iterate through bits from l to r.
    for (i = 8; i > 0; i--) {
        temp = val & 0x01; // Shift lowest bit.
        set_dq_low();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        //        NOP();
        if (temp == 1) {
            set_dq_high();
        }
        DelayT(t63us);
        set_dq_high();
        NOP();
        NOP();
        val = val >> 1; // right shift bit.
    }
}

unsigned char read_byte() {
    unsigned char i;
    unsigned char value = 0; //read temperature         
    static char j;

    for (i = 8; i > 0; i--) {
        value >>= 1;
        set_dq_low();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP(); //6us              
        set_dq_high(); // pull high       
        NOP();
        NOP();
        NOP();
        NOP();
        NOP(); //4us               
        j = dq;
        if (j) value |= 0x80; // ????
        DelayT(t63us); //63us              
    }
    return (value);
}

int get_temp() {
    //The sequence has to be
    // followed by each transaction: 1 Initialisation -> 2 ROM Function Command -> 3Memory
    // Function Command -> 4 Transaction/Data

    set_dq_high();
    resetThermometer(); //reset,wait for  18b20 response.                                                                                                              
    write_byte(0XCC); //ignore ROM matching                                                                                                                            
    write_byte(0X44); //send  temperature convert command                                                                                                              
    DelayT(t503us);
    DelayT(t503us);

    resetThermometer(); //reset again,wait for 18b20 response                                                                                                        
    write_byte(0XCC); //ignore ROM matching                                                                                                                            
    write_byte(0XBE); //send read temperature command                                                                                                                  
    TLV = read_byte(); //read temperature low byte                                                                                                                      
    THV = read_byte(); //read temperature high byte                                                                                                                   
    set_dq_high(); //release general line                                                                                                                           
    return (TLV >> 4) | ((THV << 4) & 0X3f); //temperature value
}

void resetThermometer(void) {
    char presence = 1;
    while (presence) {
        set_dq_low(); // MCU pull low
        DelayT(t503us);
        set_dq_high(); // release general line and wait for pull high
        DelayT(t70us);
        if (dq == 1)
            presence = 1; // Didn't receive response.
        else
            presence = 0; // Received response.

        DelayT(t430us);
    }
}

char* calculate_temp(int TZ) {
    unsigned int wd = 0; //temperature BCD code  after convert
    unsigned char TX = TLV << 4; //temperature decimal

    temperature[0] = TZ / 10 + 48; //integer ten bit	
    temperature[1] = TZ % 10 + 48; //integer Entries bit                                                                                                                            
    temperature[2] = '.';
    
    if (TX & 0x80) {
        wd = wd + 5000;
    }
    if (TX & 0x40) {
        wd = wd + 2500;
    }
    if (TX & 0x20) {
        wd = wd + 1250;
    }
    if (TX & 0x10) {
        wd = wd + 625; //hereinbefore four instructions are turn  decimal into BCD code        
    }

    temperature[3] = wd / 1000 + 48; //ten cent bit                                                                           
    //	temperature[4] = (wd % 1000) / 100  + 48; //hundred cent bit                                                                       
    //	temperature[5] = (wd % 100) / 10 + 48; //thousand cent bit                                                                      
    //	temperature[6] = wd % 10 + 48; //myriad cent bit
    temperature[4] = 'C';
    temperature[5] = '\0';

    return temperature;
}