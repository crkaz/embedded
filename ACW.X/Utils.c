#include <xc.h>

void delay(int t) {
	for (int i = 0; i < t; i++);
}

void delay2(int t[]) {
	char x = t[0];
	char y = t[1];

	char z;
	do {
		z = y;
		do {
			;
		} while (--z);
	} while (--x);
}