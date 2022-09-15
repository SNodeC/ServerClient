#include "ServerContext.h" // IWYU pragma: keep
#include "log/Logger.h"

#include <core/SNodeC.h>
#include <net/in/stream/legacy/SocketServer.h>
#include <net/in/stream/tls/SocketServer.h>
#include <net/in6/stream/legacy/SocketServer.h>
#include <net/un/stream/legacy/SocketServer.h>

int main(int argc, char* argv[]) {
    core::SNodeC::init(argc, argv);

    using Server = net::in::stream::legacy::SocketServer<ServerContextFactory>;
    using SocketConnection = net::in::stream::legacy::SocketServer<ServerContextFactory>::SocketConnection;

    Server server(
        [](SocketConnection* socketConnection) -> void {
            VLOG(0) << "OnConnect from: " << socketConnection->getRemoteAddress().toString();
        },
        [](SocketConnection* socketConnection) -> void {
            VLOG(0) << "OnConnected from: " << socketConnection->getRemoteAddress().toString();
        },
        [](SocketConnection* socketConnection) -> void {
            VLOG(0) << "OnDisconnect from: " << socketConnection->getRemoteAddress().toString();
        });

    server.listen(8080, 5, [](const Server::SocketAddress& socketAddress, int errnum) -> void {
        if (errnum < 0) {
            PLOG(ERROR) << "OnError";
        } else if (errnum > 0) {
            PLOG(ERROR) << "OnError: " << socketAddress.toString();
        } else {
            VLOG(0) << "snode.c listening on " << socketAddress.toString();
        }
    });

    using ServerTLS = net::in::stream::tls::SocketServer<ServerContextFactory>;
    using SocketConnectionTLS = net::in::stream::tls::SocketServer<ServerContextFactory>::SocketConnection;

    std::map<std::string, std::any> options;
    options["CertChain"] = "/home/voc/projects/ServerClient/WebServerCertificateChain.pem";
    options["CertChainKey"] = "/home/voc/projects/ServerClient/Volker_Christian_-_WEB-Cert.pem";
    options["Password"] = "pentium5";
    options["CaFile"] = "/home/voc/projects/ServerClient/Client-Root-CA.crt";

    ServerTLS servertls(
        [](SocketConnectionTLS* socketConnection) -> void {
            VLOG(0) << "OnConnect from: " << socketConnection->getRemoteAddress().toString();
        },
        [](SocketConnectionTLS* socketConnection) -> void {
            VLOG(0) << "OnConnected from: " << socketConnection->getRemoteAddress().toString();
        },
        [](SocketConnectionTLS* socketConnection) -> void {
            VLOG(0) << "OnDisconnect from: " << socketConnection->getRemoteAddress().toString();
        },
        options);

    servertls.listen(8082, 5, [](const ServerTLS::SocketAddress& socketAddress, int errnum) -> void {
        if (errnum < 0) {
            PLOG(ERROR) << "OnError";
        } else if (errnum > 0) {
            PLOG(ERROR) << "OnError: " << socketAddress.toString();
        } else {
            VLOG(0) << "snode.c listening on " << socketAddress.toString();
        }
    });

    using Server6 = net::in6::stream::legacy::SocketServer<ServerContextFactory>;
    using SocketConnection6 = net::in6::stream::legacy::SocketServer<ServerContextFactory>::SocketConnection;

    Server6 server6(
        [](SocketConnection6* socketConnection) -> void {
            VLOG(0) << "OnConnect from: " << socketConnection->getRemoteAddress().toString();
        },
        [](SocketConnection6* socketConnection) -> void {
            VLOG(0) << "OnConnected from: " << socketConnection->getRemoteAddress().toString();
        },
        [](SocketConnection6* socketConnection) -> void {
            VLOG(0) << "OnDisconnect from: " << socketConnection->getRemoteAddress().toString();
        });

    server6.listen(8081, 5, [](const Server6::SocketAddress& socketAddress, int errnum) -> void {
        if (errnum < 0) {
            PLOG(ERROR) << "OnError";
        } else if (errnum > 0) {
            PLOG(ERROR) << "OnError: " << socketAddress.toString();
        } else {
            VLOG(0) << "snode.c listening on " << socketAddress.toString();
        }
    });

    using ServerUn = net::un::stream::legacy::SocketServer<ServerContextFactory>;
    using SocketConnectionUn = net::un::stream::legacy::SocketServer<ServerContextFactory>::SocketConnection;

    ServerUn serverUn(
        [](SocketConnectionUn* socketConnection) -> void {
            VLOG(0) << "OnConnect from: " << socketConnection->getRemoteAddress().toString();
        },
        [](SocketConnectionUn* socketConnection) -> void {
            VLOG(0) << "OnConnected from: " << socketConnection->getRemoteAddress().toString();
        },
        [](SocketConnectionUn* socketConnection) -> void {
            VLOG(0) << "OnDisconnect from: " << socketConnection->getRemoteAddress().toString();
        });

    serverUn.listen("/tmp/testsocket", 5, [](const SocketConnectionUn::SocketAddress& socketAddress, int errnum) -> void {
        if (errnum < 0) {
            PLOG(ERROR) << "OnError";
        } else if (errnum > 0) {
            PLOG(ERROR) << "OnError: " << socketAddress.toString();
        } else {
            VLOG(0) << "snode.c listening on " << socketAddress.toString();
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
