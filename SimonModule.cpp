#include <Arduino.h>
#include <stdlib.h>
#include "SimonModule.h"

// Destructor
SimonModule::~SimonModule() {
    delete accel;
    delete lcd;
    delete rf;
    delete pattern;
}

/**
 * Default constructor for SimonModule
 */
SimonModule::SimonModule() {
    // initialize class pointers
    accel = new Accelerometer(2, 1, 0);
    rf = new RFHandler();

    // set piezo pinmode
    pinMode(piezo, OUTPUT);
    // set pinmodes for leds
    for(int i = 0; i < 4; i++)
        pinMode(leds[i], OUTPUT);

    // set random seed
    randomSeed(analogRead(0) + analogRead(thermistor) / analogRead(1));
}

/**
 * Generates and stores a random pattern of PLEN length using a thermistor, along with
 * the built-in arduino functions micros() and millis(), to introduce some true-ish
 * randomness into this generator.
 */
void SimonModule::generatePattern() {
    for (int i = 0; i < PLEN; i++) {
        // use thermistor to generate some randomness
        pattern[i] = random(0, 4);
    }
}

/**
 * Displays the pattern on the leds with the appropriate tone
 */
void SimonModule::displayPattern() {
    for (int i = 0; i < PLEN; i++) {
        digitalWrite(leds[pattern[i]], HIGH);
        tone(piezo, tones[pattern[i]]);
        delay(250);
        digitalWrite(leds[pattern[i]], LOW);
        noTone(piezo);
        delay(250);
    }
}

/** TODO
 * Waits for the player to tilt the module in a direction, then returns that
 * direciton in terms of the constands FORWARD, BACKWARD, LEFT, and RIGHT.
 * @return          UP, DOWN, LEFT, or RIGHT
 */
int SimonModule::readMovement() {
    while (true) {
        accel->readInput();
        float roll = accel->getRoll();
        float pitch = accel->getPitch();

        if (roll > LEVEL_ROLL_DEADZONE)
            return FORWARD;
        else if (roll < -1 * LEVEL_ROLL_DEADZONE)
            return BACKWARD;

        else if (pitch > LEVEL_PITCH_DEADZONE)
            return RIGHT;
        else if (pitch < -1 * LEVEL_PITCH_DEADZONE)
            return LEFT;
    }
}

/**
 * Waits until the player rotates the module back within the roll and pitch deadzone
 */
void SimonModule::waitForLevel() {
    // wait until both roll and pitch are in deadzone
    while (true) {
        // get roll and pitch
        accel->readInput();
        float roll = accel->getRoll();
        float pitch = accel->getPitch();

        // boolean logic: if both roll and pitch in deadzone
        int buffer = 5;
        bool rollInDead  = (roll  < LEVEL_ROLL_DEADZONE - buffer  && roll  > -1 * LEVEL_ROLL_DEADZONE + buffer );
        bool pitchInDead = (pitch < LEVEL_PITCH_DEADZONE - buffer && pitch > -1 * LEVEL_PITCH_DEADZONE + buffer);

        if (rollInDead && pitchInDead)
            break;
        else
            delay(100);
    }
}

/**
 * Plays a round of Simon Says and returns the result
 * @return          true if player is correct, false otherwise
 */
void SimonModule::playRound() {
    // generate and display a pattern
    generatePattern();
    delay(100);
    
    // loop until player gets pattern right
    int movements[PLEN] = {};
    while (true) {
        Serial.println("Displaying pattern...");
        displayPattern();
        Serial.println("Done displaying pattern.");
        // get player movements
        for (int movement = 0; movement < PLEN; movement++) {
            movements[movement] = readMovement();

            // light LED and make tone that corresponds with the movement
            digitalWrite(leds[movements[movement]], HIGH);
            tone(piezo, tones[movements[movement]]);
            delay(100);
            digitalWrite(leds[movements[movement]], LOW);
            noTone(piezo);

            // wait for player to level module out before taking next movement
            waitForLevel();
        }

        // check if movements are correct
        bool correct = true;
        for (int step = 0; step < PLEN; step++)
            if (pattern[step] != movements[step])
                correct = false;


        // if failed, flash LEDs to show they were wrong.
        if(!correct) {
            for (int i = 0; i < 3; i++) {
                digitalWrite(leds[0], HIGH);
                digitalWrite(leds[1], HIGH);
                digitalWrite(leds[2], HIGH);
                digitalWrite(leds[3], HIGH);
                tone(piezo, 700);

                delay(250);

                digitalWrite(leds[0], LOW);
                digitalWrite(leds[1], LOW);
                digitalWrite(leds[2], LOW);
                digitalWrite(leds[3], LOW);
                noTone(piezo);

                delay(250);
            }
        }
        // otherwise, break loop and exit method
        else break;
    }
    delay(100);
}

/**
 * Continuously listens for the alarm's sound command.
 */
void SimonModule::waitForAlarm() {
    while (!rf->listen().equals("ALRM;")) { /* do nothing */ }
}

/**
 * Sends disable command to alarm
 * @return              true if alarm confirms it has been disabled, false otherwise
 */
bool SimonModule::disableAlarm() {
    // send disable command
    for (int i = 0; i < 10; i++) {
        Serial.println("Sending stop command...");
        rf->send("STOP;");
    }

    // wait for confirmation from alarm module
    String cmd = "";
    while (cmd.equals("") || cmd.equals("NONE;"))
        cmd = rf->listen();
    
    if (cmd == "CONF;") return true;
    else return false;
}


/*
void SimonModule::testAccel() {
    accel->readInput();
    Serial.print("Roll: ");
    Serial.println(accel->getRoll());

    Serial.print("Pitch: ");
    Serial.println(accel->getPitch());

    Serial.println();
}
*/
