#ifndef SERVERCONTEXT_H
#define SERVERCONTEXT_H

#include <core/socket/stream/SocketContext.h> // IWYU pragma: export
#include <core/socket/stream/SocketContextFactory.h>

// IWYU pragma: no_include <SocketContext.h>

namespace core::socket {
    class SocketConnection;
}

class ServerContext : public core::socket::stream::SocketContext {
public:
    explicit ServerContext(core::socket::stream::SocketConnection* socketConnection);

private:
    void onConnected() override;
    void onDisconnected() override;
    [[nodiscard]] bool onSignal(int signum) override;

    std::size_t onReceivedFromPeer() override;
};

class ServerContextFactory : public core::socket::stream::SocketContextFactory {
    core::socket::stream::SocketContext* create(core::socket::stream::SocketConnection* socketConnection);
};

#endif // SERVERCONTEXT_H
