#include "Arduino.h"
#include "AlarmModule.h"

/**
 * Default constructor
 */
AlarmModule::AlarmModule() {
    // pinmodes
    pinMode(timeSetPin,  INPUT);
    pinMode(alarmSetPin, INPUT);

    // create LCD object
    lcd = new LCD(0x27);

    // set default time to 12:00:00AM
    time.hour   = 0;
    time.minute = 0;
    time.second = 0;

    // no last iter, so set to zero
    lastIter = 0;
}

/**
 * Helper method. Makes the displayTime() function look
 * pretty;
 * @param       i - integer to be converted to char
 * @return      char representation of integer
 */
char intToChar(int i) {
    return (char)(48 + i);
}

/**
 * Displays the current time on the LCD screen
 */
void AlarmModule::displayTime() {
    // create null terminated char[] (basically a string)
    char tstr[9] = {' ', ' ', ':' ,' ' ,' ' ,':' ,' ' ,' ', '\0'};

    // format hour by splitting and using ASCII values to convert int to char
    if (time.hour < 10)
        tstr[0] = '0';
    else
        tstr[0] = intToChar(time.hour / 10);
    tstr[1] = intToChar(time.hour % 10);

    // format minute in the same way hour was formatted
    if (time.minute < 10)
        tstr[3] = '0';
    else
        tstr[3] = intToChar(time.minute / 10);
    tstr[4] = intToChar(time.minute % 10);

    // format second in the same way minute was formatted
    if (time.second < 10)
        tstr[6] = '0';
    else
        tstr[6] = intToChar(time.second / 10);
    tstr[7] = intToChar(time.second % 10);
    
    // write converted char[] to lcd
    lcd->write(tstr);
}

/**
 * Checks if the time should be updated
 * @return          true if can be updated, false otherwise
 */
bool AlarmModule::canUpdate() {
    return millis() - lastIter >= 1000;
}


/**
 * Adds one second to the second counter, then updates
 * hour and minute counter accordingly.
 */
void AlarmModule::iterate() {
    if (canUpdate()) {
        time.second++;

        // update second, minute, and hour
        if (time.second == 60) {
            time.second = 0;
            time.minute++;
        }
        if (time.minute == 60) {
            time.minute = 0;
            time.hour++;
        }
        if (time.hour == 24) {
            time.hour   = 0;
            time.minute = 0;
            time.second = 0;
        }

        // update last iteration timer
        lastIter = millis();

        // display new time
        displayTime();
    }
}

/**
 * Time's accessor
 * @return          time - currently calculated time
 */
Time AlarmModule::getTime() {
    return time;
}

/**
 * Time's mutator
 * @param           time - new time to store
 */
void AlarmModule::setTime(Time time) {
    if (time.hour <= 23 && time.minute < 60 && time.second < 60) {
        this->time.hour = time.hour;
        this->time.minute = time.minute;
        this->time.second = time.second;
    }
}

/**
 * Time's mutator
 * @param           hour - new hour to store
 * @param           minute - new minute to store
 * @param           second - new second to store
 */
void AlarmModule::setTime(int hour, int minute, int second) {
    this->time.hour = time.hour;
    this->time.minute = time.minute;
    this->time.second = time.second;
}
