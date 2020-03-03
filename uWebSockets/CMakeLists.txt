cmake_minimum_required(VERSION 3.0.0)
project(websockets_testing)
add_compile_options(-std=c++11)

#find_package(uWS)

include_directories(
        include
        usr/include
        /usr/local/include
)

link_directories(/usr/local/lib)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

    include_directories(/usr/local/opt/openssl/include)
    link_directories(/usr/local/opt/openssl/lib)
#    link_directories(/usr/local/Cellar/libuv/1*/lib)


endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

add_executable(ws src/ws.cpp)
target_link_libraries(ws z ssl uv uWS pthread)

add_executable(main src/source.cpp src/uWServer.cpp)
target_link_libraries(main z ssl uv uWS pthread)
