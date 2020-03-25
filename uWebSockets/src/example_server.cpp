#include <uWServer_b.h>
#include <thread>
#include <iostream>
#include <random>

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
        else if(!cmd.empty()){
            server.send(cmd);
        }
    }

    return 0;
}