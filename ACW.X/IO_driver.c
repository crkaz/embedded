#include "IO_driver.h"
// Buzzer pin config.
#define heater RB6 // Use pin RE0.                                      
#define cooler RB7 // Use pin RE2.

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