// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_Utils_H
	#define	XC_Utils_H
	
	#include <xc.h> // include processor files - each processor file is guarded.  
	
	void delay(int t);
	void delay2(int t[]);
	int strlen(char a[]);
	void resetThermometer(void);
#endif	/* XC_Utils_H */

