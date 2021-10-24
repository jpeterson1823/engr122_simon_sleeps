#include "Arduino.h"
#include "SimonModule.h"
#include <stdlib.h>

/**
 * Default constructor for SimonModule
 */
SimonModule::SimonModule() {
    accel = new Accelerometer(2, 1, 0);
    lcd = new LCD(0x27);
    
    // clear lcd and display startup message
    lcd->clear();
    lcd->write("Starting Up...");
    
    // set piezo pinmode
    pinMode(piezo, OUTPUT);
    // set pinmodes for leds
    for(int i = 0; i < 4; i++)
        pinMode(leds[i], OUTPUT);

    // set pinmode for thermistor
    pinMode(thermistor, INPUT);
}

/**
 * Generates and stores a random pattern of PLEN length using a thermistor, along with
 * the built-in arduino functions micros() and millis(), to introduce some true-ish
 * randomness into this generator.
 */
void SimonModule::generatePattern() {
    for (int i = 0; i < PLEN; i++) {
        // use thermistor to generate some randomness
        srand(analogRead(thermistor) * micros() / millis());
        pattern[i] = rand() % 4;
    }
}

/**
 * Displays the pattern on the leds with the appropriate tone
 */
void SimonModule::displayPattern() {
    for (int i = 0; i < PLEN; i++) {
        Serial.print(pattern[i]);
        Serial.print(" ");

        digitalWrite(leds[pattern[i]], HIGH);
        tone(piezo, tones[pattern[i]]);
        delay(250);
        digitalWrite(leds[pattern[i]], LOW);
        noTone(piezo);
        delay(100);
    }
    Serial.println();
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
            return LEFT;
        else if (pitch < -1 * LEVEL_PITCH_DEADZONE)
            return RIGHT;
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
    
    // loop until player gets pattern right
    int movements[PLEN] = {};
    while (true) {
        displayPattern();
        // get player movements
        for (int movement = 0; movement < PLEN; movement++) {
            movements[movement] = readMovement();
            Serial.print(movements[movement]);
            Serial.print(" ");

            // light LED and make tone that corresponds with the movement
            digitalWrite(leds[movements[movement]], HIGH);
            tone(piezo, tones[movements[movement]]);
            delay(100);
            digitalWrite(leds[movements[movement]], LOW);
            noTone(piezo);

            // wait for player to level module out before taking next movement
            waitForLevel();
        }
        Serial.println();

        // check if movements are correct
        bool correct = true;
        for (int step = 0; step < PLEN; step++)
            if (pattern[step] != movements[step])
                correct = false;


        // if correct, disable alarm
        if (correct) {
            disableAlarm();
            digitalWrite(leds[0], HIGH);
            digitalWrite(leds[1], HIGH);
            digitalWrite(leds[2], HIGH);
            digitalWrite(leds[3], HIGH);
            break;
        }

        // else, flash all leds on module 3 times with a tone to signal incorrect pattern
        else {
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
    }
}


bool SimonModule::disableAlarm() {
    Serial.println("ALARM DISABLED");
    return true;
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
