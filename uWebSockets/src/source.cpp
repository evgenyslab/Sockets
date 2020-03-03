//
//

#include <uWServer.h>
#include <iostream>
//


int main() {

    uWServer::uWServer uS;

    uS.run();


    // dummy string for grabbing keyboard input:
    std::string cmd;
    // initialize exit condition:
    bool exit = false;
    // wait for a moment while uwebsockets starts...
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::vector<char> f;
    while(!exit){
        printf(">> ");
        std::getline (std::cin,cmd);
        if (cmd == "exit"){
            exit = true;
            continue;
        }else{
            // send message to connected client (browser)
            uS.send(cmd);
        }
    }


}
