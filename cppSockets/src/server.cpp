#include "common_header.h"
#include "server.h"
#include "client.h"


int main(int argc,char *argv[]) {

    Server s(7272);
    s.open();
    s.start();

    // spam client
    while(1){
//        std::string msg = randomString(2400000);
//        std::to_string(getNow());
//        std::string smsg = std::to_string(getNow()) + ": " + msg;
//        s.write(smsg);
        std::this_thread::sleep_for(std::chrono::milliseconds (100));
    }
    s.stop();
}