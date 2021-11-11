#ifndef AlarmModule_h
#define AlarmModule_h

#include "RFHandler.h"
#include "LCD.h"

struct Time {
    int hour;
    int minute;
    long long millisecond;
};

struct Alarm {
    int hour;
    int minute;
};

class AlarmModule {
    private:
        LCD* lcd;
        RFHandler* rf;
        Time time;
        Alarm alarm;
        long long lastUpdate = 0;

        // input pins
        int timeSetPin  = 2;
        int alarmSetPin = 3;

        // output pins
        int piezo = 9;
        int ledControl = 7;

    private:
        void sleep(int duration);
        void displayTime();
        void displayAlarm(bool useFirstRow = false);
        void setCurrentTime(int hour, int minute);
        void setAlarm(int hour, int minute);
        bool delayAndListen(unsigned long duration);

    public:
        AlarmModule();
        ~AlarmModule();
        void sound();
        void silence();
        void iterateClock();
        void checkSetAlarmEvent();
        void checkSetTimeEvent();

        /**
         * Checks to see if the alarm should sound.
         * @return          true if alarm should sound, false otherwise
         */
        inline bool isTime() { return (time.hour == alarm.hour && time.minute == alarm.minute); }
        
};
#endif
