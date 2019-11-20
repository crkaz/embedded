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

// Get length of a string.
int strlen(char a[]) {
    int len = 0;
    while (a[len])
        len++;
    return len;
}

// Convert BCD (binary coded decimal) to decimal.

char* convert_bcd_to_string(char bcd) {
    // Multiply most significant bit and add least significant.
    //    int dec = ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
    //    return dec;

    int tens = ((bcd & 0xF0) >> 4) + 48;
    int ones = (bcd & 0x0F) + 48;
    char num[2] = {tens, ones};

    return num;
}