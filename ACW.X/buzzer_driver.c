#include "buzzer_driver.h"
       
// Buzzer pin config.
#define buz RE1 // Use pin RE1.                                      
#define buz_on() buz = 1 // Set RE1 high.
#define buz_off() buz = 0; // Set RE1 low.

void buzzer_init() {
    TRISE1 = 0; // Using pin E1 as output.                                        
    buz_off(); // Don't sound at the start.
}

// Sound the buzzer. 
void buzzer_sound(int bT, int pT, int reps) {
    // bT = buzzer time (lower == higher pitch).
    // pT = pause time. Use for breaks in continuous buzzing (e.g. alarm).
    // reps = repetitions. Use to repeat tone (e.g. alarm).
    
    for (unsigned char i = 0; i < reps; ++i) {
        buz_on();
        for (int j = 0; j < bT; ++j)  {
           
        }
        buz_off();
        for (int j = 0; j < pT; ++j) {
            
        }
    }
}