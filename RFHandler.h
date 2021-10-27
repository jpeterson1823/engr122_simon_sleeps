#ifndef RFHandler_h
#define RFHandler_h

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
        bool listen();
};

#endif