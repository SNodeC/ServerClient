#include "ChatApp.h"

#include <core/socket/SocketContext.h>

ChatApp& ChatApp::instance() {
    static ChatApp chat;

    return chat;
}

void ChatApp::addServerContext(core::socket::SocketContext* serverContext) {
    serverContexts.push_back(serverContext);
}

void ChatApp::removeServerContext(core::socket::SocketContext* serverContext) {
    serverContexts.remove(serverContext);
}

void ChatApp::echoToAll(const std::string& text) {
    for (core::socket::SocketContext* serverContext : serverContexts) {
        serverContext->sendToPeer(text);
    }
}
