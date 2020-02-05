#include "common_header.h"
#include "server.h"
#include "client.h"


int main(int argc,char *argv[]) {


    // create client:
    client c1(7272, true);
    c1.start();

    while(1){
        // do nothing
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

}