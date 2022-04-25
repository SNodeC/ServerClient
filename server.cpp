#include "ChatApp.h"
#include "ServerContext.h"

#include <core/SNodeC.h>
#include <net/in/stream/legacy/SocketServer.h>
#include <string>

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

    server.listen(8080, 5, [](const Server::SocketAddress& socketAddress, int errnum) -> void {
        if (errnum == 0) {
            std::cout << "Server listening on " << socketAddress.toString() << std::endl;
        } else {
            std::cout << "Error: Server trying to listen on " << socketAddress.toString() << " : errno = " << errnum;
        }
    });

    /*
        sighandler_t oldSigIntHandler = core::system::signal(SIGINT, stoponsig);

        do {
            core::SNodeC::tick(10);
        } while (!stop);

        core::SNodeC::free();

        core::system::signal(SIGINT, oldSigIntHandler);

        return 0;
    */

    return core::SNodeC::start();
}
