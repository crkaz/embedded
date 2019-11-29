#include"EEP_Driver.h"
char eepStr1[0x06];
char eepStr2[0x06];

char* EEP_Read_String(char add, char strN) {
    //   static char eep_str[5];   //Needs to be static or 't'
    char *str;
    if (strN) str = eepStr1;
    else str = eepStr2;

    str[0x00] = EEP_Read_Char(add);
    str[0x01] = EEP_Read_Char(add + 0x01);
    str[0x02] = EEP_Read_Char(add + 0x02);
    str[0x03] = EEP_Read_Char(add + 0x03);
    str[0x04] = EEP_Read_Char(add + 0x04);
    str[0x05] = '\0';

    return str;
}

char EEP_Read_Char(char add) {
    return _EEREG_EEPROM_READ(add);
}

void EEP_Write_String(char add, char str[]) {
    EEP_Write_Char(add, str[0x00]);
    EEP_Write_Char(add + 0x01, str[0x01]);
    EEP_Write_Char(add + 0x02, str[0x02]);
    EEP_Write_Char(add + 0x03, str[0x03]);
    EEP_Write_Char(add + 0x04, str[0x04]);
}

void EEP_Write_Char(char add, char ch) {
    _EEREG_EEPROM_WRITE(add, ch);
}
