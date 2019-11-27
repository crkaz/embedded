#include <xc.h>

// Buzzer pin config.
#define heater RE0 // Use pin RE0.                                      
#define cooler RE2 // Use pin RE2.

void heaterOn() {
    cooler = 0;
    heater = 1;
}

void coolerOn() {
    heater = 0;
    cooler = 1;
}

void systemsOff() {
    heater = 0;
    cooler = 0;
}
