#include "rtc_driver.h"
#include "buzzer_driver.h"

unsigned char ReadByte(void); // Privatised.
void WriteByte(unsigned char time_tx); // Privatised.
char tArr[9]; // Time array for returning date or time as string.
char dArr[11]; // Date array for returning date or time as string.

//AM-PM/12-24 MODE
//Bit 7 of the hours register is defined as the 12? or 24?hour mode select bit. When high, the 12?hour
//mode is selected. In the 12?hour mode, bit 5 is the AM/PM bit with logic high being PM. In the 24?hour
//mode, bit 5 is the second 10-hour bit (20 ? 23 hours).

//define the time:       sec,  min,  hour, day, month, week, year, control word.
//const char defaults[] = {0x00, 0x23, 0x21, 0x23, 0x11, 0x17, 0x19, 0x00};
unsigned char time_rx = 0x30; //define receive reg.
//char table1[7]; //define the read time and date save table.


// Initialise DS1302 clock.

void rtc_Init() {
    TRISA = 0x00; //a port all output
    TRISD = 0X00; //d port all output
    ADCON1 = 0X06; //a port all i/o
    TRISB = 0X02; //rb1 input, others output.
    OPTION_REG = 0X00; //open b port internal pull high.
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
//
//void rtc_SetTime() {
//    int i; //define the loop counter.
//    rst = 1; //enable DS1302
//    WriteByte(0xbe); // Write burst mode.
//    for (i = 0; i < 8; i++) //continue to write 8 bytes.
//    {
//        WriteByte(defaults[i]); //write one byte
//    }
//    rst = 0; //reset
//}

// SET INDIVIDUAL TIME COMPONENT
// rtc_SetTimeComponent(SEC, 0x30); // EXAMPL: setting second bit to 30sec.

void rtc_SetTimeComponent(char b, char t) {
//    buzzer_sound(1500, 1500, 5);
    rst = 1; //enable DS1302
    WriteByte(b); // Write minute bit.
    WriteByte(t); //write one byte
    rst = 0; //reset
//    buzzer_sound(1500, 1500, 5);
}

// Read ALL components (sec, min, hour etc..) with BURST mode.

//char* rtc_GetTime() {
//    int i; //set loop counter.
//    rst = 1; //enable DS1302
//    WriteByte(0xbf); // Read burst mode.
//    for (i = 0; i < 7; i++) //continue to read 7 bytes.
//    {
//        table1[i] = ReadByte(); //
//    }
//    rst = 0; //reset DS1302
//
//    return table1;
//}

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
    ADCON1 = 0X06; //a port all i/o

    int j; //set the loop counter.
    for (j = 0; j < 8; j++) { //continue to write 8bit
        i_o = 0; //
        sclk = 0; //pull low clk
        if (time_tx & 0x01) { //judge the send bit is 0 or 1.
            i_o = 1; //is 1
        }
        time_tx >>= 1; //rotate right 1 bit.
        sclk = 1; //pull high clk
    }
    sclk = 0; //finished 1 byte,pull low clk
}

// Read byte from active register.

unsigned char ReadByte() {
    ADCON1 = 0X06; //a port all i/o

    int j; //set the loop counter.  
    TRISB4 = 1; //continue to write 8bit 
    for (j = 0; j < 8; j++) {
        sclk = 0; //pull low clk                   
        time_rx >>= 1; //judge the send bit is 0 or 1.  
        if (i_o) {
            time_rx |= 0x80; // Sets 128th bit ?
        }
        sclk = 1; //pull high clk                 
    }
    TRISB4 = 0; //finished 1 byte,pull low clk  
    sclk = 0;
    return (time_rx);
}
//
//char* rtc_GetTimeComponentAsString(char b) {
//    rst = 1; //enable DS1302
//    WriteByte(b + 1); // Read individual bit (+ 1 sets read bit).
//    char t = ReadByte();
//    rst = 0; //reset DS1302
//
//    return BcdToStr(t); // Convert binary coded decimal to str for ease of use.
//}

// Collate hour/min/second components and return a string.

char* rtc_GetTimeString() {
    tArr[0] = BcdToStr(rtc_GetTimeComponent(HOUR))[0];
    tArr[1] = BcdToStr(rtc_GetTimeComponent(HOUR))[1];
    tArr[2] = ':';
    tArr[3] = BcdToStr(rtc_GetTimeComponent(MIN))[0];
    tArr[4] = BcdToStr(rtc_GetTimeComponent(MIN))[1];
    tArr[5] = ':';
    tArr[6] = BcdToStr(rtc_GetTimeComponent(SEC))[0];
    tArr[7] = BcdToStr(rtc_GetTimeComponent(SEC))[1];
    tArr[8] = '\0';

    return tArr;
}

// Collate year/month/day components and return a string.

char* rtc_GetDateString() {
    dArr[0] = '2';
    dArr[1] = '0';
    dArr[2] = BcdToStr(rtc_GetTimeComponent(YEAR))[0];
    dArr[3] = BcdToStr(rtc_GetTimeComponent(YEAR))[1];
    dArr[4] = '-';
    dArr[5] = BcdToStr(rtc_GetTimeComponent(MONTH))[0];
    dArr[6] = BcdToStr(rtc_GetTimeComponent(MONTH))[1];
    dArr[7] = '-';
    dArr[8] = BcdToStr(rtc_GetTimeComponent(DATE))[0];
    dArr[9] = BcdToStr(rtc_GetTimeComponent(DATE))[1];
    dArr[10] = '\0';
    return dArr;
}
