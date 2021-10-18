#include "Arduino.h"
#include "Accelerometer.h"
#include <math.h>

#define SAMPLE_SIZE 10

// Accelerometer constructor just sets values to given ones.
Accelerometer::Accelerometer(int xpin, int ypin, int zpin)
    : xpin(xpin), ypin(ypin), zpin(zpin) { /* Empty */ };

//float Accelerometer::calcAccel(int val, float bias, float sensitivity) {
//    return (((float)val / 1024.0 * ADC_ref - bias)) / sensitivity;
//}

int Accelerometer::getXAccel() {
    return map(x, RMIN, RMAX, -3000, 3000);
}

int Accelerometer::getYAccel() {
    return map(y, RMIN, RMAX, -3000, 3000);
}

int Accelerometer::getZAccel() {
    return map(z, RMIN, RMAX, -3000, 3000);
}

float Accelerometer::getRoll() {
    return atan(getYAccel() / sqrt(pow(getYAccel(), 2) + pow(getZAccel(), 2))) * 180 / PI;
}

float Accelerometer::getPitch() {
    return atan(-1 * getXAccel() / sqrt(pow(getYAccel(), 2) + pow(getZAccel(), 2))) * 180 / PI;
}

int readAxis(int pin)
{
    long reading = 0;
    for (int i = 0; i < SAMPLE_SIZE; i++)
        reading += analogRead(pin);
    return reading / SAMPLE_SIZE;
}

void Accelerometer::readInput()
{
    x = readAxis(xpin);
    y = readAxis(ypin);
    z = readAxis(zpin);
}


