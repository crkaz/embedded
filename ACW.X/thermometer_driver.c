#include "thermometer_driver.h"

#define dq RE0
#define dq_dir TRISE0
#define set_dq_high() dq_dir = 1
#define set_dq_low() dq = 0; dq_dir = 0

int therm_ReadTemp(void); // Read the temperature from the thermistor and return as int.
void therm_Reset(void); // Reset the component.
void therm_WriteByte(uch val); // Write a command to the component.
uch therm_ReadByte(); // Read from the component.

uch TLV = 0; //temperature high byte                     
uch THV = 0; //temperature low byte   
char temperature[0x06]; //Stores the temperature

void therm_WriteByte(uch val) {
    uch i;
    uch temp;

    // Iterate through bits from l to r.
    for (i = 8; i > 0; i--) {
        temp = val & 0x01; // Shift lowest bit.
        set_dq_low();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        if (temp == 1) {
            set_dq_high();
        }
        DelayT(t63us);
        set_dq_high();
        NOP();
        NOP();
        val >>= 1; // right shift bit.
    }
}

uch therm_ReadByte() {
    uch i;
    uch value = 0; //read temperature         
    static char j;

    for (i = 8; i > 0; i--) {
        value >>= 1;
        set_dq_low();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        set_dq_high(); // pull high       
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        j = dq;
        if (j) value |= 0x80; // ????
        DelayT(t63us);
    }
    return (value);
}

int therm_ReadTemp() {
    //The sequence has to be
    // followed by each transaction: 1 Initialisation -> 2 ROM Function Command -> 3Memory
    // Function Command -> 4 Transaction/Data

    set_dq_high();
    therm_Reset(); //reset,wait for  18b20 response.                                                                                                              
    therm_WriteByte(0XCC); //ignore ROM matching                                                                                                                            
    therm_WriteByte(0X44); //send  temperature convert command                                                                                                              
    DelayT(t503us);
    DelayT(t503us);

    therm_Reset(); //reset again,wait for 18b20 response                                                                                                        
    therm_WriteByte(0XCC); //ignore ROM matching                                                                                                                            
    therm_WriteByte(0XBE); //send read temperature command                                                                                                                  
    TLV = therm_ReadByte(); //read temperature low byte                                                                                                                      
    THV = therm_ReadByte(); //read temperature high byte                                                                                                                   
    set_dq_high(); //release general line                                                                                                                           
    return (TLV >> 4) | ((THV << 4) & 0X3f); //temperature value
}

void therm_Reset(void) {
    char presence = 1;
    while (presence) {
        set_dq_low(); // MCU pull low
        DelayT(t503us);
        set_dq_high(); // release general line and wait for pull high
        DelayT(t70us);
        //        presence = (dq == 1) ? 1 : 0; // Didn't receive response : Did Receive response.

        if (dq == 1)
            presence = 1; // Didn't receive response.
        else
            presence = 0; // Received response.
        DelayT(t430us);
    }
}

char* therm_GetTemp() {
    int TZ = therm_ReadTemp();
    unsigned int wd = 0; //temperature BCD code  after convert
    uch TX = TLV << 4; //temperature decimal

    temperature[0x00] = TZ / 0x0A + 0x30; //integer ten bit	
    temperature[0x01] = TZ % 0x0A + 0x30; //integer Entries bit                                                                                                                            
    temperature[0x02] = '.';

    if (TX & 0x80) {
        wd += 5000;
    }
    if (TX & 0x40) {
        wd += 2500;
    }
    if (TX & 0x20) {
        wd += 1250;
    }
    if (TX & 0x10) {
        wd += 625;
    }

    temperature[0x03] = wd / 1000 + 0x30; //ten cent bit                                                                           
    temperature[0x04] = 'C';
    temperature[0x05] = eol;

    return temperature;
}