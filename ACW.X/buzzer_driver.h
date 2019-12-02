// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_BUZZER_H
#define	XC_BUZZER_H

// DRIVER DESCRIPTION:

// INCLUDES.
#include <xc.h> // include processor files - each processor file is guarded.  
#include "Utils.h"

// DEFINES.

// PUBLIC VARS.

// PUBLIC METHODS.
void buzzer_init(void);
void buzzer_sound(int bT, int pT, int reps);

// PRIVATE METHODS.

#endif	/* XC_HEADER_TEMPLATE_H */