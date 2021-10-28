#include <Arduino.h>
#include <Wire.h>
#include "SimonModule.h"
#include "AlarmModule.h"
#include "RFHandler.h"

/*
To make sure that each module only runs code it's supposed to,
a digitalRead() call will be made on specific pins. If one returns
HIGH, then the corresponding module code will be set.
*/
const uint8_t simonPin = 8;
const uint8_t alarmPin = 5;       //TBD
uint8_t module;

// Create class object pointers
SimonModule* smod;
AlarmModule* amod;
RFHandler* rf;

void setup() {
    // start serial
    Serial.begin(9600);

    // get module code
    //module = determineModule();
    module = 0;

    // do current module's setup
    switch(module) {
        case 0:
            Serial.println("Simon Setup");
            rf = new RFHandler();
            //simonSetup;
            break;

        case 1:
            pinMode(13, OUTPUT);
            Serial.println("Alarm Setup");
            rf = new RFHandler();
            //alarmSetup();
            break;

        default:
            break;
    }
}

void loop() {
    // do current module's loop
    switch (module) {
        case 0:
            simonLoop();
            break;
        
        case 1:
            alarmLoop();
            break;
    }
}

/**
 * Determines the module this script is currently running on by
 * doing digitalRead() calls to the preset pins
 * @return              the current module's code
 */
int determineModule() {
    // set pinmodes of module pins
    pinMode(simonPin, INPUT);
    pinMode(alarmPin, INPUT);

    // check pin's status and return module's code
    if (digitalRead(simonPin) == HIGH)
        return 0;
    else if (digitalRead(alarmPin) == HIGH)
        return 1;
    else
        return -1;
}

// Handles simon's setup
void simonSetup() {
        smod = new SimonModule();
}

// What simon should do each loop iteration
void simonLoop() {
    digitalWrite(13, LOW);
    delay(2000);
    rf->send("test;");
    Serial.println("Sent.");
    while (rf->listen().equals("NONE;")) { /* do nothing */ }
    digitalWrite(13, HIGH);
    Serial.println("Received.");
}

// Handles alarm's setup
void alarmSetup() {
        amod = new AlarmModule();
}

// What alarm should do each loop iteration
void alarmLoop() {
        // iterate clock
        //amod->iterate();
        //delay(100);
        while (rf->listen().equals("NONE;")) { /* do nothing */ }
        Serial.println("Received");
        delay(2000);
        rf->send("test;");
        Serial.println("Sent");
}

/*
// scans all device ports and print any connection addresses to the serial monitor
void deviceScan() {
    // init wire
    Wire.begin();

    byte error, address;
    int devices = 0;

    // begin scan
    Serial.println("Scanning...");
    for (address = 1; address < 127; address++) {
        // start and end a transmission
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        // if no error, device is there
        if (error == 0) {
            Serial.print("\tI2C device found at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.print(address, HEX);
            Serial.println();
            devices++;
        }
        // unknown error thrown. idk what that means
        else if (error == 4) {
            Serial.print("\tUnknown error at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    Serial.println("Scan completed");
    if (devices == 0)
        Serial.println("No I2C devices found\n");
}
*/