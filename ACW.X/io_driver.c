#include "io_driver.h"

#define pin0 RB6                                      
#define pin1 RB7

void io_Init() {
    // Set pins to off by default.
    io_Status = "Init...";
    pin0 = 0x00; // Off.
    pin1 = 0x00; // Off.
}

void io_TogglePin(uch pinN, char status[]) {
    io_Status = status; // Update status string.
    
    switch (pinN) { // Toggle selected pin.
        case 0x00: pin0 = !pin0; // boolean NOT to flip.
            break;
        case 0x01: pin1 = !pin1;
            break;
    }
}

void io_SwitchOff() {
    pin0 = 0x00; // Off.
    pin1 = 0x00; // Off.
}