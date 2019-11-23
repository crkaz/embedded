// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_RTC_H
	#define	XC_RTC_H
	
	#include <xc.h> // include processor files - each processor file is guarded.  

    // Args for get/set_time_bit()
    // NB: Write mode by default; +1 to set to read).
    const char SEC = 0x80;
    const char MIN = 0x82;
    const char HOUR = 0x84;
    const char DATE = 0x86;
    const char MONTH = 0x88;
    const char DAY = 0x8A;
    const char YEAR = 0x8C;
    const char CTRL = 0x8E;
    
    // Privatised.
    //    void WriteByte(unsigned char time_tx); //write one byte subroutine.
    //    unsigned char ReadByte(); //read one byte subroutine.
    
    void rtc_PortInit(void); //port initilize subroutine.
    void rtc_Init(void); //DS1302 initilize subroutine.
    void rtc_SetTime(void); //set time subroutine.
    char* rtc_GetTime(void); //get time subroutine.
    char* rtc_GetTimeComponentAsString(char); // Get individual time/date component.
    char* rtc_GetTimeString(void); // Get full time as string.
    char* rtc_GetDateString(void); // Get full date as string.
    void rtc_SetTimeComponent(char, char); // Set individual time component.

#endif	/* XC_RTC_H */