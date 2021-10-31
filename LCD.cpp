#include "LCD.h"
#include "LiquidCrystal_I2C.h"

// Destructor
LCD::~LCD() {
    delete lcd;
}

// Default constructor
LCD::LCD(uint8_t address) {
    // initialize lcd
    lcd = new LiquidCrystal_I2C(address, 16, 2);
    lcd->begin();
    lcd->backlight();
    lcd->clear();
    lcd->setCursor(0, 0);
}

// Clears the LCD screen
void LCD::clear() {
    lcd->clear();
}

// Writes a string to the LCD
void LCD::write(const char* str, int row = 0) {
    lcd->setCursor(0, row);
    lcd->printstr(str);
}

// Returns the LCD object
LiquidCrystal_I2C* LCD::getLiquidCrystal() {
    return lcd;
}