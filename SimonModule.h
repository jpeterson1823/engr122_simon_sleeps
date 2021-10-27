#ifndef SimonModule_h
#define SimonModule_h

#include "Accelerometer.h"
#include "LCD.h"
#include "RFHandler.h"

#define PLEN 5
#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3

class SimonModule {
    // member variables
    private:
        Accelerometer* accel;
        LCD* lcd;
        RFHandler* rf;

        // array to store pattern
        int pattern[PLEN];

        // LED pins {green, red, yellow, blue}
        int leds[4] = {2, 3, 4, 5};
        int tones[4] = {440, 494, 523, 587 };
        int piezo = 6;

        // thermistor connected to A3
        int thermistor = 7;
    
    // private methods
    private:
        int readMovement();
        void waitForLevel();
        bool disableAlarm();

    // public methods
    public:
        SimonModule();
        ~SimonModule();
        void generatePattern();
        void displayPattern();
        void playRound();
        void testAccel();
};

#endif
