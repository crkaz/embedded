#include"matrix_driver.h"

uch matrix_Scan(uch row); // Scan port C to detect the button pressed.
uch result;

void matrix_Init() {
    TRISC = 0XF0; //C PORT high 4 bits INPUT,low 4 bits OUTPUT    
}

char matrix_GetInput() {
    // Scan each row of the matrix for input.
    if (matrix_Scan(0XF7) == false)
        if (matrix_Scan(0XFB) == false)
            if (matrix_Scan(0XFD) == false)
                if (matrix_Scan(0XFE) == false)
                    result = 0xFF; // No input detected.

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

uch matrix_Scan(uch row) {
    const uch HALFMASK = 0xF0;

    PORTC = row; //C3 OUTPUT low,the other 3 bits OUTPUT high                      
    NOP(); //delay                                                           
    result = PORTC; //read C PORT                                                     
    result &= HALFMASK; //clear low 4 bits                                                

    if (result != HALFMASK) { //judge if high 4 bits all 1(all 1 is no key press)               
        result |= (row - HALFMASK); //no,add low 4 bits 0x07 as key scan result
        return true;
    }
    return false;
}