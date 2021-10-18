#include "Accelerometer.h"

#define XPIN 1
#define YPIN 2
#define ZPIN 3

float rollf;
float pitchf;

// create accelerometer object
Accelerometer accelerometer(XPIN, YPIN, ZPIN);

void setup() {
    // Set analog ref voltage
    analogReference(EXTERNAL);

    // get initial reading of the accelerometer
    accelerometer.readInput();
    rollf = accelerometer.getRoll();
    pitchf = accelerometer.getPitch();

    // start serial @ 9600 baud
    Serial.begin(9600);
}


void loop() {
    // read accelerometer input and updates each axis
    accelerometer.readInput();

    // apply lowpass filter to roll and pitch
    rollf = 0.96 * rollf + 0.04 * accelerometer.getRoll();
    pitchf = 0.96 * pitchf + 0.04 * accelerometer.getPitch();

    // display roll and pitch
    Serial.print("Roll: ");
    Serial.println(rollf);
    Serial.print("Pitch: ");
    Serial.println(pitchf);
}
