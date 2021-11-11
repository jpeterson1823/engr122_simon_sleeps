#include <Arduino.h>
#include <Wire.h>
#include "SimonModule.h"
#include "AlarmModule.h"
#include "RFHandler.h"

/*
This is what tells the program which module's code to run.
Simon Module = 0
Alarm Module = 1
*/
#define MODULE 1


// PROTOTYPES
void deviceScan();


// Create class object pointers
SimonModule* smod;
AlarmModule* amod;


// setup
void setup() {
    // start serial
    Serial.begin(9600);

    //deviceScan();

    Serial.print("Module: ");
    Serial.println(MODULE);


    // do current module's setup
    if (MODULE == 0)
        // instantiate smod and allocate in memory.
        smod = new SimonModule();

    else if (MODULE == 1)
        // instantiate amod and allocate in memory.
        amod = new AlarmModule();
    else
        // Print error to serial monitor
        Serial.println("Unknown module code has been defined. Must be either 0 or 1");
}

// loop
void loop() {
    // use switch statement here since this is called all the time
    switch (MODULE) {
        case 0:
            // wait until alarm sends start message
            smod->waitForAlarm();

            // once start command received, wait for user to tilt the device twice
            smod->waitForUserReady();

            // when user readys up, play two rounds
            smod->playRound();
            smod->playRound();

            // send stop cmd when both rounds completed
            smod->disableAlarm();

            // end case
        break;

        case 1:
            // check if alarm should go off
            bool isTime = amod->isTime();
            
            switch (isTime) {
                // if not, update clock display
                case false:
                    amod->iterateClock();
                    amod->checkSetAlarmEvent();
                    amod->checkSetTimeEvent();
                break;

                // sound alarm
                case true:
                    amod->sound();
                    // after silenced, loop until alarm will no longer be sounding (aka for ~60 seconds)
                    while (amod->isTime()) {
                        amod->iterateClock();
                        amod->checkSetAlarmEvent();
                        amod->checkSetTimeEvent();
                    }
                break;
            }
        break;
    }
}




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
            Serial.println("");
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
