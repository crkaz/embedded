#include"utils.h"
char bcdToStr[0x03];

// Delay for n instruction cycles.

void Delay(int n) {
    for (int i = 0; i < n; i++);
}

// Delay to give a more precise delay.
// EXAMPLE USE: DelayT(t503us); // Where t503us == a const int array.

void DelayT(uch t[]) {
    char x = t[0x00];
    char y = t[0x01];

    char z;
    do {
        z = y;
        do {
            ;
        } while (--z);
    } while (--x);
}

// Get length of a string.

uch StrLen(char a[]) {
    uch len = 0x00;
    while (a[len])
        len++;
    return len;
}

// Convert BCD (binary coded decimal) to string.

char* BcdToStr(char bcd) {
    int tens = ((bcd & 0xF0) >> 4) + 48;
    int ones = (bcd & 0x0F) + 48;
    bcdToStr[0x00] = tens;
    bcdToStr[0x01] = ones;
    bcdToStr[0x02] = eol;

    return bcdToStr;
}

char StrToBcd(char str[]) {
    //The last 4 bits of an ascii char for [0-9] are BCD encoded
    //Use a Mask to remove the first four bits for both
    //Shift the tens to the left 4 and then AND on the units
    char a = str[0x00] + toInt;
    a <<= 4;
    char b = str[0x01] + toInt;
    return a + b;
}
// Convert BCD (binary coded decimal) to decimal.

float StrToFloat(char str[]) {
    float sum = 0.0;
    sum += (str[0x00] + toInt) * 0x0A;
    sum += (str[0x01] + toInt);
    sum += (str[0x03] + toInt) * 0.1;
    return sum;
}

//https://docs.microsoft.com/en-us/office/troubleshoot/excel/determine-a-leap-year

uch IsLeapYear(int yr) {
    yr = yr + 2000;

    if (((yr % 4 == 0) && (yr % 100 != 0)) || (yr % 400 == 0))
        return true;
    else
        return false;
    }

uch BcdToDec(char bcd) {
    //    Multiply most significant bit and add least significant.
    uch dec = ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
    return dec;
}
