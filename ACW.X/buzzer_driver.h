#ifndef XC_BUZZER_H
#define	XC_BUZZER_H

// DRIVER DESCRIPTION:
// Interface with the buzzer component.

// INCLUDES.
#include <xc.h> // include processor files - each processor file is guarded.  
#include "utils.h"

// PRIVATE.
// #define buz RE1 // Use pin RE1.                                      
// #define buzOn() buz = 1 // Set RE1 high.
// #define buzOff() buz = 0; // Set RE1 low.

// PUBLIC.
void buzzer_init(void); // Initialise ports and pins.
void buzzer_sound(int bT, int pT, uch reps); // Sound the buzzer for a duration of bT, reps times, with a pause of pT between each rep.

#endif