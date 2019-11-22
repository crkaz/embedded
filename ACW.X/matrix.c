#include<pic.h>  //include MCU head file

int scan(int row); // Privatised.

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
        case 0xe7: return '0';
        case 0xeb: return '1';
        case 0xed: return '2';
        case 0xee: return '3';
        case 0xd7: return '4';
        case 0xdb: return '5';
        case 0xdd: return '6';
        case 0xde: return '7';
        case 0xb7: return '8';
        case 0xbb: return '9';
        case 0xbd: return 'A';
        case 0xbe: return 'B';
        case 0x77: return 'C';
        case 0x7b: return 'D';
        case 0x7d: return 'E';
        case 0x7e: return 'F';
    }
    return ' ';
}

int scan(int row) {
    const int HALFMASK = 0xF0;

    PORTC = row; //C3 OUTPUT low,the other 3 bits OUTPUT high                      
    NOP(); //delay                                                           
    result = PORTC; //read C PORT                                                     
    result &= HALFMASK; //clear low 4 bits                                                

    if (result != HALFMASK) { //judge if high 4 bits all 1(all 1 is no key press)               
        result |= (row - 0xF0); //no,add low 4 bits 0x07 as key scan result
        return 1;
    }
    return 0;
}