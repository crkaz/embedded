#include"EEP_Driver.h"
char eepStr1[6];
char eepStr2[6];

char* EEP_Read_String(char add, char strN) {
    //   static char eep_str[5];   //Needs to be static or 't'
    char *str;
    if (strN) str = eepStr1;
    else str = eepStr2;

    str[0] = EEP_Read_Char(add);
    str[1] = EEP_Read_Char(add + 1);
    str[2] = EEP_Read_Char(add + 2);
    str[3] = EEP_Read_Char(add + 3);
    str[4] = EEP_Read_Char(add + 4);
    str[5] = '\0';

    return str;
}

char EEP_Read_Char(char add) {
    return _EEREG_EEPROM_READ(add);
}

void EEP_Write_String(char add, char str[]) {
    EEP_Write_Char(add, str[0]);
    EEP_Write_Char(add + 1, str[1]);
    EEP_Write_Char(add + 2, str[2]);
    EEP_Write_Char(add + 3, str[3]);
    EEP_Write_Char(add + 4, str[4]);
}

void EEP_Write_Char(char add, char ch) {
    _EEREG_EEPROM_WRITE(add, ch);
}
