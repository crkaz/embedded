// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_Thermometer_H
	#define	XC_Thermometer_H

	#include <xc.h> // include processor files - each processor file is guarded.  
    #include "Utils.h"

    #define dq RE0
    #define dq_dir TRISE0
    #define set_dq_high() dq_dir = 1
    #define set_dq_low() dq = 0; dq_dir = 0
//	void write_byte(unsigned char val); // Privatised.
//	unsigned char read_byte(); // Privatised.
//	void resetThermometer(void); // Privatised.

	int get_temp(void);
	char* calculate_temp(int TZ);

#endif	/* XC_Thermometer_H */

