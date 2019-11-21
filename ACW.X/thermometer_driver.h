// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_Thermometer_H
	#define	XC_Thermometer_H

	#include <xc.h> // include processor files - each processor file is guarded.  
//	void write_byte(unsigned char val); // Privatised.
//	unsigned char read_byte(); // Privatised.
//	void resetThermometer(void); // Privatised.

	int get_temp();
	char* calculate_temp(int TZ);
#endif	/* XC_Thermometer_H */

