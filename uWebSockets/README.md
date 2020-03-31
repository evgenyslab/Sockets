# Websockets in C++

project for testing websocket based communication from C++ backend to website front end.

This repo has a git submodule with uWebsockets that is compiled together with test code.

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

## Testing Notes

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

### Older

uWebSockets seems to work really well. In combination with js-side WS client or local C++ client JSON
string messages and binary data can be easily passed.

Was able to send JPEG binary data and PNG binary data and receive on JS application side as binary, and
then decode using `URL.createObjectURL(event.data)`, **HOWEVER** in **CHROME** there is a limit
on `ws` frame size, thus, if `uWS` sends a frame 350kB (somewhere between 300-350kB), chrome cannot handle
it; however Safari was tested to work. `Safari` was able to handle an 8mB image with `uWS`.

Also used `npx create-react-app` to see if a rudimentary react app could be made with ws implementation - 
seems to work!

**TODO**: figure out how to configure `ws` on chrome client-side to correctly handle large messages.

It seems as thought its possible to push up ws to send more data correctly by sending larger and larger sized 
objects

How to make ws client reconnect automatically?

