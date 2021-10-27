#ifndef __LCD_h_
#define __LCD_h_

#include "LiquidCrystal_I2C.h"
#include <stdint.h>

class LCD {
    private:
        int address;
        LiquidCrystal_I2C* lcd;

    public:
        LCD(uint8_t address);
        void write(const char* str);
        void clear();
};

#endif
