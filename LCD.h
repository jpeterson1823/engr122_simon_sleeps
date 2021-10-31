#ifndef LCD_h
#define LCD_h

#include "LiquidCrystal_I2C.h"
#include <stdint.h>

class LCD {
    private:
        int address;
        LiquidCrystal_I2C* lcd;

    public:
        LCD(uint8_t address);
        ~LCD();
        void write(const char* str, int row = 0);
        void clear();
        LiquidCrystal_I2C* getLiquidCrystal();
};

#endif
