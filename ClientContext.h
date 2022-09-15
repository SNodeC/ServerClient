#ifndef CLIENTCONTEXT_H
#define CLIENTCONTEXT_H

#include <core/socket/SocketContext.h>
#include <core/socket/SocketContextFactory.h>

class KeyboardReader;
namespace core::socket {
    class SocketConnection;
}

class ClientContext : public core::socket::SocketContext {
public:
    explicit ClientContext(core::socket::SocketConnection* socketConnection);
    ~ClientContext();

    void keyboardReaderAway();

private:
    void onConnected() override;
    void onDisconnected() override;
    std::size_t onReceiveFromPeer() override;

    KeyboardReader* keyboardReader = nullptr;
};

class ClientContextFactory : public core::socket::SocketContextFactory {
private:
    core::socket::SocketContext* create(core::socket::SocketConnection* socketConnection) override;
};

#endif // CLIENTCONTEXT_H
