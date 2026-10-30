cmake_minimum_required(VERSION 3.16)

project(TCPThreadPoolServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tcp_server
    src/main.cpp
    src/Tcpserver.cpp
    src/ThreadPool.cpp
)

target_include_directories(tcp_server PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

find_package(Threads REQUIRED)

target_link_libraries(tcp_server PRIVATE
    Threads::Threads
)