#include "ClientContext.h" // IWYU pragma: keep
#include <SemanticLog.h>

#include <core/SNodeC.h>
#include <net/in/stream/tls/SocketClient.h>
#include <net/un/stream/legacy/SocketClient.h>

int main(int argc, char* argv[]) {
    using Client = net::un::stream::legacy::SocketClient<ClientContextFactory>;
    using SocketConnection = Client::SocketConnection;

    core::SNodeC::init(argc, argv);

    Client client(
        [](SocketConnection* socketConnection) -> void {
            snode::semantic::appLog().debug() << "OnConnect from: " << socketConnection->getRemoteAddress().toString();
        },
        [](SocketConnection* socketConnection) -> void {
            snode::semantic::appLog().debug() << "OnConnected from: " << socketConnection->getRemoteAddress().toString();
        },
        [](SocketConnection* socketConnection) -> void {
            snode::semantic::appLog().debug() << "OnDisconnect from: " << socketConnection->getRemoteAddress().toString();
        });

    client.connect("/tmp/testsocket", [](const Client::SocketAddress& socketAddress, int errnum) -> void {
        if (errnum == 0) {
            snode::semantic::appLog().info() << "Client connected to " << socketAddress.toString();
        } else {
            snode::semantic::sysError(snode::semantic::appLog(), logger::LogLevel::Error, errnum)
                << "Client failed to connect to " << socketAddress.toString();
        }
    });

    using ClientTLS = net::in::stream::tls::SocketClient<ClientContextFactory>;
    using SocketConnectionTLS = ClientTLS::SocketConnection;

    ClientTLS clienttls(
        [](SocketConnectionTLS* socketConnection) -> void {
            snode::semantic::appLog().debug() << "OnConnect from: " << socketConnection->getRemoteAddress().toString();
        },
        [](SocketConnectionTLS* socketConnection) -> void {
            snode::semantic::appLog().debug() << "OnConnected from: " << socketConnection->getRemoteAddress().toString();
        },
        [](SocketConnectionTLS* socketConnection) -> void {
            snode::semantic::appLog().debug() << "OnDisconnect from: " << socketConnection->getRemoteAddress().toString();
        });

    clienttls.getConfig()
        .setCertChain("/home/voc/projects/ServerClient/Client-End-Entity.pem")
        .setCertKey("/home/voc/projects/ServerClient/Client-End-Entity-Key.pem")
        .setCertKeyPassword("pentium5")
        .setCaCertFile("/home/voc/projects/ServerClient/VolkerChristianRootCA.pem");

    clienttls.connect("localhost", 8082, [](const ClientTLS::SocketAddress& socketAddress, int errnum) -> void {
        if (errnum < 0) {
            snode::semantic::sysError(snode::semantic::appLog(), logger::LogLevel::Error, errnum) << "OnError";
        } else if (errnum > 0) {
            snode::semantic::sysError(snode::semantic::appLog(), logger::LogLevel::Error, errnum)
                << "OnError: " << socketAddress.toString();
        } else {
            snode::semantic::appLog().info() << "snode.c connecting to " << socketAddress.toString();
        }
    });

    return core::SNodeC::start();
}
