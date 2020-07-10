#include "common_header.h"
#include "server.h"
#include "client.h"


int main(int argc,char *argv[]) {


    // create client:
    Client c1(7272, true);
    c1.start_t();

    while(1){
        // do nothing
        std::this_thread::sleep_for(std::chrono::seconds(1));
//        c1.send_t("blah blah blha");
    }

}