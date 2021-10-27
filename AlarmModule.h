#ifndef AlarmModule_h
#define AlarmModule_h

#include "RFHandler.h"
#include "LCD.h"

struct Time {
    int hour;
    int minute;
    int second;
};

class AlarmModule {
    private:
        LCD* lcd;
        RFHandler* rf;
        Time time;
        long lastIter;

        // input pins
        int timeSetPin  = 2;
        int alarmSetPin = 3;

        // output pins
        int piezo = 4;
        int ledControl = 5;

    private:
        bool canUpdate();
        void displayTime();

    public:
        AlarmModule();
        ~AlarmModule();
        void iterate();
        Time getTime();
        void setTime(Time time);
        void setTime(int hour, int minute, int second);
};
#endif
