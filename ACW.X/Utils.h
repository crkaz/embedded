// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_Utils_H
	#define	XC_Utils_H
	
	#include <xc.h> // include processor files - each processor file is guarded.  
    // Timings for delay2 args. Cannot be const.
    int t503us[2] = { 2, 70 };
    int t430us[2] = { 2, 60 };
    int t70us[2] = { 2, 8 };
    int t63us[2] = { 2, 7 };
    int tUKus[2] = { 2, 9 };
    
	void Delay(int t);
	void DelayT(int t[]);
    int StrLen(char*); // Get length of a string.
    char* BcdToStr(char); // Convert BCD (binary coded decimal) to decimal.
    char IntToChar(int i);

#endif	/* XC_Utils_H */

