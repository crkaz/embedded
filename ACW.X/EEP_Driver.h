// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_EEP_H
	#define	XC_EEP_H

	#include <xc.h> // include processor files - each processor file is guarded.  
    char EEP_Read_Char(char add);
    char* EEP_Read_String(char add);
    
    void EEP_Write_String(char add,char str[]);
    void EEP_Write_Char(char add, char ch);
    
#endif	/* XC_EEP_H */
    