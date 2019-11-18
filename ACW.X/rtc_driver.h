// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_RTC_H
	#define	XC_RTC_H
	
	#include <xc.h> // include processor files - each processor file is guarded.  
    const char SEC = 0x80;
    const char MIN = 0x82;
    const char HOUR = 0x84;
    const char DATE = 0x86;
    const char MONTH = 0x88;
    const char DAY = 0x8A;
    const char YEAR = 0x8C;
    const char CTRL = 0x8E;
    
    void rtc_port_init(); //port initilize subroutine.
    void rtc_init(); //DS1302 initilize subroutine.
    void set_time(); //set time subroutine.
    void get_time(); //get time subroutine.
    void write_byte(unsigned char time_tx); //write one byte subroutine.
    unsigned char read_byte(); //read one byte subroutine.
    char* get_time_bit_as_string(char); // Get individual time component.
    char* get_time_as_string(); // Get full time as string.
    void set_time_bit(char, char); // Set individual time component.

#endif	/* XC_RTC_H */