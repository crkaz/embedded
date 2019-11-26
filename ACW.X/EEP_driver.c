#include"EEP_Driver.h"

float EEP_Read(char add) {
   return _EEREG_EEPROM_READ(add);
}

void EEP_Write(char add, float value) {
    _EEREG_EEPROM_WRITE(add, value);
}
