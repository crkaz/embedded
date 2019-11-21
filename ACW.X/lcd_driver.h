// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_LCD_H
	#define	XC_LCD_H
	
	#include <xc.h> // include processor files - each processor file is guarded.  
//	void WriteCmd(char command); // Privatised.
//	void SetCursorPos(int lineN, int pos); // Privatised.
	void lcd_Init(void);
	void lcd_PrintChar(char character);
	void lcd_PrintString(char str[], int lineN, int pos);
    void lcd_Clear(void);
    void lcd_Home(void);
	void writeInt(int i);

#endif	/* XC_HEADER_TEMPLATE_H */

