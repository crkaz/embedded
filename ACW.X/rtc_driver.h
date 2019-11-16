// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_RTC_H
	#define	XC_RTC_H
	
	#include <xc.h> // include processor files - each processor file is guarded.  
	void port_init(); //port initilize subroutine.
    void rtc_init(); //DS1302 initilize subroutine.
    void set_time(); //set time subroutine.
    void get_time(); //get time subroutine.
    void write_byte(unsigned char time_tx); //write one byte subroutine.
    unsigned char read_byte(); //read one byte subroutine.
    char get_time_bit(char); // Get individual time component.
    void set_time_bit(char, char); // Set individual time component.

    // 1302I pin config.
    #define i_o   RB4 //1302I_O           
    #define sclk  RB0 //1302 clock        
    #define rst   RB5 //1302 enable bit   

#endif	/* XC_RTC_H */