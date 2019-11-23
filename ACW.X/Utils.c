#include <xc.h>

unsigned char table[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90 }; // 7seg lcd nums 0-9
char bcdToStr[3];

// Delay for n instruction cycles.
void Delay(int n) {
    for (int i = 0; i < n; i++);
}

// Delay to give a more precise delay.
// EXAMPLE USE: DelayT(t503us); // Where t503us == a const int array.
void DelayT(int t[]) {
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
int StrLen(char a[]) {
    int len = 0;
    while (a[len])
        len++;
    return len;
}

// Convert BCD (binary coded decimal) to string.
char* BcdToStr(char bcd) {
    int tens = ((bcd & 0xF0) >> 4) + 48;
    int ones = (bcd & 0x0F) + 48;
    bcdToStr[0] =  tens;
    bcdToStr[1] = ones;
    bcdToStr[2] = '\0';

    
    return bcdToStr;
}

//// Convert an int to a char.
//char IntToChar(int i) {
//    char c = i + 48;
//    return c;
//}

// Convert BCD (binary coded decimal) to decimal.
//char* BcdToDec(char bcd) {
////    Multiply most significant bit and add least significant.
//            int dec = ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
//    return dec;
//}
