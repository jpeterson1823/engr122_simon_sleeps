#ifndef Accelerometer_h
#define Accelerometer_h

#include "Arduino.h"

// Minimum and Maximum possible raw digital numbers
#define RMIN 0
#define RMAX 1024

class Accelerometer
{
    private:
        // ADC Reference Voltage
        //const float ADC_ref;

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
