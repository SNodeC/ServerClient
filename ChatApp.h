#ifndef CHATAPP_H
#define CHATAPP_H

namespace core::socket {
    class SocketContext;
}

#include <list>
#include <string>

class ChatApp {
private:
    ChatApp() = default;
    ChatApp(const ChatApp&) = delete;
    ChatApp& operator=(const ChatApp&) = delete;

public:
    static ChatApp& instance();

    void addServerContext(core::socket::SocketContext* serverContext);

    void removeServerContext(core::socket::SocketContext* serverContext);

    void echoToAll(const std::string& text);

private:
    std::list<core::socket::SocketContext*> serverContexts;
};

#endif // CHATAPP_H
