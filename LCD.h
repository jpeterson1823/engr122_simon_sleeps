#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include <stdint.h>

class LCD {
    private:
        byte address;
        LiquidCrystal_I2C* lcd;

    public:
        LCD(uint8_t address);
        void write(const char* str);
        void clear();
};
