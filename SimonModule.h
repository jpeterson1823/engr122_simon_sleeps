#include "Accelerometer.h"
#include "LCD.h"

#define PLEN 5
#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3

class SimonModule {
    // member variables
    private:
        Accelerometer* accel;
        LCD* lcd;
        int pattern[PLEN];
        int patternLength;

        // LED pins {green, red, blue, yellow}
        int leds[4] = {12, 11, 10, 9};
        int tones[4] = {600, 450, 500, 550};
        int piezo = 8;
    
    // private methods
    private:
        int readMovement();

    // public methods
    public:
        SimonModule();
        void generatePattern();
        void displayPattern();
        void playRound();
        void testAccel();
};
