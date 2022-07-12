#include "ChatApp.h"
#include "ServerContext.h"

#include <any>
#include <cerrno>
#include <core/SNodeC.h>
#include <map>
#include <net/in/stream/legacy/SocketServer.h>
#include <net/in/stream/tls/SocketServer.h>
#include <net/in6/stream/legacy/SocketServer.h>
#include <net/un/stream/legacy/SocketServer.h>
#include <set>
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
        if (errnum < 0) {
            PLOG(ERROR) << "OnError";
        } else if (errnum > 0) {
            errno = errnum;
            PLOG(ERROR) << "OnError: " << socketAddress.toString();
        } else {
            VLOG(0) << "snode.c connecting to " << socketAddress.toString();
        }
    });

    using ServerTLS = net::in::stream::tls::SocketServer<ServerContextFactory>;
    using SocketConnectionTLS = net::in::stream::tls::SocketServer<ServerContextFactory>::SocketConnection;

    std::map<std::string, std::any> options;
    options["CertChain"] = "/home/voc/projects/ServerClient1/WebServerCertificateChain.pem";
    options["CertChainKey"] = "/home/voc/projects/ServerClient1/Volker_Christian_-_WEB-Cert.pem";
    options["Password"] = "pentium5";
    options["CaFile"] = "/home/voc/projects/ServerClient1/Client-Root-CA.crt";

    ServerTLS servertls(
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

    servertls.listen(8082, 5, [](const ServerTLS::SocketAddress& socketAddress, int errnum) -> void {
        if (errnum < 0) {
            PLOG(ERROR) << "OnError";
        } else if (errnum > 0) {
            errno = errnum;
            PLOG(ERROR) << "OnError: " << socketAddress.toString();
        } else {
            VLOG(0) << "snode.c connecting to " << socketAddress.toString();
        }
    });

    using Server6 = net::in6::stream::legacy::SocketServer<ServerContextFactory>;
    using SocketConnection6 = net::in6::stream::legacy::SocketServer<ServerContextFactory>::SocketConnection;

    Server6 server6(
        [](SocketConnection6* socketConnection) -> void {
            std::cout << "OnConnect from: " << socketConnection->getRemoteAddress().toString() << std::endl;
        },
        [](SocketConnection6* socketConnection) -> void {
            std::cout << "OnConnected from: " << socketConnection->getRemoteAddress().toString() << std::endl;
        },
        [](SocketConnection6* socketConnection) -> void {
            std::cout << "OnDisconnect from: " << socketConnection->getRemoteAddress().toString() << std::endl;
        });

    server6.listen(8081, 5, [](const Server6::SocketAddress& socketAddress, int errnum) -> void {
        if (errnum < 0) {
            PLOG(ERROR) << "OnError";
        } else if (errnum > 0) {
            errno = errnum;
            PLOG(ERROR) << "OnError: " << socketAddress.toString();
        } else {
            VLOG(0) << "snode.c connecting to " << socketAddress.toString();
        }
    });

    using ServerUn = net::un::stream::legacy::SocketServer<ServerContextFactory>;
    using SocketConnectionUn = net::un::stream::legacy::SocketServer<ServerContextFactory>::SocketConnection;

    ServerUn serverUn(
        [](SocketConnectionUn* socketConnection) -> void {
            std::cout << "OnConnect from: " << socketConnection->getRemoteAddress().toString() << std::endl;
        },
        [](SocketConnectionUn* socketConnection) -> void {
            std::cout << "OnConnected from: " << socketConnection->getRemoteAddress().toString() << std::endl;
        },
        [](SocketConnectionUn* socketConnection) -> void {
            std::cout << "OnDisconnect from: " << socketConnection->getRemoteAddress().toString() << std::endl;
        });

    serverUn.listen("/tmp/testsocket", 5, [](const SocketConnectionUn::SocketAddress& socketAddress, int errnum) -> void {
        if (errnum < 0) {
            PLOG(ERROR) << "OnError";
        } else if (errnum > 0) {
            errno = errnum;
            PLOG(ERROR) << "OnError: " << socketAddress.toString();
        } else {
            VLOG(0) << "snode.c connecting to " << socketAddress.toString();
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
