#include"EEP_Driver.h"

void eep_WriteChar(char add, char ch);
char eep_ReadChar(char add);

// Storage for up to 2 strings read from eeprom.
char eepStr1[0x06];
char eepStr2[0x06];

char* eep_ReadString(char addr, char strN) {
    char *str;
    if (strN) str = eepStr1; // If strN == 1, store in first array.
    else str = eepStr2; // If strN == 2, store in second array.

    str[0x00] = eep_ReadChar(addr);
    str[0x01] = eep_ReadChar(addr + 0x01);
    str[0x02] = eep_ReadChar(addr + 0x02);
    str[0x03] = eep_ReadChar(addr + 0x03);
    str[0x04] = eep_ReadChar(addr + 0x04);
    str[0x05] = eol;

    return str;
}

char eep_ReadChar(char addr) {
    return _EEREG_EEPROM_READ(addr);
}

void eep_WriteString(char addr, char str[]) {
    eep_WriteChar(addr, str[0x00]);
    eep_WriteChar(addr + 0x01, str[0x01]);
    eep_WriteChar(addr + 0x02, str[0x02]);
    eep_WriteChar(addr + 0x03, str[0x03]);
    eep_WriteChar(addr + 0x04, str[0x04]);
}

void eep_WriteChar(char addr, char ch) {
    _EEREG_EEPROM_WRITE(addr, ch);
}
