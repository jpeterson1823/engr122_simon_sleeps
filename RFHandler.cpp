#include "RFHandler.h"

// Destructor
RFHandler::~RFHandler() {
    delete rf;
}

/**
 * Default constructor.
 * Handles initialization of RH_ASK
 */
RFHandler::RFHandler() {
    rf = new RH_ASK();
    if (!rf->init()) Serial.println("RF init failed.");
    else Serial.print("RF initiated.");
}

/**
 * Sends a string out as 433MHz
 * @param               str - string to be sent
 */
void RFHandler::send(String str) {
    const char* msg = str.c_str();
    rf->send((uint8_t *)msg, strlen(msg));
    rf->waitPacketSent();
    delay(1000);
}

/**
 * Listens for an attempt at sending data
 * @return              true if message received, false otherwise
 */
String RFHandler::listen() {
    uint8_t buf[CMD_LEN];
    uint8_t buflen = sizeof(buf);

    if (rf->recv(buf, &buflen)) {
        String msg = "";
        for (int i = 0; i < CMD_LEN; i++) 
            msg += (char)buf[i];
        return msg;
    }
    else
        return "NONE;";
}
