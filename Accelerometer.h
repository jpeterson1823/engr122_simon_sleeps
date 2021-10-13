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
        // getters for axes
        unsigned int getX();
        unsigned int getY();
        unsigned int getZ();
        
        // methods for getting roll and pitch
        float getRoll();
        float getPitch();

    public:
        Accelerometer(int xpin, int ypin, int zpin);

        // calculates acceleration for x-axis
        int getXAccel();

        // calculates acceleration for y-axis
        int getYAccel();

        // calculates acceleration for z-axis
        int getZAccel();

        // prints formatted accelerometer info to serial monitor
        void printInfo();

        // reads accelerometer pins and updates all axis data
        void readInput();

};
#endif
