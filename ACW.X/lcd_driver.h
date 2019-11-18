// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_LCD_H
	#define	XC_LCD_H
	
	#include <xc.h> // include processor files - each processor file is guarded.  
	void writecmd(char command);
	void writechar(char character);
	void writeInt(int i);
	void writeString(char str[]);
	void writeIntArray(char ints[]);
	void setCursorPos(int lineN, int pos);
	void lcd_init(void);
    
    #define rs RA5
#define rw RA4
#define e RA3

#endif	/* XC_HEADER_TEMPLATE_H */

