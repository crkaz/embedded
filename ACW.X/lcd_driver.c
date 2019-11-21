#include <xc.h>
#include "Utils.h"
#include "lcd_driver.h"


void lcd_WriteCmd(char command) {
    rs = 0; //is command not data
    rw = 0; //is write not read.
    PORTD = command;
    e = 0; //pull low enable signal.
    Delay(60); //for a while.
    e = 1; //pull high to build the rising edge
}

void lcd_PrintChar(char character) {
    rs = 1; //is data not command.
    rw = 0; //is write not read.
    PORTD = character; //data send to PORTD
    e = 0; //pull low enable signal.
    Delay(30); //for a while.
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

   
void writeIntArray(char ints[]) {
	for (int i = 0; i < strlen(ints); i++) {
		writeInt(ints[i]);
	}
}

int addr;
void setCursorPos(int lineN, int pos) {
	const int ln1 = 0xC0;
	const int ln2 = 0xD0;
	const int ln3 = 0xC8;
	const int ln4 = 0xD8;

    switch (lineN) {
        case 1: addr = ln1 + pos; break;
        case 2: addr = ln2 + pos; break;
        case 3: addr = ln3 + pos; break;
        case 4: addr = ln4 + pos; break;
        default: break;
    }

    lcd_WriteCmd(addr);
}

void lcd_Init(void) {
    lcd_WriteCmd(0x0F); //display on,cursor on,blink on.
    lcd_WriteCmd(0x38); // 8 bits 2 lines 5*7 mode. / Set function
    lcd_WriteCmd(0x01); //clr screen
    //writecmd(0x02); // Home.
}

void lcd_Clear() {
    lcd_WriteCmd(0x01);
}

void lcd_Home() {
    lcd_WriteCmd(0x02);
}