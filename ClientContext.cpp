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

std::size_t ClientContext::onReceiveFromPeer() {
    char buffer[1024];

    std::size_t numBytesRead = readFromPeer(buffer, 1024);

    VLOG(0) << "Buffer: " << std::string(buffer, numBytesRead);

    return numBytesRead;
}

void ClientContext::keyboardReaderAway() {
    keyboardReader = nullptr;
}

core::socket::SocketContext* ClientContextFactory::create(core::socket::SocketConnection* socketConnection) {
    return new ClientContext(socketConnection);
}
