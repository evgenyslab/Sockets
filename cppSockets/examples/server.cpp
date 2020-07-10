#include "common_header.h"
#include "server.h"
#include "client.h"


int main(int argc,char *argv[]) {

    Server s(7272);
    s.open();
    s.start();


    bool exit = false;
    // spam client
    while(!exit){
        // hold for clients
        if (!s.hasClients()){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        else{
            std::string inputText;
            getline(std::cin, inputText);
            if (inputText == "q")
                exit = true;
            else
                s.write(inputText);
        }

    }
    s.stop();
}