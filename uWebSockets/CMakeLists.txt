cmake_minimum_required(VERSION 3.0.0)
project(websockets_testing)
add_compile_options(-std=c++11)

#find_package(uWS)

include_directories(
        include
        usr/include
)

add_executable(ws src/ws.cpp)
target_link_libraries(ws ssl crypto uWS z pthread)