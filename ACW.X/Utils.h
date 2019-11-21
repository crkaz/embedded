// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_Utils_H
	#define	XC_Utils_H
	
	#include <xc.h> // include processor files - each processor file is guarded.  
	
	void Delay(int t);
	void DelayT(int t[]);
    int StrLen(char*); // Get length of a string.
    char* BcdToStr(char); // Convert BCD (binary coded decimal) to decimal.
    
#endif	/* XC_Utils_H */

