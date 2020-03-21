# Websockets in C++

project for testing websocket based communication from C++ backend to website front end.

requires installation of uWebsockets v0.14:

https://github.com/uNetworking/uWebSockets/tree/v0.14


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

uWebSockets seems to work really well. In combination with js-side WS client or local C++ client JSON
string messages and binary data can be easily passed.

Was able to send JPEG binary data and PNG binary data and receive on JS application side as binary, and
then decode using `URL.createObjectURL(event.data)`, **HOWEVER** in **CHROME** there is a limit
on `ws` frame size, thus, if `uWS` sends a frame 350kB (somewhere between 300-350kB), chrome cannot handle
it; however Safari was tested to work.

Also used `npx create-react-app` to see if a rudimentary react app could be made with ws implementation - 
seems to work!

**TODO**: figure out how to configure `ws` on chrome client-side to correctly handle large messages.