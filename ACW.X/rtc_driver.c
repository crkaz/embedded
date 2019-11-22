// HARDWARE CONFIG:
// Clock chip: DS1302
// Switches ON: S9bX, S5bX, S6bX, S1b7 (all other switches off).

#include<pic.h> //include MCU head file     
#include <xc.h>
#include "Utils.h"
#include "rtc_driver.h"

// DS1302 pin config.
#define i_o   RB4 //1302I_O           
#define sclk  RB0 //1302 clock        
#define rst   RB5 //1302 enable bit   

unsigned char ReadByte(); // Privatised.
void WriteByte(unsigned char time_tx); // Privatised.

//AM-PM/12-24 MODE
//Bit 7 of the hours register is defined as the 12? or 24?hour mode select bit. When high, the 12?hour
//mode is selected. In the 12?hour mode, bit 5 is the AM/PM bit with logic high being PM. In the 24?hour
//mode, bit 5 is the second 10-hour bit (20 ? 23 hours).

//define the time:       sec,  min,  hour, day, month, week, year, control word.
const char defaults[] = {0x00, 0x13, 0x17, 0x21, 0x11, 0x17, 0x19, 0x00};
unsigned char time_rx = 0x30; //define receive reg.
char table1[7]; //define the read time and date save table.


// Initialise DS1302 clock.

void rtc_Init() {
    TRISA = 0x00; //a port all output
    TRISD = 0X00; //d port all output
    ADCON1 = 0X06; //a port all i/o
    //-------------
    TRISB = 0X02; //rb1 input, others output // CAN REMOVE?
    OPTION_REG = 0X00; //open b port internal pull high. // CAN REMOVE?
    //-------------
    PORTA = 0XFF;
    PORTD = 0XFF; //clear all display

    sclk = 0; //pull low clock
    rst = 0; //reset DS1302
    rst = 1; //enable DS1302
    WriteByte(0x8e); //send control command
    WriteByte(0); //enable write DS1302
    rst = 0; //reset
}

// Set ALL components (sec, min, hour etc..) with BUST mode.

void rtc_SetTime() {
    int i; //define the loop counter.
    rst = 1; //enable DS1302
    WriteByte(0xbe); // Write burst mode.
    for (i = 0; i < 8; i++) //continue to write 8 bytes.
    {
        WriteByte(defaults[i]); //write one byte
    }
    rst = 0; //reset
}

// SET INDIVIDUAL TIME COMPONENT
// set_time_bit(SEC, 0x30); // EXAMPL: setting second bit to 30sec.

void rtc_SetTimeComponent(char b, char t) {
    rst = 1; //enable DS1302
    WriteByte(b); // Write minute bit.
    WriteByte(t); //write one byte
    rst = 0; //reset
}

// Read ALL components (sec, min, hour etc..) with BURST mode.

char* rtc_GetTime() {
    int i; //set loop counter.
    rst = 1; //enable DS1302
    WriteByte(0xbf); // Read burst mode.
    for (i = 0; i < 7; i++) //continue to read 7 bytes.
    {
        table1[i] = ReadByte(); //
    }
    rst = 0; //reset DS1302

    return table1;
}

// Get a component of time as a binary coded decimal.

char rtc_GetTimeComponent(char b) {
    rst = 1; //enable DS1302
    WriteByte(b + 1); // Read individual bit (+ 1 sets read bit).
    char t = ReadByte();
    rst = 0; //reset DS1302

    return t;
}

// Write byte to active register.
void WriteByte(unsigned char time_tx) {
    int j; //set the loop counter.
    for (j = 0; j < 8; j++) //continue to write 8bit
    {
        i_o = 0; //
        sclk = 0; //pull low clk
        if (time_tx & 0x01) //judge the send bit is 0 or 1.
        {
            i_o = 1; //is 1
        }
        time_tx = time_tx >> 1; //rotate right 1 bit.
        sclk = 1; //pull high clk
    }
    sclk = 0; //finished 1 byte,pull low clk
}

// Read byte from active register.
unsigned char ReadByte() {
    int j; //set the loop counter.  
    TRISB4 = 1; //continue to write 8bit 
    for (j = 0; j < 8; j++) {
        sclk = 0; //pull low clk                   
        time_rx = time_rx >> 1; //judge the send bit is 0 or 1.  
        if (i_o) {
            time_rx |= 0x80; // Sets 128th bit ?
        }
        sclk = 1; //pull high clk                 
    }
    TRISB4 = 0; //finished 1 byte,pull low clk  
    sclk = 0;
    return (time_rx);
}

char* rtc_GetTimeComponentAsString(char b) {
    rst = 1; //enable DS1302
    WriteByte(b + 1); // Read individual bit (+ 1 sets read bit).
    char t = ReadByte();
    rst = 0; //reset DS1302

    return BcdToStr(t); // Convert binary coded decimal to str for ease of use.
}

// Collate hour/min/second components and return a string.
char* rtc_GetTimeString() {
    char t[] = {
        rtc_GetTimeComponentAsString(HOUR)[0],
        rtc_GetTimeComponentAsString(HOUR)[1],
        ':',
        rtc_GetTimeComponentAsString(MIN)[0],
        rtc_GetTimeComponentAsString(MIN)[1],
        ':',
        rtc_GetTimeComponentAsString(SEC)[0],
        rtc_GetTimeComponentAsString(SEC)[1],
        '\0',
    };
    return t;
}

// Collate year/month/day components and return a string.
char* rtc_GetDateString() {
    char d[] = {
        '2',
        '0',
        rtc_GetTimeComponentAsString(YEAR)[0],
        rtc_GetTimeComponentAsString(YEAR)[1],
        '-',
        rtc_GetTimeComponentAsString(MONTH)[0],
        rtc_GetTimeComponentAsString(MONTH)[1],
        '-',
        rtc_GetTimeComponentAsString(DAY)[0],
        rtc_GetTimeComponentAsString(DAY)[1],
        '\0',
    };
    return d;
}
