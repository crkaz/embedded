#include <xc.h>




unsigned char table[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90 }; // 7seg lcd nums 0-9

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

int strlen(char a[]) {
	int len = 0;
	while (a[len])
		len++;
	return len;
}