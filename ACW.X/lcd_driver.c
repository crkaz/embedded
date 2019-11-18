#include <xc.h>
#include "Utils.h"
#include "lcd_driver.h"

#define rs RA5
#define rw RA4
#define e RA3

void writecmd(char command) {
    rs = 0; //is command not data
    rw = 0; //is write not read.
    PORTD = command;
    e = 0; //pull low enable signal.
    delay(60); //for a while.
    e = 1; //pull high to build the rising edge
}

void writechar(char character) {
    rs = 1; //is data not command.
    rw = 0; //is write not read.
    PORTD = character; //data send to PORTD
    e = 0; //pull low enable signal.
    delay(30); //for a while.
    e = 1; //pull high to build the rising edge.
}

void writeInt(int i) {
    writechar(i + 48);
}

void writeString(char str[]) {
    for (int i = 0; i < strlen(str); ++i) {
        writechar(str[i]);
    }
}

void setCursorPos(int lineN, int pos) {
    const int ln1 = 0xC0;
    const int ln2 = 0xD0;
    const int ln3 = 0xC8;
    const int ln4 = 0xD8;

    int addr;

    switch (lineN) {
        case 1: addr = ln1 + pos;
            break;
        case 2: addr = ln2 + pos;
            break;
        case 3: addr = ln3 + pos;
            break;
        case 4: addr = ln4 + pos;
            break;
        default: break;
    }

    writecmd(addr);
}

void lcd_init(void) {
    writecmd(0x0F); //display on,cursor on,blink on.
    writecmd(0x38); // 8 bits 2 lines 5*7 mode. / Set function
    writecmd(0x01); //clr screen
    //writecmd(0x02); 
}

void lcd_clear() {
    writecmd(0x01);
}

void lcd_home() {
    writecmd(0x02);
}