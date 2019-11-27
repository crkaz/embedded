// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_Utils_H
#define	XC_Utils_H
#include <xc.h>

// Timings for delay2 args. Cannot be const.
int t503us[2] = {2, 70};
int t430us[2] = {2, 60};
int t70us[2] = {2, 8};
int t63us[2] = {2, 7};
//    int tUKus[2] = { 10, 10 };

void Delay(int t);
void DelayT(int t[]);
int StrLen(char*); // Get length of a string.
char* BcdToStr(char); // Convert BCD (binary coded decimal) to decimal.
char StrToBcd(char str[]);
//    char IntToChar(int i);
int BcdToDec(char bcd);
int IsLeapYear(int yr);
float strFloat(char str[], int length);

#endif	/* XC_Utils_H */

