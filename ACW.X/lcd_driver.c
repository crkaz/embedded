#include "lcd_driver.h"

void WriteCmd(char command); // Privatise.

void lcd_Init() {
    psb = 1; // As parallel.
    ADCON1 = 0X07; //a port as ordinary i/o.
    TRISA = 0X00; //a port as output.
    TRISD = 0X00; //d port as output.

    WriteCmd(0x0C); //display on, cursor off, blink off.
    WriteCmd(0x01); //clr screen
    WriteCmd(0x38); // 8 bits 2 lines 5*7 mode. / Set function
}

void WriteCmd(char command) {
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
    rs = 1; //is data not command.
    rw = 0; //is write not read.
    e = 0;
    Delay(10); //for a while.
    e = 1;
    PORTD = character;
    Delay(10); //for a while.
    e = 0; //pull low enable signal.
}

void lcd_PrintString(char str[], int lineN, int pos) {
    lcd_SetCursorPos(lineN, pos);
    for (int i = 0; i < StrLen(str); ++i) {
        lcd_PrintChar(str[i]);
    }
}

const int LN1 = 0xC0; // Address of the start of line 1.
const int LN2 = 0xD0; // Address of the start of line 2.
const int LN3 = 0xC8; // Address of the start of line 3.
const int LN4 = 0xD8; // Address of the start of line 4.
void lcd_SetCursorPos(int lineN, int pos) {
    int addr;
    switch (lineN) {
        case 0: addr = LN1 + pos;
            break;
        case 1: addr = LN2 + pos;
            break;
        case 2: addr = LN3 + pos;
            break;
        case 3: addr = LN4 + pos;
            break;
        default: break;
    }

    WriteCmd(addr);
}

//void writeIntArray(char ints[]) {
//    for (int i = 0; i < StrLen(ints); i++) {
//        writeInt(ints[i]);
//    }
//}

void lcd_Clear() {
    WriteCmd(0x01);
}

//void lcd_Home() {
//    WriteCmd(0x02);
//}

void lcd_CursorStatus(int i) {
    if (i == 0) {
        WriteCmd(0x0C);
    } else if (i == 1) {
        WriteCmd(0x0F);
    }
}