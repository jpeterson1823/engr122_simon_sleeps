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
        tone(piezo, tones[i]);
        delay(250);
        digitalWrite(leds[pattern[i]], LOW);
        noTone(piezo);
        delay(100);
    }
}
