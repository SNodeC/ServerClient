cmake_minimum_required(VERSION 3.5)

project(ServerClient1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
    snodec COMPONENTS core-socket-stream-legacy core-socket-stream-tls net-in-stream net-in6-stream net-un-stream
)

add_executable(server server.cpp ChatApp.cpp ChatApp.h ServerContext.cpp ServerContext.h)
target_link_libraries(server PRIVATE snodec::core-socket-stream-legacy snodec::core-socket-stream-tls snodec::net-in-stream snodec::net-in6-stream snodec::net-un-stream)

add_executable(client client.cpp ClientContext.cpp ClientContext.h KeyboardReader.cpp KeyboardReader.h)
target_link_libraries(client PRIVATE snodec::core-socket-stream-legacy snodec::core-socket-stream-tls snodec::net-in-stream snodec::net-un-stream)
