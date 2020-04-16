# Websockets in C++

project for testing websocket based communication from C++ backend to website front end.

This repo has a git submodule with uWebsockets that is compiled together with test code.

## TODO List

- [ ] cython/python wrapping server + client headers
- [x] test webcam stream to browser (new app, using c++ lib + react app)
- [x] App.js ws socket select url & connect
- [x] s++ client auto reconnect
- [x] loopback speedtest from server to webclient back to server @ different package sizes

## Testing Notes

### April 2 2020

Was able to configure the `uWS::Hub` on both client and server to correctly handle larger
amount of data. Specifically, in the server, needed to select hub server group and 
enable async:

```cpp
if (h.listen("0.0.0.0",this->port)) {
    printf("Server listening on port: %d\n", this->port);
    // add async to server hub to correctly handle asynchronous sending...
    h.getDefaultGroup<uWS::SERVER>().addAsync();
    h.run();
}
```

### March 31 2020

Updated the App.js function with fileReader to decode byte array using msgpack &
added messagepacking of binary data to send to server

### March 20 2020

In Node.h for client side socket connection is done only once, meaning that the client 
must be run after the server is run. The question is can Node.h be adapted to try to automatically
connect and reconnect to server in the absence of a server?

in [Node.h](uWebSockets/src/Node.h):
```cpp
while (::connect(fd, result->ai_addr, result->ai_addrlen) < 0){
            ::close(fd);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            // reset...
            fd = netContext->createSocket(result->ai_family, result->ai_socktype, result->ai_protocol);
            if (fd == INVALID_SOCKET) {
                freeaddrinfo(result);
                return nullptr;
            }
        }
```

# Installing uWebSockets

To install uWebsockets on system use the following..

## Linux install:

```bash
git clone https://github.com/uNetworking/uWebSockets.git
cd uWebSockets
git checkout v0.14
make -j 8
sudo make install
```


## Mac Install:

```bash
brew install openssl zlib libuv
git clone https://github.com/uNetworking/uWebSockets.git
cd uWebSockets
git checkout v0.14
make -j 8
sudo make install
```

