// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_EEP_H
#define	XC_EEP_H

// DRIVER DESCRIPTION:

// INCLUDES.
#include <xc.h> // include processor files - each processor file is guarded.  
#include "Utils.h"

// DEFINES.

// PUBLIC VARS.

// PUBLIC METHODS.
char* eep_ReadString(char addr, char strN); // Read a series of 5 chars from EEPROM into a target string.
void eep_WriteString(char addr, char str[]); // Write a series of 5 chars into EEPROM at the address.

// PRIVATE METHODS.
//void eep_WriteChar(char addr, char ch); // Write a char to an address in EEPROM.
//char eep_ReadChar(char addr); // Read a char from an address in EEPROM.

#endif	/* XC_EEP_H */
