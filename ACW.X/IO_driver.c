#include "IO_driver.h"

#define pin0 RB6 // Use pin RE0.                                      
#define pin1 RB7 // Use pin RE2.

void io_Init() {
    // Default off.
    io_Status = "Init...";
    pin0 = 0x00;
    pin1 = 0x00;
}

void io_TogglePin(uch pinN, char status[]) {
    io_Status = status;
    switch (pinN) {
        case 0x00: pin0 = !pin0;
        case 0x01: pin1 = !pin1;

    }
}

void io_SwitchOff(){
    pin0 = 0x00;
    pin1 = 0x00;
}