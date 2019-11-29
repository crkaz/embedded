#include "ui.h"

const int MAX_SCREEN_INDEX = 2; // 0 == default, 1 == settings, 2 == settings...
char changedMode = 0x00;

void ui_DisplayStandby() {
    lcd_PrintString("Date:", 0, 0);
    lcd_PrintString(rtc_GetString(0x01), 0, 3);

    lcd_PrintString("Time:", 1, 0);
    lcd_PrintString(rtc_GetString(0x00), 1, 3);

    lcd_PrintString("Temp:", 2, 0);
    lcd_PrintString(calculate_temp(get_temp()), 2, 3);

    lcd_PrintString("Status:", 3, 0);
    lcd_PrintString("OK", 3, 4);
}

void ui_DisplayMenu(char ln1[], char ln2[], char ln3[], char ln4[]) {
    lcd_PrintString(ln1, 0, 0);
    lcd_PrintString(ln2, 1, 0);
    lcd_PrintString(ln3, 2, 0);
    lcd_PrintString(ln4, 3, 0);
}

// Set date and time.

void ui_DisplaySetDateTime(char isTime) {
    //    lcd_CursorStatus(0x01); // Switch cursor on.

    char seperator = (0x0D * isTime) + 0x2D; // 58':' for time or 45'-' for date.
    char *title = "#Date YY-MM-DD";
    if (isTime)
        title = "#Time";

    lcd_PrintString(title, 0, 0);

    lcd_PrintString("Curr:", 1, 0);
    lcd_PrintString(rtc_GetString(!isTime), 1, 3); // ! because getstring asks for isDate.

    lcd_PrintString("New:", 2, 0);
    lcd_SetCursorPos(2, 3);

    // Wait for user to give valid value or cancel.
    if (isTime)
        /*return */ ui_GetInput(seperator, 0x04); // h m s
    else
        /*return */ ui_GetInput(seperator, 0x0A); // -y- m d
    //    lcd_CursorStatus(0x00);
    lcd_Clear();
}

void ui_DisplaySetDaytime() {
    //    lcd_CursorStatus(0x01); // Switch cursor on.

    char seperator = ':';
    char *title = "#Daytime ";

    char *dayStart = EEP_Read_String(DAY_START_TIME, 0x00);
    char *dayEnd = EEP_Read_String(DAY_END_TIME, 0x01);

    lcd_PrintString(title, 0, 0);

    lcd_PrintString("Cur:", 1, 0);
    lcd_PrintString(dayStart, 1, 2);
    lcd_PrintString(dayEnd, 1, 5);

    lcd_PrintString("New:", 2, 0);
    lcd_SetCursorPos(2, 3);


    // Wait for user to give valid value or cancel.
    /*return */ ui_GetInput(seperator, DAY_START_TIME);
    lcd_Clear();
}

// Set daytime periods and thresholds.

void ui_DisplaySetThresholds(char isNight) {
    //    lcd_CursorStatus(0x01); // Switch cursor on.

    char seperator = '.';
    char *title = "#Thresholds (d)";
    if (isNight)
        title = "#Thresholds (n)";

    char *lowerThresh = EEP_Read_String(DAY_LOWER_THRESH_TEMP + (isNight * 0x20), 0x00);
    char *upperThresh = EEP_Read_String(DAY_UPPER_THRESH_TEMP + (isNight * 0x20), 0x01);

    lcd_PrintString(title, 0, 0);

    lcd_PrintString("Cur:", 1, 0);
    lcd_PrintString(lowerThresh, 1, 2);
    lcd_PrintString(upperThresh, 1, 5);

    lcd_PrintString("New:", 2, 0);
    lcd_SetCursorPos(2, 3);


    // Wait for user to give valid value or cancel.
    if (isNight)
        /*return */ ui_GetInput(seperator, NIGHT_LOWER_THRESH_TEMP);

    else
        /*return */ ui_GetInput(seperator, DAY_LOWER_THRESH_TEMP);
    //    lcd_CursorStatus(0x00);
    lcd_Clear();
}

void ui_Navigate() {
    if (changedMode) {
        Delay(8000); // Don't change screens too fast.
        changedMode = 0;
    }
    char currMode = ui_Mode;
    char input = matrix_Scan();
    switch (input) {
        case 'x':
            if (ui_Mode > MAX_SCREEN_INDEX) ui_Mode /= 10; // Go back.
            else ui_Mode = 0; // Else return home.
            break;
        case '<':
            if (ui_Mode < MAX_SCREEN_INDEX + 1 && ui_Mode != 0) { // Check not in a sub menu.
                ui_Mode -= 1; // Go back a screen.
                //                if (mode == -1) mode = MAX_SCREEN_INDEX; // Cycle to last menu item.
            }
            break;
        case '>':
            if (ui_Mode < MAX_SCREEN_INDEX) { // Check not in a sub menu.
                ui_Mode += 1; // Go forward a screen.
                //                if (mode > MAX_SCREEN_INDEX) mode = 0; // Cycle to first menu item.
            }
            break;
        case '1':
        case '2':
        case '3':
        {
            int i = input - '0'; // Get 1/2/3 as an int.
            if (ui_Mode != 0 && ui_Mode <= MAX_SCREEN_INDEX) ui_Mode = ui_Mode * 10 + i; // Step into sub menu.
            break;
        }
    }
    if (currMode != ui_Mode) { // Check if mode has changed i.e. user has navigated.
        lcd_Clear(); // If loading a new screen, clear previous one.
        changedMode = 1; // Set has-changed flag to trigger delay.
    }
}

void ui_Render() {
    switch (ui_Mode) {
            // Standby screen.
        case 0: ui_DisplayStandby();
            break;

        case 1: ui_DisplayMenu("Settings", "1.Date", "2.Time", "3.Daytime");
            break;

        case 2: ui_DisplayMenu("...", "1.Thresholds (d)", "2.Thresholds (n)", "3.BEPIS");
            break;

        case 11: ui_DisplaySetDateTime(0x00); // Date.
            break;
        case 12: ui_DisplaySetDateTime(0x01); // Time.
            break;
        case 13: ui_DisplaySetDaytime(); // Daytime.
            break;
        case 21: ui_DisplaySetThresholds(0x00); // Thresholds (day).
            break;
        case 22: ui_DisplaySetThresholds(0x01); // Thresholds (night).
            break;
        case 23: lcd_PrintString("NOOG", 0, 0);
            break;
    }
}

int ui_ValidateInput(char inputs[]) {
    int returnVal = 1; // Valid.
    int hrsYrs = ((inputs[0] - '0') * 10) + (inputs[1] - '0');
    int minsMnths = ((inputs[2] - '0') * 10) + (inputs[3] - '0');
    int secDays = ((inputs[4] - '0') * 10) + (inputs[5] - '0');
    int high = ((inputs[3] - '0') * 10) + (inputs[4] - '0');
    int maxDays = 0;

    switch (ui_Mode) {
        case 11: // Date.
            if (minsMnths > 12 || minsMnths == 0 || secDays == 0) returnVal = 0;
                // Check days and leapyear.
            else if (minsMnths == 2) { // Handle feb and leap years.
                if (IsLeapYear(hrsYrs))
                    maxDays = 29;
                else
                    maxDays = 28;
            } else {
                if (minsMnths > 6) minsMnths += 1; // From Aug, pattern changes.
                maxDays = 30 + (minsMnths % 2); // Calc max number of days for month. 
            }
            if (secDays > maxDays) returnVal = 0;
            break;
        case 12: // Time.
            if (hrsYrs > 24 || minsMnths > 60 || secDays > 60) returnVal = 0;
            break;
        case 13: // Daytime.
        case 21: // Thresholds (day).
        case 22: // Thresholds (night).
            if (ui_Mode == 13 && (hrsYrs > 23 || minsMnths > 60)) returnVal = 0;
            if (hrsYrs > high) returnVal = 0; // Start time/low thresh must be lower.
            else if (hrsYrs == high && (inputs[2] - '0' >= inputs[5] - '0')) returnVal = 0; // Check minutes/decimal point.
            break;
    }

    // Validation messages.
    lcd_Clear();
    if (returnVal)
        lcd_PrintString("Success!", 0, 0);
    else
        lcd_PrintString("Try again!", 0, 0);

    // Show validation message for a while.
    Delay(25000);
    Delay(25000);

    return returnVal; // Success. Return to previous menu.
}

char ui_GetInput(char separator, char addr) {
    char busy = 0x01;
    char input;
    char nInputs = 0x00;
    char inputs[6]; // Has to be compile-time constant; don't always need 4.
    char inputsChanged = 0x00;
    char lastChar = 'C';
    if (ui_Mode == 13) lastChar = '0';

    Delay(7000); // Delay initial key press so it isn't carried from menu selection.
    while (busy) { // Wait for user input.
        input = '_';

        while (input == '_') // Make responsive to user input.
            input = matrix_Scan();
        Delay(5500); // Delay key presses.

        switch (input) {
            case 's': // Enter/select.
                if (nInputs == 0x06) { // Only validate when expected n inputs given.
                    // Validate input.
                    if (ui_ValidateInput(inputs)) {
                        // Set values.
                        if (ui_Mode == 21 || ui_Mode == 22 || ui_Mode == 13) { // Set thresholds.
                            // Set lower thresholds (heating). // OR Day start.
                            char lowerOrStart[5] = {inputs[0x00], inputs[0x01], separator, inputs[0x02], lastChar};
                            EEP_Write_String(addr, lowerOrStart); // Set lower thresh.

                            // Set upper thresholds (cooling). // OR Day end.
                            char upperOrEnd[5] = {inputs[0x03], inputs[0x04], separator, inputs[0x05], lastChar};
                            EEP_Write_String(addr + 0x10, upperOrEnd); // Set upper thresh.
                        } else { // mode == 11 || mode == 12 // or 23 which isn't implemented.
                            char writingYear;
                            for (char i = 0x00; i < 0x06; ++i) {
                                char str[2] = {inputs[i], inputs[i + 0x01]};
                                if (ui_Mode == 11 && i == 0x00) // If date mode and first iteration; year is offset more than other time components.
                                    writingYear = 0x01; // True.
                                else
                                    writingYear = 0x00; // False.

                                rtc_SetTimeComponent(((addr + 0x80) - i++) + (writingYear * 0x02), StrToBcd(str)); // Set date/time.
                            }
                        }
                        input = 'x'; // Set input to 'x' to return to menu.
                    } else
                        return 1; // Try again.
                }

                break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            {
                // Record user input.
                if (nInputs < 0x06) {
                    inputs[nInputs++] = input; // Store for validation.
                    inputsChanged = 1;

                    // Optional UX.
                    if (nInputs == 0x06)
                        lcd_PrintString("...", 3, 0); // Prompt user to continue.
                    else
                        lcd_PrintString(lcd_EMPTY_STRING, 3, 0); // Clear line 2.
                }
                break;
            }
            case 'b': // Backspace.
                if (nInputs > 0) {
                    inputs[nInputs--] = ' '; // Clear last input.
                    inputsChanged = 1;

                    // Optional UX.
                    lcd_PrintString(lcd_EMPTY_STRING, 3, 0); // Clear line 2.
                }
                break;
        }

        if (input == 'x') { // Cancel/back.
            busy = 0; // Break input loop.
            ui_Mode /= 10; // Return to previous screen.
        }

        if (inputsChanged) {
            lcd_PrintString(lcd_EMPTY_STRING, 2, 0); // Clear line 2.
            lcd_PrintString("New:", 2, 0);
            lcd_SetCursorPos(2, 2);

            char mod = 0x02;
            for (unsigned char i = 0; i < nInputs; ++i) {
                if (i > 0 && !(i % mod)) {// Auto-insert separator char.
                    lcd_PrintChar(separator);
                    lcd_PrintChar(inputs[i]);

                    if (ui_Mode == 21 || ui_Mode == 22 || ui_Mode == 13) { // Add "C " when setting low/high threshold and '0' when setting daytime.
                        lcd_PrintChar(lastChar);
                        lcd_PrintChar(' ');
                        mod = 0x05; // Adjust modulus to position decimal point for upper thresh.
                    }
                } else
                    lcd_PrintChar(inputs[i]);
            }

            inputsChanged = 0;
        }
    }
    return 0; // Finish.
}
