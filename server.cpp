#include <snode.c/core/SNodeC.h>
#include <snode.c/core/socket/SocketContextFactory.h>
#include <snode.c/net/in/stream/legacy/SocketServer.h>

class ServerContextFactory : public core::socket::SocketContextFactory {
    core::socket::SocketContext* create([[maybe_unused]] core::socket::SocketConnection* socketConnection) {
        return nullptr;
    }
};

int main(int argc, char* argv[]) {
    core::SNodeC::init(argc, argv);

    using Server = net::in::stream::legacy::SocketServer<ServerContextFactory>;
    using SocketConnection = net::in::stream::legacy::SocketServer<ServerContextFactory>::SocketConnection;

    Server server(
        [](SocketConnection* socketConnection) -> void {
            std::cout << "OnConnect from: " << socketConnection->getRemoteAddress().toString() << std::endl;
        },
        [](SocketConnection* socketConnection) -> void {
            std::cout << "OnConnected from: " << socketConnection->getRemoteAddress().toString() << std::endl;
        },
        [](SocketConnection* socketConnection) -> void {
            std::cout << "OnDisconnect from: " << socketConnection->getRemoteAddress().toString() << std::endl;
        });

    return core::SNodeC::start();
}
