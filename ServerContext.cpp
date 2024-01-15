#include "ServerContext.h"

#include "ChatApp.h"

#include <iostream>
#include <string> // for string

ServerContext::ServerContext(core::socket::stream::SocketConnection* socketConnection)
    : core::socket::stream::SocketContext(socketConnection) {
}

void ServerContext::onConnected() {
    std::cout << "Context connected to Peer" << std::endl;
    ChatApp::instance().addServerContext(this);
}

void ServerContext::onDisconnected() {
    std::cout << "Context disconnecte4d from Peer" << std::endl;
    ChatApp::instance().removeServerContext(this);
}

bool ServerContext::onSignal(int signum) {
    return true;
}

std::size_t ServerContext::onReceivedFromPeer() {
    char buffer[1024];

    std::size_t numBytesRead = readFromPeer(buffer, 1023);
    buffer[numBytesRead] = 0;

    std::cout << "Buffer: " << buffer << std::endl;

    ChatApp::instance().echoToAll(std::string(buffer, numBytesRead));

    return numBytesRead;
}

core::socket::stream::SocketContext* ServerContextFactory::create(core::socket::stream::SocketConnection* socketConnection) {
    return new ServerContext(socketConnection);
}
