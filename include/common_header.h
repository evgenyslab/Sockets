#pragma once
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <thread>
#include <random>
#include <map>

#define PORT        unsigned short
#define SOCKET    int
#define HOSTENT  struct hostent
#define SOCKADDR    struct sockaddr
#define SOCKADDR_IN  struct sockaddr_in
#define ADDRPOINTER  unsigned int*
#define INVALID_SOCKET_m -1
#define SOCKET_ERROR   -1
#define TIMEOUT_M       200000
#define NUM_CONNECTIONS 10
#define ID_LENGTH 12
#define BYTES_TO_READ 4096*16

uint64_t getNow(){
    return (std::chrono::duration_cast<std::chrono::nanoseconds >(std::chrono::system_clock::now().time_since_epoch())).count();
}

void randomString(char *buf, int l){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(48,125);
    for (int i=0; i<l; ++i) {
        buf[i] = (char) distribution(generator);
    }
}

std::vector<unsigned char> vencode(unsigned int paramInt)
{
    std::vector<unsigned char> arrayOfByte(4);
    for (int i = 0; i < 4; i++)
        arrayOfByte[3 - i] = (paramInt >> (i * 8));
    return arrayOfByte;
}

void decode(unsigned char *bytes, unsigned int &n){
    n = static_cast<unsigned int>(
            (unsigned char)(bytes[0]) << 24 |
            (unsigned char)(bytes[1]) << 16 |
            (unsigned char)(bytes[2]) << 8 |
            (unsigned char)(bytes[3])
    );
}

struct message{
    unsigned long _time;
    std::string msg;
};