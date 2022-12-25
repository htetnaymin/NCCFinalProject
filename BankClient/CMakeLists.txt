cmake_minimum_required(VERSION 3.21)
project(BankClient)

set(CMAKE_CXX_STANDARD 14)

link_libraries(ws2_32 wsock32)
add_executable(BankClient main.cpp client.cpp)
