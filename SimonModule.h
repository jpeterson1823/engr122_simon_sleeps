#include "Accelerometer.h"
#include "LCD.h"

#define PLEN 5

class SimonModule {
    private:
        Accelerometer* accel;
        LCD* lcd;
        int pattern[PLEN];
        int patternLength;

        // LED pins {green, red, blue, yellow}
        int leds[4] = {12, 11, 10, 9};
        int tones[4] = {587, 440, 494, 523};
        int piezo = 8;

    public:
        SimonModule();
        void generatePattern();
        void displayPattern();
};
