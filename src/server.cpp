#include "common_header.h"
#include "server.h"
#include "client.h"

std::string random_string(std::size_t length)
{
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<int> distribution(48,125);

    std::string random_string;

    for (std::size_t i = 0; i < length; ++i)
    {
        random_string += (char) distribution(generator);

    }

    return random_string;
}

int main(int argc,char *argv[]) {

    server s(7272);
    s.open();
    s.start();

    // spam client
    while(1){
        std::string msg = random_string(2400000);
        std::to_string(getNow());
        std::string smsg = std::to_string(getNow()) + ": " + msg;
        s.write(smsg);
        std::this_thread::sleep_for(std::chrono::milliseconds (100));
    }
    s.stop();
}