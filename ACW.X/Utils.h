// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_Utils_H
#define	XC_Utils_H
#include <xc.h>

// Timings for delay2 args. Cannot be const.
char t503us[2] = {0x02, 0x46};
char t430us[2] = {0x02, 0x3C};
char t70us[2] = {0x02, 0x08};
char t63us[2] = {0x02, 0x07};
//    int tUKus[2] = { 10, 10 };

void Delay(int t);
void DelayT(char t[]);
int StrLen(char*); // Get length of a string.
char* BcdToStr(char); // Convert BCD (binary coded decimal) to decimal.
char StrToBcd(char str[]);
//    char IntToChar(int i);
int BcdToDec(char bcd);
int IsLeapYear(int yr);
float strFloat(char str[]);

#endif	/* XC_Utils_H */

