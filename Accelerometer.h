#ifndef Accelerometer_h
#define Accelerometer_h

//#include "Arduino.h"

// Minimum and Maximum possible raw digital numbers
#define RMIN 0
#define RMAX 1024

// Roll and Pitch offset used to calibrate feeling of Simon game (degrees)
#define ROLL_OFFSET 0
#define PITCH_OFFSET 0

// Deadzone for what is considered "level" by the accelerometer (degrees)
#define LEVEL_ROLL_DEADZONE 30
#define LEVEL_PITCH_DEADZONE 30


class Accelerometer
{
    private:
        // accelerometer pins
        const unsigned int xpin;
        const unsigned int ypin;
        const unsigned int zpin;

        // analog values of each pin
        unsigned int x;
        unsigned int y;
        unsigned int z;

    public:
        Accelerometer(int xpin, int ypin, int zpin);

        // methods for getting roll and pitch
        float getRoll();
        float getPitch();

        // calculates acceleration for x-axis
        int getXAccel();

        // calculates acceleration for y-axis
        int getYAccel();

        // calculates acceleration for z-axis
        int getZAccel();

        // reads accelerometer pins and updates all axis data
        void readInput();

};
#endif
