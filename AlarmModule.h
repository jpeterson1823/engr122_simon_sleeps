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

        // boolean to allow for disabling flashing lights
        boolean flashEnabled = false;

        // input pins
        int timeSetPin  = 2;
        int alarmSetPin = 3;

        // output pins
        int piezo = 9;
        int ledControl = 7;

    private:
        void displayTime();
        void displayAlarm(bool useFirstRow = false);
        bool delayAndListen(unsigned long duration);

        void setCurrentTime(int hour, int minute);
        void setAlarm(int hour, int minute);

    public:
        AlarmModule();
        ~AlarmModule();
        void sound();
        void silence();
        void iterateClock();
        void checkSetAlarmEvent();
        void checkSetTimeEvent();
        bool isTime();
        
};
#endif
