#include "SimonModule.h"
#include <Wire.h>

SimonModule* smod;

void setup() {
    // start Serial
    Serial.begin(9600);

    smod = new SimonModule();
    smod->playRound();
}

void loop() {

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
