cmake_minimum_required(VERSION 3.21)
project(BankServer)

set(CMAKE_CXX_STANDARD 14)

link_libraries(ws2_32 wsock32)
add_executable(BankServer main.cpp server.cpp)
