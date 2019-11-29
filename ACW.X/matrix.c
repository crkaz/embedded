#include"matrix.h"

char scan(unsigned char row); // Privatised.

void matrix_Init() {
    TRISC = 0XF0; //C PORT high 4 bits INPUT,low 4 bits OUTPUT    
}
int result;

char matrix_Scan() {
    if (scan(0XF7) == 0)
        if (scan(0XFB) == 0)
            if (scan(0XFD) == 0)
                if (scan(0XFE) == 0)
                    result = 0xFF;

    switch (result) {
        case 0xe7: return 's'; // Select. //// Bottom right.
        case 0xeb: return 'b'; // Backspace.
        case 0xed: return '.';
        case 0xee: return '0';

        case 0xd7: return '>'; // Down/next.
        case 0xdb: return '3';
        case 0xdd: return '2';
        case 0xde: return '1';

        case 0xb7: return '<'; // Up/previous.
        case 0xbb: return '6';
        case 0xbd: return '5';
        case 0xbe: return '4';

        case 0x77: return 'x'; // Cancel/close/back.
        case 0x7b: return '9';
        case 0x7d: return '8';
        case 0x7e: return '7'; //// Top left.
    }
    return '_'; // Nothing.
}

char scan(unsigned char row) {
    char HALFMASK = 0xF0;

    PORTC = row; //C3 OUTPUT low,the other 3 bits OUTPUT high                      
    NOP(); //delay                                                           
    result = PORTC; //read C PORT                                                     
    result &= HALFMASK; //clear low 4 bits                                                

    if (result != HALFMASK) { //judge if high 4 bits all 1(all 1 is no key press)               
        result |= (row - HALFMASK); //no,add low 4 bits 0x07 as key scan result
        return 1;
    }
    return 0;
}