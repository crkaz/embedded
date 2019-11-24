// This is a guard condition so that contents of this file are not included more than once.  
#ifndef XC_Matrix_H
	#define	XC_Matrix_H

	#include <xc.h> // include processor files - each processor file is guarded.  

    // 7 8 9 X
    // 4 5 6 <
    // 1 2 3 >
    // 0 .   S
	void matrix_Init(void);
    char matrix_Scan(void); //key scan function declare
//    int scan(int row); // Privatised.
#endif	/* XC_Matrix_H */
    