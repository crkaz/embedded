// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_LCD_H
    #define	XC_LCD_H

    #include <xc.h> // include processor files - each processor file is guarded.  
    #include "Utils.h"
    //	void WriteCmd(char command); // Privatised.

    #define rs RA5
    #define rw RA4
    #define e RA3
    #define psb RA2

    char lcd_EMPTY_STRING[] = "               \0"; // Empty string for clearing a line.

    void lcd_CursorStatus(int i); // Switch cursor on (1) or off (0);
    void lcd_SetCursorPos(int lineN, int pos);
    void lcd_Init(void);
    void lcd_PrintChar(char character);
    void lcd_PrintString(char str[], int lineN, int pos);
    void lcd_Clear(void);
    void lcd_Home(void);
//    void writeInt(int i);

#endif	/* XC_HEADER_TEMPLATE_H */

