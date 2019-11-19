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
//#define psb RA2 // This is for lcd.

// Args for get/set_time_bit()
// NB: Write mode by default; +1 to set to read).
//const char SEC = 0x80;
//const char MIN = 0x82;
//const char HOUR = 0x84;
//const char DATE = 0x86;
//const char MONTH = 0x88;
//const char DAY = 0x8A;
//const char YEAR = 0x8C;
//const char CTRL = 0x8E;
//AM-PM/12-24 MODE
//Bit 7 of the hours register is defined as the 12? or 24?hour mode select bit. When high, the 12?hour
//mode is selected. In the 12?hour mode, bit 5 is the AM/PM bit with logic high being PM. In the 24?hour
//mode, bit 5 is the second 10-hour bit (20 ? 23 hours).

//define the time:       sec,  min,  hour, day, month, week, year, control word.
const char defaults[] = {0x00, 0x37, 0x13, 0x8, 0x3, 0x06, 0x19, 0x00};
unsigned char time_rx = 0x30; //define receive reg.
char table1[7]; //define the read time and date save table.

// Initialise required ports.

void rtc_port_init() {
    TRISA = 0x00; //a port all output
    TRISD = 0X00; //d port all output
    ADCON1 = 0X06; //a port all i/o
    //-------------
    TRISB = 0X02; //rb1 input, others output // CAN REMOVE?
    OPTION_REG = 0X00; //open b port internal pull high. // CAN REMOVE?
    //-------------
    PORTA = 0XFF;
    PORTD = 0XFF; //clear all display
}

// Initialise DS1302 clock.

void rtc_init() {
    sclk = 0; //pull low clock
    rst = 0; //reset DS1302
    rst = 1; //enable DS1302
    write_byte(0x8e); //send control command
    write_byte(0); //enable write DS1302
    rst = 0; //reset
}

// Set ALL components (sec, min, hour etc..) with BUST mode.

void set_time() {
    int i; //define the loop counter.
    rst = 1; //enable DS1302
    write_byte(0xbe); // Write burst mode.
    for (i = 0; i < 8; i++) //continue to write 8 bytes.
    {
        write_byte(defaults[i]); //write one byte
    }
    rst = 0; //reset
}

// SET INDIVIDUAL TIME COMPONENT
// set_time_bit(SEC, 0x30); // EXAMPL: setting second bit to 30sec.

void set_time_bit(char b, char t) {
    rst = 1; //enable DS1302
    write_byte(b); // Write minute bit.
    write_byte(t); //write one byte
    rst = 0; //reset
}

// Read ALL components (sec, min, hour etc..) with BUST mode.

void get_time() {
    int i; //set loop counter.
    rst = 1; //enable DS1302
    write_byte(0xbf); // Read burst mode.
    for (i = 0; i < 7; i++) //continue to read 7 bytes.
    {
        table1[i] = read_byte(); //
    }
    rst = 0; //reset DS1302
}

// GET INDIVIDUAL TIME COMPONENT AS INT
// get_time_bit(YEAR); // EXAMPLE: returning YEAR bit as int.

char* get_time_bit_as_string(char b) {
    rst = 1; //enable DS1302
    write_byte(b + 1); // Read individual bit (+ 1 sets read bit).
    char t = read_byte();
    rst = 0; //reset DS1302

    return BcdToStr(t); // Convert binary coded decimal to str for ease of use.
}

// Collate hour/min/second components and return a string.

char* get_time_as_string() {
    char t[] = {
        get_time_bit_as_string(HOUR)[0],
        get_time_bit_as_string(HOUR)[1],
        ':',
        get_time_bit_as_string(MIN)[0],
        get_time_bit_as_string(MIN)[1],
        ':',
        get_time_bit_as_string(SEC)[0],
        get_time_bit_as_string(SEC)[1],
        '\0'
    };

    return t;
}

// Write byte to active register.

void write_byte(unsigned char time_tx) {
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

unsigned char read_byte() {
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


//// FOR REFERENCE
//void main() {
//    port_init(); //port initilize.
//    rtc_init(); //DS1302 initilize.
//    set_time(); //set default time
//    while (1) {
//        get_time();
//        display_time();
//    }
//}


//// FOR REFERENCE
//void display_time() {
//    int i; //define table variable.
//    if (RB1 == 0) //judge rb1 to swap from time to date.
//    {
//        table1[0] = table1[3];
//        table1[1] = table1[4];
//        table1[2] = table1[6];
//    }
//    i = table1[0]&0x0f; //sec's low.
//    PORTD = table2[i]; //send to port d.
//    PORTA = 0x1f; //light on sec's low.
//    delay(); //delay some times.
//    i = table1[0]&0xf0; //sec's high
//    i = i >> 4; //rotate right for 4 bits.
//    PORTD = table2[i]; //send to port d.    
//    PORTA = 0x2f; //light on sec's high.
//    delay(); //delay some times.  
//
//    i = table1[1]&0x0f; //min's low.                 
//    PORTD = table2[i]&0x7f; //send to port d.            
//    PORTA = 0x37; //light on min's low.        
//    delay(); //delay some times.          
//    i = table1[1]&0xf0; //min's high                 
//    i = i >> 4; //rotate right for 4 bits.   
//    PORTD = table2[i]; //send to port d.            
//    PORTA = 0x3b; //light on min's high.       
//    delay(); //delay some times.          
//
//    i = table1[2]&0x0f; //hour's low.                 
//    PORTD = table2[i]&0x7f; //send to port d.            
//    PORTA = 0x3d; //light on hour's low.        
//    delay(); //delay some times.          
//    i = table1[2]&0xf0; //hour's high                 
//    i = i >> 4; //rotate right for 4 bits.   
//    PORTD = table2[i]; //send to port d.            
//    PORTA = 0x3e; //light on hour's high.       
//    delay(); //delay some times.          
//}