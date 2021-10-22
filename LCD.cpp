#include <Wire.h>
#include <Arduino.h>
#include "LCD.h"
#include "LiquidCrystal_I2C.h"


LCD::LCD(uint8_t address) {
    // initialize lcd
    lcd = new LiquidCrystal_I2C(address, 16, 2);
    lcd->begin();
    lcd->backlight();
    lcd->clear();
    lcd->setCursor(0, 0);
}

void LCD::clear() {
    lcd->clear();
}

void LCD::write(const char* str) {
    lcd->printstr(str);
}
