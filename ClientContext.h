#ifndef CLIENTCONTEXT_H
#define CLIENTCONTEXT_H

#include <core/socket/stream/SocketContext.h>
#include <core/socket/stream/SocketContextFactory.h>

class KeyboardReader;
namespace core::socket::stream {
    class SocketConnection;
}

class ClientContext : public core::socket::stream::SocketContext {
public:
    explicit ClientContext(core::socket::stream::SocketConnection* socketConnection);
    ~ClientContext();

    void keyboardReaderAway();

private:
    void onConnected() override;
    void onDisconnected() override;
    [[nodiscard]] bool onSignal(int signum) override;

    std::size_t onReceivedFromPeer() override;

    KeyboardReader* keyboardReader = nullptr;
};

class ClientContextFactory : public core::socket::stream::SocketContextFactory {
private:
    core::socket::stream::SocketContext* create(core::socket::stream::SocketConnection* socketConnection) override;
};

#endif // CLIENTCONTEXT_H
