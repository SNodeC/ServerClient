#include "KeyboardReader.h"

#include "ClientContext.h"

#include <cerrno>
#include <iostream>
#include <string>
#include <unistd.h>

KeyboardReader::KeyboardReader(ClientContext* clientContext, const std::function<void(std::string)>& callBack)
    : core::eventreceiver::ReadEventReceiver("KeyboardReader", 0)
    , callBack(callBack)
    , clientContext(clientContext) {
    if (enable(STDIN_FILENO)) {
        std::cout << "KeyboardReader enabled";
    } else {
        std::cout << "KeyboardReader not enabled";
    }
}

KeyboardReader::~KeyboardReader() {
    if (clientContext != nullptr) {
        clientContext->keyboardReaderAway();
    }
}

void KeyboardReader::readEvent() {
    char buffer[256];

    ssize_t ret = read(STDIN_FILENO, buffer, 256);

    if (ret > 0) {
        if (clientContext != nullptr) {
            callBack(std::string(buffer, ret));
        }
    } else {
        std::cout << "Nothing entered or error: " << errno;
    }
}

void KeyboardReader::stop() {
    disable();
}

void KeyboardReader::clientContextAway() {
    clientContext = nullptr;
}

void KeyboardReader::unobservedEvent() {
    delete this;
}
