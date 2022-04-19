#include <core/SNodeC.h>
#include <core/socket/SocketContext.h>
#include <core/socket/SocketContextFactory.h>
#include <list>
#include <net/in/stream/legacy/SocketServer.h>
#include <string>

class ChatApp {
private:
    ChatApp() = default;
    ChatApp(const ChatApp&) = delete;
    ChatApp& operator=(const ChatApp&) = delete;

public:
    static ChatApp& instance() {
        static ChatApp chat;

        return chat;
    }

    void addServerContext(core::socket::SocketContext* serverContext) {
        serverContexts.push_back(serverContext);
    }

    void removeServerContext(core::socket::SocketContext* serverContext) {
        serverContexts.remove(serverContext);
    }

    void echoToAll(const std::string& text) {
        for (core::socket::SocketContext* serverContext : serverContexts) {
            serverContext->sendToPeer(text);
        }
    }

private:
    std::list<core::socket::SocketContext*> serverContexts;
};

class ServerContext : public core::socket::SocketContext {
public:
    explicit ServerContext(core::socket::SocketConnection* socketConnection)
        : core::socket::SocketContext(socketConnection) {
    }

private:
    void onConnected() override {
        std::cout << "Context connected to Peer" << std::endl;
        ChatApp::instance().addServerContext(this);
    }

    void onDisconnected() override {
        std::cout << "Context disconnecte4d from Peer" << std::endl;
        ChatApp::instance().removeServerContext(this);
    }

    void onReceiveFromPeer() override {
        char buffer[1024];

        ssize_t numBytesRead = readFromPeer(buffer, 1023);
        buffer[numBytesRead] = 0;

        std::cout << "Buffer: " << buffer << std::endl;

        ChatApp::instance().echoToAll(std::string(buffer, numBytesRead));
    }
};

class ServerContextFactory : public core::socket::SocketContextFactory {
    core::socket::SocketContext* create(core::socket::SocketConnection* socketConnection) {
        return new ServerContext(socketConnection);
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
