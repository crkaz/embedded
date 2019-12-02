// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_Thermometer_H
#define	XC_Thermometer_H

// DRIVER DESCRIPTION:

// INCLUDES.
#include <xc.h> // include processor files - each processor file is guarded.  
#include "utils.h"

// DEFINES.

// PUBLIC VARS.

// PUBLIC METHODS.
char* therm_GetTemp(void);

// PRIVATE METHODS.
//int therm_ReadTemp(void); // Read the temperature from the thermistor and return as int.
//void therm_Reset(void); // Reset the component.
//void therm_WriteByte(uch val); // Write a command to the component.
//uch therm_ReadByte(); // Read from the component.


#endif	/* XC_Thermometer_H */

