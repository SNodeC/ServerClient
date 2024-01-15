#include "ClientContext.h" // IWYU pragma: keep
#include "log/Logger.h"

#include <core/SNodeC.h>
#include <net/in/stream/tls/SocketClient.h>
#include <net/un/stream/legacy/SocketClient.h>

int main(int argc, char* argv[]) {
    using Client = net::un::stream::legacy::SocketClient<ClientContextFactory>;
    using SocketConnection = Client::SocketConnection;

    core::SNodeC::init(argc, argv);

    Client client(
        [](SocketConnection* socketConnection) -> void {
            VLOG(0) << "OnConnect from: " << socketConnection->getRemoteAddress().toString();
        },
        [](SocketConnection* socketConnection) -> void {
            VLOG(0) << "OnConnected from: " << socketConnection->getRemoteAddress().toString();
        },
        [](SocketConnection* socketConnection) -> void {
            VLOG(0) << "OnDisconnect from: " << socketConnection->getRemoteAddress().toString();
        });

    client.connect("/tmp/testsocket", [](const Client::SocketAddress& socketAddress, int errnum) -> void {
        if (errnum == 0) {
            VLOG(0) << "Client connected to " << socketAddress.toString();
        } else {
            VLOG(0) << "Error: Client trying to connect to " << socketAddress.toString() << " : errno = " << errnum;
        }
    });

    using ClientTLS = net::in::stream::tls::SocketClient<ClientContextFactory>;
    using SocketConnectionTLS = ClientTLS::SocketConnection;

    ClientTLS clienttls(
        [](SocketConnectionTLS* socketConnection) -> void {
            VLOG(0) << "OnConnect from: " << socketConnection->getRemoteAddress().toString();
        },
        [](SocketConnectionTLS* socketConnection) -> void {
            VLOG(0) << "OnConnected from: " << socketConnection->getRemoteAddress().toString();
        },
        [](SocketConnectionTLS* socketConnection) -> void {
            VLOG(0) << "OnDisconnect from: " << socketConnection->getRemoteAddress().toString();
        });

    clienttls.getConfig()
        .setCertChain("/home/voc/projects/ServerClient/Client-End-Entity.pem")
        .setCertKey("/home/voc/projects/ServerClient/Client-End-Entity-Key.pem")
        .setCertKeyPassword("pentium5")
        .setCaCertFile("/home/voc/projects/ServerClient/VolkerChristianRootCA.pem");

    clienttls.connect("localhost", 8082, [](const ClientTLS::SocketAddress& socketAddress, int errnum) -> void {
        if (errnum < 0) {
            PLOG(ERROR) << "OnError";
        } else if (errnum > 0) {
            PLOG(ERROR) << "OnError: " << socketAddress.toString();
        } else {
            VLOG(0) << "snode.c connecting to " << socketAddress.toString();
        }
    });

    return core::SNodeC::start();
}
