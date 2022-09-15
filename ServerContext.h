#ifndef SERVERCONTEXT_H
#define SERVERCONTEXT_H

#include <core/socket/SocketContext.h> // IWYU pragma: export
#include <core/socket/SocketContextFactory.h>

// IWYU pragma: no_include <SocketContext.h>

namespace core::socket {
    class SocketConnection;
}

class ServerContext : public core::socket::SocketContext {
public:
    explicit ServerContext(core::socket::SocketConnection* socketConnection);

private:
    void onConnected() override;

    void onDisconnected() override;

    std::size_t onReceiveFromPeer() override;
};

class ServerContextFactory : public core::socket::SocketContextFactory {
    core::socket::SocketContext* create(core::socket::SocketConnection* socketConnection);
};

#endif // SERVERCONTEXT_H
