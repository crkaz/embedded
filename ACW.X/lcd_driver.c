#include <xc.h>
#include "Utils.h"
#include "lcd_driver.h"

#define rs RA5
#define rw RA4
#define e RA3

void lcd_Init(){
    ADCON1 = 0X07; //a port as ordinary i/o.
    TRISA = 0X00; //a port as output.
    TRISD = 0X00; //d port as output.
    
    lcd_WriteCmd(0x0F); //display on,cursor on,blink on.
    lcd_WriteCmd(0x01); //clr screen
    lcd_WriteCmd(0x38); // 8 bits 2 lines 5*7 mode. / Set function
}

void lcd_WriteCmd(char command) {
    rs = 0; //is command not data
    rw = 0; //is write not read.
    PORTD = command;
    e = 0; //pull low enable signal.
    Delay(30); //for a while.
    e = 1; //pull high to build the rising edge
}

void lcd_PrintChar(char character) {
    rs = 1; //is data not command.
    rw = 0; //is write not read.
    PORTD = character; //data send to PORTD
    e = 0; //pull low enable signal.
    Delay(20); //for a while.
    e = 1; //pull high to build the rising edge.
}

void lcd_PrintString(char str[], int lineN, int pos) {
    lcd_SetCursorPos(lineN, pos);
    for (int i = 0; i < StrLen(str); ++i) {
        lcd_PrintChar(str[i]);
    }
}

void lcd_SetCursorPos(int lineN, int pos) {
    const int ln1 = 0xC0; // Address of the start of line 1.
    const int ln2 = 0xD0; // Address of the start of line 2.
    const int ln3 = 0xC8; // Address of the start of line 3.
    const int ln4 = 0xD8; // Address of the start of line 4.

    int addr;
    switch (lineN) {
        case 0: addr = ln1 + pos; break;
        case 1: addr = ln2 + pos; break;
        case 2: addr = ln3 + pos; break;
        case 3: addr = ln4 + pos; break;
        default: break;
    }

    lcd_WriteCmd(addr);
}

   
void writeIntArray(char ints[]) {
	for (int i = 0; i < StrLen(ints); i++) {
		writeInt(ints[i]);
	}
}

void lcd_Clear() {
    lcd_WriteCmd(0x01);
}

void lcd_Home() {
    lcd_WriteCmd(0x02);
}