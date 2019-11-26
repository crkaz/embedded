// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_EEP_H
	#define	XC_EEP_H

	#include <xc.h> // include processor files - each processor file is guarded.  
    int EEP_Read(int add);
    void EEP_Write(int add, int value);
#endif	/* XC_EEP_H */
    