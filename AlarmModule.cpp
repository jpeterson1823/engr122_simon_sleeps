#include <Arduino.h>
#include "AlarmModule.h"

// Destructor
AlarmModule::~AlarmModule() {
    delete lcd;
    delete rf;
}

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
    lcd->write("Starting up...");

    // create RF transmitter and receiver handler
    rf = new RFHandler();

    // set default time to 12:00 AM
    time.hour   = 6;
    time.minute = 59;
    time.millisecond = 55000;

    // set default alarm to 6:00 AM
    alarm.hour = 7;
    alarm.minute = 0;

    // delay to let things catch up and then display time and alarm
    delay(500);
    displayTime();
    displayAlarm();
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
    String tstr = " Time: ";
    bool pm = false;

    if (time.hour == 0)
        tstr += "12";
    else if (time.hour < 10) {
        tstr += "0" + (String)time.hour;
    }
    else {
        if (time.hour >= 12) {
            pm = true;
            int h = time.hour - 12;
            if (h == 0) tstr += "12";
            else if (h < 10) tstr += "0" + (String)h;
            else tstr += (String)h;
        }
        else tstr += (String)time.hour;
    }

    // add colon
    tstr += ':';

    // format minute in the same way hour was formatted
    if (time.minute < 10)
        tstr += "0" + (String)time.minute;
    else
        tstr += (String)time.minute;

    // check if pm
    if (pm)
        tstr += 'P';
    else
        tstr += 'A';
    tstr += 'M';
    
    // write converted char[] to lcd first row
    lcd->clear();
    lcd->write(tstr.c_str());
}

/**
 * Displays the current alarm on the LCD screen
 * Should be the same method as displayTime but im lazy.
 */
void AlarmModule::displayAlarm(bool useFirstRow) {
    // create null terminated char[] (basically a string)
    String tstr = "Alarm: ";
    bool pm = false;

    if (alarm.hour == 0)
        tstr += "12";
    else if (alarm.hour < 10) {
        tstr += "0" + (String)alarm.hour;
    }
    else {
        if (alarm.hour >= 12) {
            pm = true;
            int h = alarm.hour - 12;
            if (h == 0) tstr += "12";
            else if (h < 10) tstr += "0" + (String)h;
            else tstr += (String)h;
        }
        else tstr += (String)alarm.hour;
    }

    // add colon
    tstr += ':';

    // format minute in the same way hour was formatted
    if (alarm.minute < 10)
        tstr += "0" + (String)alarm.minute;
    else
        tstr += (String)alarm.minute;

    // check if pm
    if (pm)
        tstr += 'P';
    else
        tstr += 'A';
    tstr += 'M';
    
    // write converted char[] to lcd second row
    if (useFirstRow) {
        lcd->clear();
        lcd->write(tstr.c_str(), 0);
    }
    else
        lcd->write(tstr.c_str(), 1);
}

/**
 * Adds one minute to the counter if 60 seconds have passed, then updates
 * hour and minute counter accordingly.
 */
void AlarmModule::iterateClock() {
    // update delta time and millisecond counter
    long long dT = millis() - lastUpdate;
    time.millisecond += dT;
    lastUpdate = millis();

    // increment minute if 60 seconds have passed
    if (time.millisecond >= 60000) {
        time.millisecond = 0;
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
        displayTime();
        displayAlarm();
    }
}

/**
 * Time's mutator
 * @param           hour - new hour to store
 * @param           minute - new minute to store
 */
void AlarmModule::setCurrentTime(int hour, int minute) {
    this->time.hour = time.hour;
    this->time.minute = time.minute;
    this->time.millisecond = 0;
}

/**
 * Alarm's mutator
 * @param           hour - hour to sound alarm
 * @param           minute - minute to sound alarm
 */
void AlarmModule::setAlarm(int hour, int minute) {
    alarm.hour = hour;
    alarm.minute = minute;
}

/**
 * Acts just as delay() does, but listens for RF communication during the delay.
 * @return              true if received STOP command, false otherwise.
 */
bool AlarmModule::delayAndListen(unsigned long duration) {
    String cmd = "";
    unsigned long start = millis();
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
    // send the start command to the simon module
    for (int i = 0; i < 5; i++)
        rf->send("ALRM;");

    // begin listening while sounding...
    String s = rf->listen();
    while (!s.equals("STOP;")) {
        s = rf->listen();
        // turn on leds and tone
        digitalWrite(ledControl, HIGH);
        tone(piezo, 600);

        // delay while listening for stop cmd
        //if (delayAndListen(500)) {
        //    this->silence();
        //    Serial.println("Stopping sound...");
        //    s = "STOP;";
        //}
        for (int i = 0; i < 50; i++) {
            s = rf->listen();
            if (s.equals("STOP;")) break;
            delay(10);
        }

        // turn off leds and tone
        digitalWrite(ledControl, LOW);
        noTone(piezo);

        // delay while listening for stop cmd
        //if (delayAndListen(500)) {
        //    this->silence();
        //    Serial.println("Stopping sound...");
        //    s = "STOP;";
        //}

        for (int i = 0; i < 50; i++) {
            s = rf->listen();
            if (s.equals("STOP;")) break;
            delay(10);
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

    lcd->clear();
    lcd->write("Alarm Silenced!");
}

void AlarmModule::checkSetTimeEvent() {
    if (digitalRead(timeSetPin) == HIGH) {
        // clear screen and delay for user QoL
        lcd->clear();
        delay(500);

        // create header and display current option and selection
        String header = "Set: ";
        displayTime();
        lcd->write("Setting: HOUR", 1);

        // Set hour
        displayTime();
        lcd->write("Setting: HOUR", 1);
        while (digitalRead(timeSetPin) != HIGH) {
            if (digitalRead(alarmSetPin) == HIGH) {
                time.hour++;
                if (time.hour > 23)
                    time.hour = 0;
                displayTime();
                lcd->write("Setting: HOUR", 1);
            }
            delay(175);
        }

        //clear screen and delay for user's QoL
        lcd->clear();
        delay(250);

        // Set minute
        displayTime();
        lcd->write("Setting: MINUTE", 1);
        while (digitalRead(timeSetPin) != HIGH) {
            if (digitalRead(alarmSetPin) == HIGH) {
                time.minute++;
                if (time.minute > 59)
                    time.minute = 0;
                displayTime();
                lcd->write("Setting: MINUTE", 1);
            }
            delay(100);
        }

        // clear screen and let user know the update has taken effect
        lcd->clear();
        lcd->write("Clock Updated!");
        delay(500);

        // display time normally along with alarm
        displayTime();
        displayAlarm();
    }
}

void AlarmModule::checkSetAlarmEvent() {
    if (digitalRead(alarmSetPin) == HIGH) {
        // clear lcd and delay for user QoL
        lcd->clear();
        delay(500);

        // Set hour
        displayAlarm(true);
        lcd->write("Setting: HOUR", 1);
        while (digitalRead(timeSetPin) != HIGH) {
            if (digitalRead(alarmSetPin) == HIGH) {
                alarm.hour++;
                if (alarm.hour > 23)
                    alarm.hour = 0;
                displayAlarm(true);
                lcd->write("Setting: HOUR", 1);
            }
            delay(175);
        }

        //clear screen and delay for user's QoL
        lcd->clear();
        delay(250);

        // Set minute
        displayAlarm(true);
        lcd->write("Setting: MINUTE", 1);
        while (digitalRead(timeSetPin) != HIGH) {
            if (digitalRead(alarmSetPin) == HIGH) {
                alarm.minute++;
                if (alarm.minute > 59)
                    alarm.minute = 0;
                displayAlarm(true);
                lcd->write("Setting: MINUTE", 1);
            }
            delay(100);
        }
        
        // clear screen and let user know the update has taken effect
        lcd->clear();
        lcd->write("Clock Updated!");
        delay(500);

        // display alarm normally along with time.
        displayTime();
        displayAlarm();
    }
}

/**
 * Checks to see if the alarm should sound
 * @return              true if alarm should sound, false otherwise
 */
bool AlarmModule::isTime() {
    return (time.hour == alarm.hour && time.minute == alarm.minute);
}
