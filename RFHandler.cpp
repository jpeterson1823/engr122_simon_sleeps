#include "RFHandler.h"

/* Empty  deconstructor */
RFHandler::~RFHandler() { /* empty */ }

/**
 * Default constructor.
 * Handles initialization of RH_ASK
 */
RFHandler::RFHandler() {
    rf = new RH_ASK();
    rf->init();
}

/**
 * Sends a string out as 433MHz
 * @param               str - string to be sent
 */
void RFHandler::send(String str) {
    const char* msg = str.c_str();
    rf->send((uint8_t *)msg, strlen(msg));
    rf->waitPacketSent();
    delay(100);
}

/**
 * Listens for an attempt at sending data
 * @return              true if message received, false otherwise
 */
bool RFHandler::listen() {
    uint8_t buf[12];
    uint8_t buflen = sizeof(buf);

    if (rf->recv(buf, &buflen)) {
        Serial.print("Message: ");
        Serial.println((char*)buf);
        return true;
    }
    return false;
}
