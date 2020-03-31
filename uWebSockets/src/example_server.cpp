#include <uWServer_b.h>
#include <thread>
#include <iostream>
#include <random>
#include <msgpack.hpp>

/* Try with new single
 * TODO: need to speed test this somehow
 * */

std::string randomString(std::size_t length)
{
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<int> distribution(0,255);

    std::string random_string;

    for (std::size_t i = 0; i < length; ++i)
    {
        random_string += (char) distribution(generator);

    }

    return random_string;
}

int main(){

    uWServer_b server(13049);
    server.run();

    msgpack::sbuffer streamBuffer;  // stream buffer
    // create a key-value pair packer linked to stream-buffer
    msgpack::packer<msgpack::sbuffer> packer(&streamBuffer);
    // create key-value map definition, n = number of items in map...
    packer.pack_map(1);                                         // MESSAGE 1
    // Populate:
    // key
    packer.pack(std::string("x"));

    streamBuffer.data();

    bool exit = false;
    // wait for a moment while uwebsockets starts...
    while(!exit) {
        printf(">> ");
        std::string cmd;
        std::getline(std::cin, cmd);
        if (cmd.substr(0,4) =="rand") {
            // get length of random:
            int l;
            if (cmd.size()>4)
                l = atoi(cmd.substr(4,cmd.size()-1).c_str());
            else
                l = 260000;
            auto I = randomString(l);
            server.send(I);
        }
        else if(cmd == "exit")
            exit = true;
        else if(cmd.substr(0,3) =="mpk"){
            int l;
            if (cmd.size()>4)
                l = atoi(cmd.substr(4,cmd.size()-1).c_str());
            else
                l = 10000;
            auto I = randomString(l);
            streamBuffer.clear();

            packer.pack_map(1);
            packer.pack("data");
//            packer.pack_bin(I.size());
            packer.pack_bin_body(I.data(), I.size());
            server.c_send(streamBuffer.data(), streamBuffer.size()); // streamsize isnt correct yet...
        }
        else if(!cmd.empty()){
            server.send(cmd);
        }
    }

    return 0;
}