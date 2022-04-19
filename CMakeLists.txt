cmake_minimum_required(VERSION 3.5)

project(ServerClient1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
    snodec COMPONENTS core-legacy net-in
)

add_executable(server server.cpp)
target_link_libraries(server PRIVATE snodec::core-legacy snodec::net-in)

add_executable(client client.cpp ClientContext.cpp ClientContext.h KeyboardReader.cpp KeyboardReader.h)
target_link_libraries(client PRIVATE snodec::core-legacy snodec::net-in)
