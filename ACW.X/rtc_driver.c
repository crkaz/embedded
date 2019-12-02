#include "rtc_driver.h"

#define i_o   RB4 //1302I_O           
#define sclk  RB0 //1302 clock        
#define rst   RB5 //1302 enable bit   

char rtc_ReadByte(void); // Privatised.
void rtc_WriteByte(char time_tx); // Privatised.

//AM-PM/12-24 MODE
//Bit 7 of the hours register is defined as the 12? or 24?hour mode select bit. When high, the 12?hour
//mode is selected. In the 12?hour mode, bit 5 is the AM/PM bit with logic high being PM. In the 24?hour
//mode, bit 5 is the second 10-hour bit (20 ? 23 hours).

//define the time:       sec,  min,  hour, day, month, week, year, control word.
//const char defaults[] = {0x45, 0x04, 0x02, 0x26, 0x11, 0x17, 0x19, 0x00};
uch time_rx = 0x30; //define receive reg.
char rtc_Vals[0x08]; //define the read time and date save table.
char rtc_StrVals[0x09]; //define the read time and date save table.

// Initialise DS1302 clock.

void rtc_Init() {
    TRISA = 0x00; //a port all output
    TRISD = 0x00; //d port all output
    ADCON1 = 0x06; //a port all i/o
    TRISB = 0x02; //rb1 input, others output.
    OPTION_REG = 0X00; //open b port internal pull high.
    PORTA = 0XFF;
    PORTD = 0XFF; //clear all display

    sclk = 0; //pull low clock
    rst = 0; //reset DS1302
    rst = 1; //enable DS1302
    rtc_WriteByte(0x8e); //send control command
    rtc_WriteByte(0); //enable write DS1302
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
// set_time_bit(SEC, 0x30); // EXAMPL: setting second bit to 30sec.

void rtc_SetTimeComponent(char b, char t) {
    rst = 1; // Enable.
    rtc_WriteByte(b); // Select time component.
    rtc_WriteByte(t); // Set time component to t.
    rst = 0; // Reset.
}

// Read ALL components (sec, min, hour etc..) with BURST mode.

void rtc_Update() {
    rst = 1; //enable DS1302
    rtc_WriteByte(0xbf); // Read burst mode.
    for (char i = 0x00; i < 0x07; ++i) //continue to read 7 bytes.
    {
        rtc_Vals[i] = rtc_ReadByte();
    }
    rst = 0; //reset DS1302
}

// Write byte to active register.

void rtc_WriteByte(char addr) {
    for (char i = 0x00; i < 0x08; ++i) //continue to write 8bit
    {
        i_o = 0; //
        sclk = 0; //pull low clk
        if (addr & 1) //judge the send bit is 0 or 1.
        {
            i_o = 1; //is 1
        }
        addr = addr >> 1; //rotate right 1 bit.
        sclk = 1; //pull high clk
    }
    sclk = 0; //finished 1 byte,pull low clk
}

// Read byte from active register.

char rtc_ReadByte() {
    TRISB4 = 1; //continue to write 8bit
    for (char i = 0x00; i < 0x08; ++i) {
        sclk = 0; //pull low clk
        time_rx = time_rx >> 1; //judge the send bit is 0 or 1.
        if (i_o) {
            time_rx |= 0x80; // Sets 128th bit ?
        }
        sclk = 1; //pull high clk
    }
    TRISB4 = 0; //finished 1 byte,pull low clk
    sclk = 0;
    return time_rx;
}

// Convert date/time bcd to formatted string.

char *rtc_GetString(char isDate) {
    char seperator = (0x0D * !isDate) + 0x2D; // 58'-' for date or 45':' for time.
    char year = isDate * 0x01; // == 0 if not setting date.
    isDate *= 0x03; // Array offset.
    char j = 0x02; // Array indexer.
    for (char i = 0x00; i < 0x09; i += 0x03) {
        if (i > 0x00) year = 0x00;

        // HRS MIN SEC YRS MNT DAT
        //  2,  1,  0,  6,  4,  3
        rtc_StrVals[i] = BcdToStr(rtc_Vals[j + isDate + year])[0x00]; // 0, 3, 6 = 2, 1, 0 || 5(+1), 4, 3
        rtc_StrVals[i + 0x01] = BcdToStr(rtc_Vals[j + isDate + year])[0x01]; // 1, 4, 7
        rtc_StrVals[i + 0x02] = seperator; // 2, 5, 8
        j--;
    }
    rtc_StrVals[0x08] = eol;

    return rtc_StrVals;
}
