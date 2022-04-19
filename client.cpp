#include "ClientContext.h"

#include <core/SNodeC.h>
#include <net/in/stream/legacy/SocketClient.h>
#include <string>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    using Client = net::in::stream::legacy::SocketClient<ClientContextFactory>;
    using SocketConnection = Client::SocketConnection;

    core::SNodeC::init(argc, argv);

    Client client(
        [](SocketConnection* socketConnection) -> void {
            std::cout << "OnConnect from: " << socketConnection->getRemoteAddress().toString() << std::endl;
        },
        [](SocketConnection* socketConnection) -> void {
            std::cout << "OnConnected from: " << socketConnection->getRemoteAddress().toString() << std::endl;
        },
        [](SocketConnection* socketConnection) -> void {
            std::cout << "OnDisconnect from: " << socketConnection->getRemoteAddress().toString() << std::endl;
        });

    client.connect("localhost", 8080, [](const Client::SocketAddress& socketAddress, int errnum) -> void {
        if (errnum == 0) {
            std::cout << "Client connected to " << socketAddress.toString() << std::endl;
        } else {
            std::cout << "Error: Client trying to connect to " << socketAddress.toString() << " : errno = " << errnum;
        }
    });

    return core::SNodeC::start();
}
