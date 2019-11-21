#include<pic.h>              //include MCU head file
int result;

void matrix_Init() {
    TRISC = 0XF0; //C PORT high 4 bits INPUT,low 4 bits OUTPUT    
}

void matrix_Scan() {
    const int HALFMASK = 0xF0;
    
    PORTC = 0XF7; //C3 OUTPUT low,the other 3 bits OUTPUT high                      
    NOP(); //delay                                                           
    result = PORTC; //read C PORT                                                     
    result &= HALFMASK; //clear low 4 bits                                                
    if (result != HALFMASK) //judge if high 4 bits all 1(all 1 is no key press)               
    {
        result = result | 0x07; //no,add low 4 bits 0x07 as key scan result                       
    } else //yes,change low 4 bits OUTPUT, judge if a key press again        
    {
        
        PORTC = 0XFB; //C2 OUTPUT low,the other 3 bits OUTPUT high                      
        NOP(); //delay                                                           
        result = PORTC; //read C PORT                                                     
        result &= HALFMASK; //clear low 4 bits                                                
        if (result != HALFMASK) //judge if high 4 bits all 1(all 1 is no key press)               
        {
            result |= 0x0B; //no,add low 4 bits 0x0b as key scan result                       
        } else //yes,change low 4 bits OUTPUT, judge if a key press again        
        {
            
            PORTC = 0XFd; //C1 OUTPUT low,the other 3 bits OUTPUT high                      
            NOP(); //delay                                                           
            result = PORTC; //read C PORT                                                     
            result &= HALFMASK; //clear low 4 bits                                                
            if (result != HALFMASK) //judge if high 4 bits all 1(all 1 is no key press)               
            {
                result |= 0x0D; //no,add low 4 bits 0x0d as key scan result                       
            } else //yes,change low 4 bits OUTPUT, judge if a key press again        
            {
                
                PORTC = 0XFe; //C0 OUTPUT low,the other 3 bits OUTPUT high                      
                NOP(); //delay                                                           
                result = PORTC; //read C PORT                                                     
                result &= HALFMASK; //clear low 4 bits                                                
                if (result != HALFMASK) //judge if high 4 bits all 1(all 1 is no key press)               
                {
                    result |= 0x0E; //no,add low 4 bits 0x0e as key scan result                    
                } else //yes,all key scan end,no key press,set no key press flag         
                {
                    result = 0xFF; //key scan result 0xff as no key press flag                       
                }
            }
        }
    }
}

char display() {
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
