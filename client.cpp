#include "ClientContext.h"

#include <core/SNodeC.h>
#include <net/in/stream/tls/SocketClient.h>
#include <net/un/stream/legacy/SocketClient.h>
#include <string>

int main(int argc, char* argv[]) {
    //    using Client = net::un::stream::legacy::SocketClient<ClientContextFactory>;
    //    using SocketConnection = Client::SocketConnection;

    core::SNodeC::init(argc, argv);
    /*
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

        client.connect("/tmp/testsocket", [](const Client::SocketAddress& socketAddress, int errnum) -> void {
            if (errnum == 0) {
                std::cout << "Client connected to " << socketAddress.toString() << std::endl;
            } else {
                std::cout << "Error: Client trying to connect to " << socketAddress.toString() << " : errno = " << errnum;
            }
        });
    */

    using ClientTLS = net::in::stream::tls::SocketClient<ClientContextFactory>;
    using SocketConnectionTLS = ClientTLS::SocketConnection;

    std::map<std::string, std::any> options;
    options["CertChain"] = "/home/voc/projects/ServerClient1/Client-End-Entity.pem";
    options["CertChainKey"] = "/home/voc/projects/ServerClient1/Client-End-Entity-Key.pem";
    options["Password"] = "pentium5";
    options["CaFile"] = "/home/voc/projects/ServerClient1/VolkerChristianRootCA.pem";

    ClientTLS clienttls(
        [](SocketConnectionTLS* socketConnection) -> void {
            std::cout << "OnConnect from: " << socketConnection->getRemoteAddress().toString() << std::endl;
        },
        [](SocketConnectionTLS* socketConnection) -> void {
            std::cout << "OnConnected from: " << socketConnection->getRemoteAddress().toString() << std::endl;
        },
        [](SocketConnectionTLS* socketConnection) -> void {
            std::cout << "OnDisconnect from: " << socketConnection->getRemoteAddress().toString() << std::endl;
        },
        options);

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
