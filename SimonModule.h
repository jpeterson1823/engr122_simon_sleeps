#include "Accelerometer.h"
#include "LCD.h"

#define PLEN 5
#define FORWARD 0
#define BACKWARD 1
#define RIGHT 2
#define LEFT 3

class SimonModule {
    // member variables
    private:
        Accelerometer* accel;
        LCD* lcd;

        // array to store pattern
        int pattern[PLEN];

        // LED pins {green, red, blue, yellow}
        int leds[4] = {12, 11, 10, 9};
        int tones[4] = {587, 440, 494, 523};
        int piezo = 8;

        // thermistor connected to A3
        int thermistor = 3;
    
    // private methods
    private:
        int readMovement();
        void waitForLevel();
        bool disableAlarm();

    // public methods
    public:
        SimonModule();
        void generatePattern();
        void displayPattern();
        void playRound();
        void testAccel();
};
