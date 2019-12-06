#include "buzzer_driver.h"

#define buz RE1 // Use pin RE1.                                      
#define buzOn() buz = 1 // Set RE1 high.
#define buzOff() buz = 0; // Set RE1 low.

void buzzer_init() {
    TRISE1 = 0; // Using pin E1 as output.                                        
    buzOff(); // Don't sound at the start.
}

// Sound the buzzer. 

void buzzer_sound(int bT, int pT, uch reps) {
    // bT = buzzer time (lower == higher pitch).
    // pT = pause time. Use for breaks in continuous buzzing (e.g. alarm).
    // reps = repetitions. Use to repeat tone (e.g. alarm).
    for (uch i = 0x00; i < reps; ++i) {
        buzOn();
        for (int j = 0; j < bT; ++j) {

        }
        buzOff();
        for (int j = 0; j < pT; ++j) {

        }
    }
}