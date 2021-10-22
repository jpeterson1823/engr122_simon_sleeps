#include "SimonModule.h"
#include <Wire.h>

#define XPIN 1
#define YPIN 2
#define ZPIN 3

SimonModule* smod;


void setup() {
    smod = new SimonModule();
}

void loop() {
    smod->generatePattern();
    smod->displayPattern();
    delay(1000);
}


// scans all device ports and print any connection addresses to the serial monitor
void deviceScan() {
    // init wire
    Wire.begin();

    byt error, address;
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
