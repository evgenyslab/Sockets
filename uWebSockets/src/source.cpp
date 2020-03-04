//
//

#include <uWServer.h>
#include <iostream>
//

std::vector<char> readFile(const char* filename)
{
    // open the file:
    std::ifstream file(filename, std::ios::binary);

    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<char> vec;
    vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(),
               std::istream_iterator<char>(file),
               std::istream_iterator<char>());

    return vec;
}

int main() {

    uWServer::uWServer uS(13049);

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
        }else if (cmd == "read") {
            // try to read from uS buffer
            auto r = uS.read();
            printf("received:\n%s\n", r.c_str());
        }else if (cmd =="load") {
            // can't use json with images in any way, UTF-8 decoding breaks it.
            // TODO: look at using msgpack; browers should support
            auto I = readFile("/Users/en/Git/Sockets/uWebSockets/test.jpeg");
            uS.send(&I);
        }else{
                // send message to connected client (browser)
                nlohmann::json j = {{"message", cmd}};
                uS.send(j);
        }
    }
}
