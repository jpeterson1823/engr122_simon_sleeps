#ifndef RFHandler_h
#define RFHandler_h

#define CMD_LEN 4

#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>

class RFHandler {
    private:
        RH_ASK* rf;

    public:
        RFHandler();
        ~RFHandler();
        void send(String str);
        String listen();
};

#endif