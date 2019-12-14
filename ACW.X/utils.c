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
    while (a[len]) // Step through chars and iterate counter.
        len++;
    return len;
}

// Convert BCD (binary coded decimal) to string.

char* BcdToStr(char bcd) {
    int tens = ((bcd & 0xF0) >> 4) + toInt; // Get left half of char as int.
    int ones = (bcd & 0x0F) + toInt; // Get right  half of char as int.
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

float StrToFloat(char str[]) {
    float sum = 0.0;
    sum += (str[0x00] + toInt) * 0x0A; // Tens.
    sum += (str[0x01] + toInt); // Units.
    sum += (str[0x03] + toInt) * 0.1; // Decimal.
    return sum;
}

// https://www.tutorialspoint.com/learn_c_by_examples/leap_year_program_in_c.htm
uch IsLeapYear(int yr) {
    yr = yr + 2000;

    if (((yr % 4 == 0) && (yr % 100 != 0)) || (yr % 400 == 0))
        return true; // Is leap year.
    else
        return false; // Is not leap year.
    }

uch BcdToDec(char bcd) {
    //    Multiply most significant bit and add least significant.
    uch dec = ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
    return dec;
}
