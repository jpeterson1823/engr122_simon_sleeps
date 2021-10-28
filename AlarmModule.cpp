#include <Arduino.h>
#include "AlarmModule.h"

/* Empty deconstructor */
AlarmModule::~AlarmModule() { /* empty */ }

/**
 * Default constructor
 */
AlarmModule::AlarmModule() {
    // pinmodes
    pinMode(timeSetPin,  INPUT);
    pinMode(alarmSetPin, INPUT);
    pinMode(ledControl, OUTPUT);

    // create LCD object
    lcd = new LCD(0x27);

    rf = new RFHandler();

    // set default time to 12:00:00AM
    time.hour   = 18;
    time.minute = 20;
    time.millisecond = 0;
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
    char tstr[8] = {' ', ' ', ':' ,' ' ,' ' , 'A', 'M', '\0'};
    bool pm = false;

    // format hour by splitting and using ASCII values to convert int to char
    if (time.hour < 10)
        tstr[0] = '0';
    else {
        if (time.hour > 12) {
            pm = true;
            int h = time.hour - 12;
            if (h < 10)
                tstr[0] = '0';
        }
        else
            tstr[0] = intToChar(time.hour / 10);
    }
    if (time.hour > 12)
        tstr[1] = intToChar((time.hour-2) % 10);
    else
        tstr[1] = intToChar(time.hour % 10);

    // format minute in the same way hour was formatted
    if (time.minute < 10)
        tstr[3] = '0';
    else
        tstr[3] = intToChar(time.minute / 10);
    tstr[4] = intToChar(time.minute % 10);

    // check if pm
    if (pm)
        tstr[6] = 'P';
    
    // write converted char[] to lcd
    lcd->clear();
    lcd->write(tstr);
}

/**
 * Checks if the time should be updated
 * @return          true if can be updated, false otherwise
 */


/**
 * Adds one second to the second counter, then updates
 * hour and minute counter accordingly.
 */
void AlarmModule::iterateClock() {
    // increment minute if 60 seconds have passed
    if (time.millisecond >= 60000)
        time.minute++;

    // update minute and hour variables
    if (time.minute == 60) {
        time.minute = 0;
        time.hour++;
    }
    if (time.hour == 24) {
        time.hour = 0;
        time.minute = 0;
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
    if (time.hour <= 23 && time.minute < 60) {
        this->time.hour = time.hour;
        this->time.minute = time.minute;
    }
}

/**
 * Time's mutator
 * @param           hour - new hour to store
 * @param           minute - new minute to store
 * @param           second - new second to store
 */
void AlarmModule::setTime(int hour, int minute) {
    this->time.hour = time.hour;
    this->time.minute = time.minute;
    this->time.millisecond = 0;
}

/**
 * Acts just as delay() does, but listens for RF communication during the delay.
 * @return              true if received STOP command, false otherwise.
 */
bool AlarmModule::delayAndListen(long duration) {
    String cmd = "";
    long start = millis();
    while (millis() - start != duration) {
        cmd = rf->listen();

        //check for stop command
        if (cmd.equals("STOP;")) return true;
    }
    return false;
}

/**
 * Activates the alarm sequence for the module.
 * Will beep and flash (if flash is enabled) until stop command is received
 * (or is unplugged).
 */
void AlarmModule::sound() {
    while (!rf->listen().equals("STOP;")) {
        // turn on leds and tone
        digitalWrite(ledControl, HIGH);
        tone(piezo, 600);

        // delay while listening for stop cmd
        if (delayAndListen(500)) {
            this->silence();
            return;
        }

        // turn off leds and tone
        digitalWrite(ledControl, LOW);
        noTone(piezo);

        // delay while listening for stop cmd
        if (delayAndListen(500)) {
            this->silence();
            return;
        }
    }
}

/**
 * Silences the alarm sequence
 */
void AlarmModule::silence() {
    // turn off LEDs and tone
    digitalWrite(ledControl, LOW);
    noTone(piezo);
}
