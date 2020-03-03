//
//

#include "uWServer.h"

using namespace uWS;

namespace uWServer{

    uWServer::uWServer(int _port):port(_port) {
        //create object hub instance:
        this->h =  new uWS::Hub();
    };

    void uWServer::run() {
        pthread_create(&this->_t, nullptr, this->runThread, this);
    };

    void uWServer::send(const std::string msg) {
        if(!this->connected){
            printf("No Clients connected, skipping send!\n");
            return;
        }
        // create JSON string:
        nlohmann::json j = {"message", msg};
        std::string jmsg = "{\"message\": \"" + msg + "\"}";
        // send to all clients:
        for(auto cptr: this->connections)
            cptr->send(j.dump().c_str());
    }

    void uWServer::send(std::vector<char> *cArray) {
        // send binary char array
        if(!this->connected){
            printf("No Clients connected, skipping send!\n");
            return;
        }
        std::string msg(cArray->begin(), cArray->end());
        // send to all clients:
        for(auto cptr: this->connections)
            cptr->send(msg.c_str(),msg.size(),OpCode::BINARY);
    }

    void uWServer::send(const nlohmann::json &jobj) {
        if(!this->connected){
            printf("No Clients connected, skipping send!\n");
            return;
        }

        // send to all clients:
        for(auto cptr: this->connections)
            cptr->send(jobj.dump().c_str());
    }

    void uWServer::_run(){
        // do actual work here...
        // does this mean the hub is a server or a client?
        if(this->h == nullptr)
            int k;
        this->h->onConnection([this](uServer ws, uWS::HttpRequest req) {
                                          std::cout << "A client connected" << std::endl;
                                          // seems like theres a new pointer per connected client; need to manage this better.
                                          pthread_mutex_lock(&this->_lock);
                                          this->connections.emplace_back(ws);
                                          this->connected = true;
                                          pthread_mutex_unlock(&this->_lock);
                                      }
        );

        // TOOD: implement this to handle clients correctly...
        this->h->onDisconnection([this](uServer ws, int code, char *message, size_t length) {
            std::cout << "CLIENT CLOSE: " << code << std::endl;
            std::vector<uServer>::iterator it;
            it = find (this->connections.begin(), this->connections.end(), ws);
            if (it != this->connections.end()){

                pthread_mutex_lock(&this->_lock);
                this->connections.erase(it);
                // set connection state based on how many connected clients there are:
                this->connected = this->connections.size() > 0;
                pthread_mutex_unlock(&this->_lock);
                printf("Client removed!\n");
                if (this->connections.size()==0){
                    printf("All Clients disconnected!\n");
                }
            }else{
                printf("client NOT found in array\n");
            }
        });

        this->h->onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
            const std::string s = "<h1>Hello world!</h1>";
            if (req.getUrl().valueLength == 1)
            {
                res->end(s.data(), s.length());
            }
            else
            {
                // i guess this should be done more gracefully?
                res->end(nullptr, 0);
            }
        });

        this->h->onMessage([this](uServer ws, char *message, size_t length, uWS::OpCode opCode){
            // could match ws to client list if we really wanted to...
            // could push message into local context work queue.
            std::string rmsg(message, length);
            nlohmann::json j;
            bool jsondata = false;
            try{
                j = nlohmann::json::parse(rmsg);
                jsondata = true;
            }catch(const std::exception& e){
                // could not parse JSON
                jsondata = false;
            }
            // KNOWN ISSUE OF HANDLING '\n' in MESSAGE!
            if (jsondata){

                printf("\nJSON Message Received:\n");
                std::cout << j.dump(2) << std::endl;
                this->send(j.dump());
            }else{
                printf("\nMessage Received: <%s>\n", rmsg.c_str());
                this->send(rmsg);
            }

        });



        if (this->h->listen("0.0.0.0",this->port)) {
            std::cout << "Listening on port " << this->port << std::endl;
            this->h->run();
        }
        pthread_exit(nullptr);
    };

    void uWServer::stop(){
        pthread_kill(this->_t, 0);
    }



}