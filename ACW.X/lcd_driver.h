// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_LCD_H
	#define	XC_LCD_H
	
	#include <xc.h> // include processor files - each processor file is guarded.  
	void lcd_WriteCmd(char command);
	void lcd_PrintChar(char character);
	void writeInt(int i);
	void lcd_Init(void);
	void lcd_PrintString(char str[], int lineN, int pos);
	void lcd_SetCursorPos(int lineN, int pos);
	void lcd_Init(void);
    void lcd_Clear(void);
    void lcd_Home(void);

#endif	/* XC_HEADER_TEMPLATE_H */

