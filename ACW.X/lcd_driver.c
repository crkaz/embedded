#include "lcd_driver.h"

#define rs RA5
#define rw RA4
#define e RA3
#define psb RA2

void lcd_WriteCmd(char command); // Privatise.

void lcd_Init() {
    psb = 1; // As parallel.
    ADCON1 = 0X07; //a port as ordinary i/o.
    TRISA = 0X00; //a port as output.
    TRISD = 0X00; //d port as output.

    lcd_WriteCmd(0x0C); //display on, cursor off, blink off.
    lcd_WriteCmd(0x01); //clr screen
    lcd_WriteCmd(0x38); // 8 bits 2 lines 5*7 mode. / Set function
}

void lcd_WriteCmd(char command) {
    rs = 0; //is command not data
    rw = 0; //is write not read.
    e = 0;
    Delay(10); //for a while.
    e = 1;
    PORTD = command;
    Delay(10); //for a while.
    e = 0; //pull low enable signal.
}

void lcd_PrintChar(char character) {
    if (character > '~') { //Stops Chinese characters
        character = ' ';
    }
    rs = 1; //is data not command.
    rw = 0; //is write not read.
    e = 0;
    Delay(10); //for a while.
    e = 1;
    PORTD = character;
    Delay(10); //for a while.
    e = 0; //pull low enable signal.
}

void lcd_PrintString(char str[], uch lineN, uch pos) {
    lcd_SetCursorPos(lineN, pos);
    for (uch i = 0; i < StrLen(str); ++i) {
        lcd_PrintChar(str[i]);
    }
}

void lcd_SetCursorPos(uch lineN, uch pos) {
    int addr;
    switch (lineN) {
        case 0: addr = 0xC0 + pos;
            break;
        case 1: addr = 0xD0 + pos;
            break;
        case 2: addr = 0xC8 + pos;
            break;
        case 3: addr = 0xD8 + pos;
            break;
    }

    lcd_WriteCmd(addr);
}

void lcd_Clear() {
    lcd_WriteCmd(0x01);
    Delay(50);
}

//void lcd_CursorStatus(char i) {
//    if (i == 0) {
//        WriteCmd(0x0C);
//    } else if (i == 1) {
//        WriteCmd(0x0F);
//    }
//}