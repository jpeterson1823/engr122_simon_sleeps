#ifndef AlarmModule_h
#define AlarmModule_h

#include "RFHandler.h"
#include "LCD.h"

struct Time {
    int hour;
    int minute;
    int millisecond;
};

class AlarmModule {
    private:
        LCD* lcd;
        RFHandler* rf;
        Time time;
        int lastMinute = 0;

        // boolean to allow for disabling flashing lights
        boolean flashEnabled = false;

        // input pins
        int timeSetPin  = 2;
        int alarmSetPin = 3;

        // output pins
        int piezo = 4;
        int ledControl = 5;

    private:
        bool canUpdate();
        void displayTime();
        bool delayAndListen(long duration);

    public:
        AlarmModule();
        ~AlarmModule();
        void sound();
        void silence();
        void iterateClock();
        Time getTime();
        void setTime(Time time);
        void setTime(int hour, int minuten);
};
#endif
