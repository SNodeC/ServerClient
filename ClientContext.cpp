#include "ClientContext.h"

#include <log/Logger.h>
#include <string>

ClientContext::ClientContext(core::socket::SocketConnection* socketConnection)
    : core::socket::SocketContext(socketConnection)
    , keyboardReader(new KeyboardReader(this, [this](std::string text) -> void {
        sendToPeer(text);
    })) {
}

ClientContext::~ClientContext() {
    if (keyboardReader != nullptr) {
        keyboardReader->clientContextAway();
    }
}

void ClientContext::onConnected() {
}

void ClientContext::onDisconnected() {
    if (keyboardReader != nullptr) {
        keyboardReader->stop();
    }
}

void ClientContext::onReceiveFromPeer() {
    char buffer[1024];

    ssize_t numBytesRead = readFromPeer(buffer, 1024);

    VLOG(0) << "Buffer: " << std::string(buffer, numBytesRead);
}

void ClientContext::keyboardReaderAway() {
    keyboardReader = nullptr;
}

core::socket::SocketContext* ClientContextFactory::create(core::socket::SocketConnection* socketConnection) {
    return new ClientContext(socketConnection);
}
