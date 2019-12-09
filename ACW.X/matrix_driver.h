// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_Matrix_H
#define	XC_Matrix_H

// DRIVER DESCRIPTION:
//--Button layout:
//-- 7 8 9 X (where X == cancel/back)
//-- 4 5 6 < (where < == up/previous)
//-- 1 2 3 > (where > == down/next)
//-- 0 . b S (where . == decimal point, b == backspace, and S == select/enter)

// INCLUDES.
#include <xc.h> // include processor files - each processor file is guarded.  
#include "utils.h"

// PRIVATE.
// uch result; // Stores key press location from matrix_Scan.

// uch matrix_Scan(uch row); // Get key press location.


// PUBLIC.
void matrix_Init(void); // Initialise ports and pins.
char matrix_GetInput(void); // Scan the 4x4 matrix for user interaction and return input.

#endif
