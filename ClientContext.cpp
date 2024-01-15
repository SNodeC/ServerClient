#include "ClientContext.h"

#include "KeyboardReader.h" // for KeyboardReader

#include <functional> // for function
#include <log/Logger.h>
#include <string>

ClientContext::ClientContext(core::socket::stream::SocketConnection* socketConnection)
    : core::socket::stream::SocketContext(socketConnection)
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

bool ClientContext::onSignal(int signum) {
    return true;
}

std::size_t ClientContext::onReceivedFromPeer() {
    char buffer[1024];

    std::size_t numBytesRead = readFromPeer(buffer, 1024);

    VLOG(0) << "Buffer: " << std::string(buffer, numBytesRead);

    return numBytesRead;
}

void ClientContext::keyboardReaderAway() {
    keyboardReader = nullptr;
}

core::socket::stream::SocketContext* ClientContextFactory::create(core::socket::stream::SocketConnection* socketConnection) {
    return new ClientContext(socketConnection);
}
