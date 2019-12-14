#include "ui.h"

void ui_DisplayStandby(void); // Renders the standby screen.
void ui_DisplayMenu(char ln1[], char ln2[], char ln3[], char ln4[]); // Renders a title and 3 menu options.
void ui_DisplaySetDateTime(char isDate); // Renders the screen to set time.
void ui_DisplaySetDaytime(void); // Renders the screen to set the daytime start and finish.
void ui_DisplaySetThresholds(char isNight); // Renders the screen to set the lower (heating) and upper (cooling) temperature thresholds.
uch ui_ValidateInput(char inputs[]); // Validates a user input based on the current UI state.
char ui_GetInput(char seperator, char addr); // Gets and formats input from the user, and sets the use the values to update the system.

const uch MAX_SCREEN_INDEX = 0x02; // 0 == default, 1 == settings, 2 == settings...
char changedMode = false; // Flag for when mode changes between renders.

void ui_DisplayStandby() {
    lcd_PrintString("Da:", 0x00, 0x00);
    lcd_PrintString(rtc_GetString(0x01), 0x00, 0x02); // Current date.

    lcd_PrintString("Ti:", 0x01, 0x00);
    lcd_PrintString(rtc_GetString(0x00), 0x01, 0x02); // Current time.

    lcd_PrintString("Te:", 0x02, 0x00);
    lcd_PrintString(therm_GetTemp(), 0x02, 0x02); // Temperature.

    lcd_PrintString("St:", 0x03, 0x00);
    lcd_PrintString(io_Status, 0x03, 0x02); // System status.
}

void ui_DisplayMenu(char ln1[], char ln2[], char ln3[], char ln4[]) {
    lcd_PrintString(ln1, 0x00, 0x00);
    lcd_PrintString(ln2, 0x01, 0x00);
    lcd_PrintString(ln3, 0x02, 0x00);
    lcd_PrintString(ln4, 0x03, 0x00);
}

void ui_DisplaySetDateTime(char isTime) {
    char seperator = (0x0D * isTime) + 0x2D; // 58':' for time or 45'-' for date.
    char *title = "Date";
    if (isTime)
        title = "Time";

    lcd_PrintString(title, 0x00, 0x00);

    lcd_PrintString("Cur:", 0x01, 0x00);
    lcd_PrintString(rtc_GetString(!isTime), 0x01, 0x02); // Get current time from clock.

    lcd_PrintString("New:", 0x02, 0x00);
    lcd_SetCursorPos(0x02, 0x02); // Set cursor for user input.

    // Wait for user to give valid value or cancel.
    if (isTime)
        ui_GetInput(seperator, 0x04); // h m s
    else
        ui_GetInput(seperator, 0x0A); // -y- m d

    lcd_Clear(); // Returning to settings screen, clear outputs.
}

void ui_DisplaySetDaytime() {
    char seperator = ':';
    char *title = "Daytime";

    // Read daytime start and end from eeprom.
    char *dayStart = eep_ReadString(DAY_START_TIME, 0x00);
    char *dayEnd = eep_ReadString(DAY_END_TIME, 0x01);

    lcd_PrintString(title, 0, 0);

    lcd_PrintString("Cur:", 1, 0);
    lcd_PrintString(dayStart, 1, 2); // Print current day-start value.
    lcd_PrintString(dayEnd, 1, 5); // Print current day-end value

    lcd_PrintString("New:", 2, 0);
    lcd_SetCursorPos(2, 3); // Set cursor for user input.

    // Wait for user to give valid value or cancel.
    ui_GetInput(seperator, DAY_START_TIME);
    lcd_Clear(); // Returning to settings screen, clear outputs.
}

void ui_DisplaySetThresholds(char isNight) {
    char seperator = '.';
    char *title = "Thresholds (d)";
    if (isNight)
        title = "Thresholds (n)";

    // Get upper and lower thresholds from eeprom.
    char *lowerThresh = eep_ReadString(DAY_LOWER_THRESH_TEMP + (isNight * 0x20), 0x00);
    char *upperThresh = eep_ReadString(DAY_UPPER_THRESH_TEMP + (isNight * 0x20), 0x01);

    lcd_PrintString(title, 0, 0);

    lcd_PrintString("Cur:", 1, 0);
    lcd_PrintString(lowerThresh, 1, 2); // Lower temperature threshold.
    lcd_PrintString(upperThresh, 1, 5); // Upper temperature threshold.

    lcd_PrintString("New:", 2, 0);
    lcd_SetCursorPos(2, 3); // Set cursor for user input.

    // Wait for user to give valid value or cancel.
    if (isNight)
        ui_GetInput(seperator, NIGHT_LOWER_THRESH_TEMP);
    else
        ui_GetInput(seperator, DAY_LOWER_THRESH_TEMP);

    lcd_Clear(); // Returning to settings screen; clear all outputs.
}

void ui_Navigate() {
    if (changedMode) {
        Delay(8000); // Don't change screens too fast.
        changedMode = false; // Reset flag.
    }

    char currMode = ui_Mode;
    char input = matrix_GetInput(); // Get user input.

    switch (input) {
        case 'x':
            if (ui_Mode > MAX_SCREEN_INDEX) ui_Mode /= 10; // Go back.
            else ui_Mode = 0; // Else return home.
            break;
        case '<':
            if (ui_Mode < MAX_SCREEN_INDEX + 1 && ui_Mode != 0) { // Check not in a sub menu.
                ui_Mode--; // Go back a screen.
                //                if (mode == -1) mode = MAX_SCREEN_INDEX; // Cycle to last menu item.
            }
            break;
        case '>':
            if (ui_Mode < MAX_SCREEN_INDEX) { // Check not in a sub menu.
                ui_Mode++; // Go forward a screen.
                //                if (mode > MAX_SCREEN_INDEX) mode = 0; // Cycle to first menu item.
            }
            break;
        case '1':
        case '2':
        case '3':
        {
            int i = input + toInt; // Get 1/2/3 as an int.
            if (ui_Mode != 0 && ui_Mode <= MAX_SCREEN_INDEX) ui_Mode = ui_Mode * 10 + i; // Step into sub menu.
            break;
        }
    }
    if (currMode != ui_Mode) { // Check if mode has changed i.e. user has navigated.
        lcd_Clear(); // If loading a new screen, clear previous one.
        changedMode = true; // Set has-changed flag to trigger delay.
    }
}

void ui_Render() {
    switch (ui_Mode) {
        case 0: ui_DisplayStandby();
            break;

            // Settings screen 1.
        case 1: ui_DisplayMenu("Settings", "1.Date", "2.Time", "...");
            break;
        case 11: ui_DisplaySetDateTime(0x00); // Date.
            break;
        case 12: ui_DisplaySetDateTime(0x01); // Time.
            break;
        case 13: ui_Mode = 2;
            break;

            // Settings screen 2.
        case 2: ui_DisplayMenu("...", "1.Daytime", "2.Thresholds (d)", "3.Thresholds (n)");
            break;
        case 21: ui_DisplaySetDaytime(); // Daytime.
            break;
        case 22: ui_DisplaySetThresholds(0x00); // Thresholds (day). 
            break;
        case 23: ui_DisplaySetThresholds(0x01); // Thresholds (night).
            break;
    }
}

uch ui_ValidateInput(char inputs[]) {
    uch returnVal = true; // Valid.
    // Get user inputs in date/time format (3 inputs of 2 chars e.g YY MM DD / HH MM SS).
    int hrsYrs = ((inputs[0x00] + toInt) * 0x0A) + (inputs[0x01] + toInt); // 1st 2 chars as int
    int minsMnths = ((inputs[0x02] + toInt) * 0x0A) + (inputs[0x03] + toInt); // 2nd 2 chars as int
    int secDays = ((inputs[0x04] + toInt) * 0x0A) + (inputs[0x05] + toInt); // 3rs 2 chars as int

    // Get user inputs in daytime/threshold format (2 inputs of 3 chars e.g 00 0 / HH M).
    int nightHrs = ((inputs[0x03] + toInt) * 0x0A) + (inputs[0x04] + toInt); // 4th/5th inputs as int
    int nightMins = ((inputs[0x05] + toInt) * 0x0A); //  last input as int.

    int maxDays = 0; // Variable for max days in month.

    // Change validation method based on ui mode (e.g. date or time).
    switch (ui_Mode) {
        case 11: // Date.
            if (minsMnths > 0x0C || minsMnths == 0x00 || secDays == 0x00) returnVal = false;
                // Check days in month is valid (including leapyear).
            else if (minsMnths == 0x02) { // Handle feb and leap years.
                if (IsLeapYear(hrsYrs))
                    maxDays = 0x1D; // 29.
                else
                    maxDays = 0x1C; // 28.
            } else {
                if (minsMnths > 0x06) minsMnths++; // From Aug, pattern changes.
                maxDays = 0x1E + (minsMnths % 2); // Calc max number of days for month as 30 or 3.
            }
            if (secDays > maxDays) returnVal = false; // Days out of range.
            break;

        case 12: // Time.
            // Hours can't be > 23, mins > 59, secs > 59. 
            if (hrsYrs > 0x17 || minsMnths > 0x3B || secDays > 0x3B) returnVal = false; // 0x17 == 23, 0x3B = 59
            break;

        case 21: // Daytime.
        case 22: // Thresholds (day).
        case 23: // Thresholds (night).
            // If setting daytime and times out of range.
            if (ui_Mode == 21 && (hrsYrs > 0x17 || minsMnths > 0x3B || nightHrs > 0x17 || nightMins > 0x3B)) returnVal = false;
             // Start time/low thresh must be lower.
            if (hrsYrs > nightHrs) returnVal = false;
            // Check minutes/decimal point. Thresholds must be at least .2 apart.
            else if (hrsYrs == nightHrs && ((inputs[0x02] + toInt >= inputs[0x05] + toInt) || (ui_Mode != 21 && inputs[0x02] + toInt + 0x02 > inputs[0x05] + toInt)))
                returnVal = false;
            break;
    }

    // Validation messages.
    lcd_Clear();
    if (returnVal)
        lcd_PrintString("Success!", 0x00, 0x00);
    else
        lcd_PrintString("Invalid!", 0x00, 0x00);

    // Show validation message for a while.
    Delay(25000);
    Delay(25000);

    return returnVal; // Success. Return to previous menu.
}

char ui_GetInput(char separator, char addr) {
    char busy = true; // Flag.
    char input; 
    char nInputs = 0x00; // Current number of inputs.
    char inputs[0x06]; // Storage for 6 input chars.
    char inputsChanged = false; // Flag.
    char lastChar = 'C'; // Symbol for temperature thresholds.
    if (ui_Mode == 21) lastChar = '0'; // Allows reuse of set thresholds logic for setting daytime.

    Delay(7000); // Delay initial key press so it isn't carried from menu selection.
    while (busy) { // Wait for user input.
        input = '_';

        while (input == '_') { // Make responsive to user input.
            input = matrix_GetInput();
            // If setting the time, update the time whilst setting.
            if (ui_Mode == 12) {
                rtc_Update(); // Update the current time value.
                lcd_PrintString(rtc_GetString(0x00), 0x01, 0x02); // Print current time to 2nd line.
            }
        }
        Delay(6000); // Delay key presses to prevent ghosting.

        switch (input) {
            case 's': // Enter/select.
                if (nInputs == 0x06) { // Only validate when expected n inputs given.
                    // Validate input.
                    if (ui_ValidateInput(inputs)) {
                        // Set values.
                        if (ui_Mode == 21 || ui_Mode == 22 || ui_Mode == 23) { // Set datetime(21) and thresholds (22, 23).
                            // Set lower thresholds (heating). // OR Day start.
                            char lowerOrStart[0x05] = {inputs[0x00], inputs[0x01], separator, inputs[0x02], lastChar};
                            eep_WriteString(addr, lowerOrStart); // Set lower thresh.

                            // Set upper thresholds (cooling). // OR Day end.
                            char upperOrEnd[0x05] = {inputs[0x03], inputs[0x04], separator, inputs[0x05], lastChar};
                            eep_WriteString(addr + 0x10, upperOrEnd); // Set upper thresh.
                        } else { // mode == 11 (set date) || mode == 12 (set time) || mode == 13 (set day) -- not implemented)
                            char writingYear;
                            for (uch i = 0x00; i < 0x06; ++i) {
                                char str[0x02] = {inputs[i], inputs[i + 0x01]};
                                if (ui_Mode == 11 && i == 0x00) // If date mode and first iteration; year is offset more than other time components.
                                    writingYear = true;
                                else
                                    writingYear = false;

                                rtc_SetTimeComponent(((addr + 0x80) - i++) + (writingYear * 0x02), StrToBcd(str)); // Set date/time.
                            }
                        }
                        input = 'x'; // Set input to 'x' to return to menu.
                    } else
                        return true; // Try again.
                }

                break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            {
                // Record user input.
                if (nInputs < 0x06) {
                    inputs[nInputs++] = input; // Store for validation.
                    inputsChanged = true;

                    // Optional UX.
                    if (nInputs == 0x06)
                        lcd_PrintString("...", 0x03, 0x00); // Prompt user to continue.
                    else
                        lcd_PrintString(lcd_EMPTY_STRING, 0x03, 0x00); // Clear line 2.
                }
                break;
            }
            case 'b': // Backspace.
                if (nInputs > 0x00) {
                    inputs[nInputs--] = ' '; // Clear last input.
                    inputsChanged = true;

                    // Optional UX.
                    lcd_PrintString(lcd_EMPTY_STRING, 0x03, 0x00); // Clear line 2.
                }
                break;
        }

        if (input == 'x') { // Cancel/back.
            busy = false; // Break input loop.
            ui_Mode /= 10; // Return to previous screen.
        }

        if (inputsChanged) {
            lcd_PrintString(lcd_EMPTY_STRING, 0x02, 0x00); // Clear line 2.
            lcd_PrintString("New:", 0x02, 0x00);
            lcd_SetCursorPos(0x02, 0x02);

            char mod = 0x02; // Modulus of where to insert separator.
            for (uch i = 0x00; i < nInputs; ++i) {
                if (i > 0x00 && !(i % mod)) {// Auto-insert separator char.
                    lcd_PrintChar(separator);
                    lcd_PrintChar(inputs[i]);

                    if (ui_Mode == 21 || ui_Mode == 22 || ui_Mode == 23) { // Add "C " when setting low/high threshold and '0' when setting daytime.
                        lcd_PrintChar(lastChar);
                        lcd_PrintChar(' ');
                        mod = 0x05; // Adjust modulus to position decimal point for upper thresh.
                    }
                } else
                    lcd_PrintChar(inputs[i]);
            }

            inputsChanged = false;
        }
    }
    return false; // Finish.
}