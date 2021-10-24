#include "Arduino.h"
#include "SimonModule.h"
#include <stdlib.h>

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

    // play start tone
    for(int i = 0; i < 2; i++) {
        tone(piezo, 400);
        delay(100);
        noTone(piezo);
        delay(100);
    }
}

void SimonModule::generatePattern() {
    for (int i = 0; i < PLEN; i++)
        pattern[i] = rand() % 4;
}

void SimonModule::displayPattern() {
    for (int i = 0; i < PLEN; i++) {
        digitalWrite(leds[pattern[i]], HIGH);
        tone(piezo, tones[pattern[i]]);
        delay(250);
        digitalWrite(leds[pattern[i]], LOW);
        noTone(piezo);
        delay(100);
    }
}

/** TODO
 * Waits for the player to tilt the module in a direction, then returns that
 * direciton in terms of the constands UP, DOWN, LEFT, and RIGHT.
 * @return          UP, DOWN, LEFT, or RIGHT
 */
int SimonModule::readMovement() {
    while (true) {
        float roll = accel->getRoll();
        float pitch = accel->getPitch();
        break;
    }
}

/**
 * Plays a round of Simon Says and returns the result
 * @return          true if player is correct, false otherwise
 */
void SimonModule::playRound() {
    for (int step = 0; step < patternLength; step++) {
        if (pattern[step] == UP) {

        }
    }
}

void SimonModule::testAccel() {
    long t1 = millis();
    while (millis - t1 < 10000) {
        Serial.print("Roll: ");
        Serial.println(accel->getRoll());

        Serial.print("Pitch: ");
        Serial.println(accel->getPitch());

        Serial.println();

        delay(100);
    }
}