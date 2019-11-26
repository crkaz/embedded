#include"EEP_Driver.h"

int EEP_Read(int add) {
   return _EEREG_EEPROM_READ(add);
}

void EEP_Write(int add, int value) {
    _EEREG_EEPROM_WRITE(add, value);
}
