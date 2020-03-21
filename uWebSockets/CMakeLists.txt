cmake_minimum_required(VERSION 3.0.0)
project(websockets_testing)
add_compile_options(-std=c++11)

include_directories(
        include
        usr/include
        /usr/local/include
        ../../uWebSockets/src
)

link_directories(/usr/local/lib)

# macOS only:
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

    include_directories(/usr/local/opt/openssl/include)
    link_directories(/usr/local/opt/openssl/lib)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

file(GLOB UWS
        "../../uWebSockets/src/*.c*"
        )

#add_executable(ws src/ws.cpp)
#target_link_libraries(ws z ssl uv uWS pthread)

# if compiling uWS directly (NEEDS CRYPTO!!):
add_executable(server src/source.cpp ${UWS} src/uWServer.cpp src/uWClient.cpp )
target_link_libraries(server crypto z uv ssl  pthread)

# if compiling using library installed uWS:
#add_executable(server src/source.cpp src/uWServer.cpp src/uWClient.cpp )
#target_link_libraries(server z uv ssl  pthread)

# if compiling uWS directly:
add_executable(client src/client.cpp ${UWS} src/uWServer.cpp src/uWClient.cpp )
target_link_libraries(client  crypto z uv ssl pthread)

# if compiling using library installed uWS:
#add_executable(client src/client.cpp  src/uWServer.cpp src/uWClient.cpp )
#target_link_libraries(client  z uv ssl pthread)