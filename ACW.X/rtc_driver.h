// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_RTC_H
#define	XC_RTC_H
#include <xc.h> // include processor files - each processor file is guarded.  
#include "Utils.h"

char rtc_Vals[8]; //define the read time and date save table.
char rtc_StrVals[9]; //define the read time and date save table.
// DS1302 pin config.
#define i_o   RB4 //1302I_O           
#define sclk  RB0 //1302 clock        
#define rst   RB5 //1302 enable bit   

// Privatised.
//    void WriteByte(unsigned char time_tx); //write one byte subroutine.
//    unsigned char ReadByte(); //read one byte subroutine.

void rtc_PortInit(void); //port initilize subroutine.
void rtc_Init(void); //DS1302 initilize subroutine.
void rtc_SetTime(void); //set time subroutine.
//    char* rtc_GetTime(void); //get time subroutine.
//char rtc_GetTimeComponent(char); // Get individual time/date component.
//    char* rtc_GetTimeComponentAsString(char); // Get individual time/date component.
//    char* rtc_GetTimeString(void); // Get full time as string.
//    char* rtc_GetDateString(void); // Get full date as string.
void rtc_SetTimeComponent(char, char); // Set individual time component.
char *rtc_GetString(char isTime);
void rtc_Update(void);

#endif	/* XC_RTC_H */