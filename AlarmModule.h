#ifndef AlarmModule_h
#define AlarmModule_h

#include "LCD.h"

struct Time {
    int hour;
    int minute;
    int second;
};

class AlarmModule {
    private:
        LCD* lcd;
        Time time;
        long lastIter;

        // input pins
        int timeSetPin  = 5;
        int alarmSetPin = 6;

    private:
        bool canUpdate();
        void displayTime();

    public:
        AlarmModule();
        void iterate();
        Time getTime();
        void setTime(Time time);
        void setTime(int hour, int minute, int second);
};
#endif
