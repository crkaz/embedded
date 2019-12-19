// This is a guard condition so that contents of this file are not included more than once.
#ifndef XC_IO_H
#define	XC_IO_H

// DRIVER DESCRIPTION:

// INCLUDES.
#include <xc.h> // include processor files - each processor file is guarded.
#include "utils.h"

// DEFINES.
//#define pin0 RB6 // Use pin RB6.                                      
//#define pin1 RB7 // Use pin RB7.

// PUBLIC VARS.
char *io_Status;

// PUBLIC METHODS.
void io_Init(void); // Initialise component.
void io_TogglePin(uch pinN, char status[]); // Toggle individual pins.
void io_SwitchOff(void); // Switch all pins off.

#endif	/* XC_IO_H */