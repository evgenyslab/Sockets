cmake_minimum_required(VERSION 3.0.0)
project(Sockets)
add_compile_options(-std=c++11)

include_directories(
        include
)

add_executable(test src/test.cpp include/server.h include/common_header.h include/client.h)
target_link_libraries(test pthread)