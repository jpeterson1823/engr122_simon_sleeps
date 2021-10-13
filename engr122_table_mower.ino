#include "Accelerometer.h"

#define XPIN 1
#define YPIN 2
#define ZPIN 3

float rollf;
float pitchf;

// create accelerometer object
Accelerometer accelerometer(XPIN, YPIN, ZPIN);

void setup()
{
    // Set analog ref voltage
    analogReference(EXTERNAL);

    pinMode(A3, INPUT);

    accelerometer.readInput();
    rollf = accelerometer.getRoll();
    pitchf = accelerometer.getPitch();
    Serial.begin(9600);
}


void loop()
{
    // update accelerometer axis digital values
    accelerometer.readInput();
    float roll = accelerometer.getRoll();
    float pitch = accelerometer.getPitch();

    // low-pass filter
    //rollf = 0.94 * rollf + 0.06 * roll;
    //pitchf = 0.94 * pitchf + 0.06 * pitch;

    // print out acceleration of z axis
    Serial.println("##########################################");
    Serial.print("X-Axis Acceleration: ");
    Serial.print(accelerometer.getXAccel() / 1000.0);
    Serial.println("G");
    //Serial.println(analogRead(A1));
    Serial.print("Y-Axis Acceleration: ");
    Serial.print(accelerometer.getYAccel() / 1000.0);
    Serial.println("G");
    //Serial.println(analogRead(A2));
    Serial.print("Z-Axis Acceleration: ");
    Serial.print(accelerometer.getZAccel() / 1000.0);
    Serial.println("G"); 
    //Serial.println(analogRead(A3));
    Serial.print("Roll: ");
    Serial.println(roll);
    Serial.print("Pitch: ");
    Serial.println(pitch);
    Serial.println("##########################################");
    //Serial.println(analogRead(A3), 288, 423, -100, 100));
    Serial.println(analogRead(A3));

    delay(1000);
}


/**
 * Sends a mess-ton of return characters to the Serial monitor,
 * effectively clearing the screen.
 */
void clearSerial()
{
    for (int i = 0; i < 30; i++)
        Serial.println();
}
