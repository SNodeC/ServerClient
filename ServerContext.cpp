#include "ServerContext.h"

#include "ChatApp.h"

#include <iostream>

ServerContext::ServerContext(core::socket::SocketConnection* socketConnection)
    : core::socket::SocketContext(socketConnection) {
}

void ServerContext::onConnected() {
    std::cout << "Context connected to Peer" << std::endl;
    ChatApp::instance().addServerContext(this);
}

void ServerContext::onDisconnected() {
    std::cout << "Context disconnecte4d from Peer" << std::endl;
    ChatApp::instance().removeServerContext(this);
}

void ServerContext::onReceiveFromPeer() {
    char buffer[1024];

    ssize_t numBytesRead = readFromPeer(buffer, 1023);
    buffer[numBytesRead] = 0;

    std::cout << "Buffer: " << buffer << std::endl;

    ChatApp::instance().echoToAll(std::string(buffer, numBytesRead));
}

core::socket::SocketContext *ServerContextFactory::create(core::socket::SocketConnection *socketConnection) {
    return new ServerContext(socketConnection);
}
