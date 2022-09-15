cmake_minimum_required(VERSION 3.5)

project(ServerClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
    snodec COMPONENTS net-in-stream-legacy net-in-stream-tls net-in6-stream-legacy net-in6-stream-tls net-un-stream-legacy
)

find_program(iwyu_path NAMES include-what-you-use iwyu)

if(iwyu_path)
    option(CHECK_INCLUDES "Check used headers")

    set(iwyu_path_and_options
        ${iwyu_path}
        -Xiwyu
        --verbose=3
        -Xiwyu
        --cxx17ns
        -Xiwyu
        --quoted_includes_first
        -Xiwyu
        --check_also='${PROJECT_SOURCE_DIR}/*'
    )

    if(CHECK_INCLUDES)
        set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path_and_options})
    endif(CHECK_INCLUDES)
else()
    message(
        AUTHOR_WARNING "Could not find the program include-what-you-use"
    )
endif()

add_executable(server server.cpp ChatApp.cpp ChatApp.h ServerContext.cpp ServerContext.h)
target_link_libraries(server PRIVATE snodec::net-in-stream-legacy snodec::net-in-stream-tls snodec::net-in6-stream-legacy snodec::net-in6-stream-tls snodec::net-un-stream-legacy)

add_executable(client client.cpp ClientContext.cpp ClientContext.h KeyboardReader.cpp KeyboardReader.h)
target_link_libraries(client PRIVATE snodec::net-un-stream-legacy snodec::net-in-stream-tls)
