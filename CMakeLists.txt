cmake_minimum_required(VERSION 3.5)

project(ServerClient1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
    snodec COMPONENTS core-legacy core-tls net-in net-in6 net-un
)

add_executable(server server.cpp ChatApp.cpp ChatApp.h ServerContext.cpp ServerContext.h)
target_link_libraries(server PRIVATE snodec::core-legacy snodec::core-tls snodec::net-in snodec::net-in6 snodec::net-un)

add_executable(client client.cpp ClientContext.cpp ClientContext.h KeyboardReader.cpp KeyboardReader.h)
target_link_libraries(client PRIVATE snodec::core-legacy snodec::core-tls snodec::net-in snodec::net-un)
