cmake_minimum_required(VERSION 3.0.0)
project(websockets_testing)
add_compile_options(-std=c++11)

include_directories(
        include
        usr/include
        /usr/local/include
        msgpack-c/include
        uWebSockets/src
        /usr/local/Cellar/opencv/4.2.0_1/include/opencv4
)

link_directories(/usr/local/lib)
link_directories(/usr/local/Cellar/opencv/4.2.0_1/lib)

# macOS only:
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

    include_directories(/usr/local/opt/openssl/include)
    link_directories(/usr/local/opt/openssl/lib)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


file(GLOB UWS
        "uWebSockets/src/*.c*"
        )

file(GLOB OPENCV_LIBS
        "/usr/local/Cellar/opencv/4.2.0_1/lib/*.dylib"
        )

add_library(uwserver STATIC ${UWS} include/uWServer_b.h)

add_executable(video_send src/video_send.cpp ${UWS} include/uWServer_b.h)
target_link_libraries(video_send crypto z uv ssl pthread ${OPENCV_LIBS}
        )

add_executable(example_server src/example_server.cpp ${UWS} include/uWServer_b.h)
target_link_libraries(example_server crypto z uv ssl pthread)

add_executable(example_client src/example_client.cpp ${UWS} include/uWClient_b.h)
target_link_libraries(example_client crypto z uv ssl pthread)

# if compiling uWS directly (NEEDS CRYPTO!!):
add_executable(server src/server.cpp ${UWS} src/uWServer.cpp src/uWClient.cpp )
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