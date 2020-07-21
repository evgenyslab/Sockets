# CPP Sockets Generic Interfacing Class

*Project is split from [here](https://github.com/evgenyslab/Sockets/tree/v0.0.0)*

-------

# install gtest on mac

```bash
git clone https://github.com/google/googletest.git
cd googletest
git checkout release-1.10.0
mkdir build
cd build
cmake ..
make -j 8
sudo make install

echo "export CPLUS_INCLUDE_PATH=/usr/local/include" >> ~/.bash_profile
echo "export LIBRARY_PATH=/usr/local/lib" >> ~/.bash_profile

source ~/.bash_profile

```

[ref](https://medium.com/@alexanderbussan/getting-started-with-google-test-on-os-x-a07eee7ae6dc)

# install gtest on linux

```bash
sudo apt-get install libgtest-dev
```


To run the test suite...

# Interface Description

## COMMON

### Send

sends to connected client or server. Catches connection errors

-> void send(char*, int)
-> void send(const std::string&)
-> send(vector<std::string>)


### Read

Read received messages from client or server

-> std::string readBlocking()
-> std::string readNonBlocking()

### Start

### Stop

## SERVER-SPECIFIC

- multi client support

## CLIENT-SPECIFIC

- auto reconnect on disconnection

# TODO

- [ ] Create interface over server and client
- [ ] add doxy comments
- [ ] add tests
- [ ] clean up common functions -> UTF8 keygen, ASCII char array generator
- [ ] maybe move everything to byte arrays instead of char vectors
- [ ] test client send, server receive
- [ ] test server send, client receive
