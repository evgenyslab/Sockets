#include <uWS/uWS.h>
#include <iostream>
#include <algorithm>
#include <thread>

using namespace uWS;

uint64_t now(){
    return (std::chrono::duration_cast<std::chrono::nanoseconds >(std::chrono::system_clock::now().time_since_epoch())).count();
}

typedef uWS::WebSocket<uWS::SERVER>* uServer;

struct ct{
    uWS::Hub *h= nullptr;
    std::vector<uServer> hptr = {};
    bool connected = false;
    pthread_mutex_t _lock = PTHREAD_MUTEX_INITIALIZER;
};

void * fun(void *ptr){
    auto *context = (ct*) ptr;
    printf("I've made it into my thread!\n");
    while(!context->connected){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    context->hptr[0]->send("why hello there\n");
    pthread_exit(nullptr);
}

void * webserver(void *ptr){


    ct * localContext = (ct*) ptr;
    // does this mean the hub is a server or a client?
    localContext->h->onConnection([localContext](uServer ws, uWS::HttpRequest req) {
        std::cout << "A client connected" << std::endl;
        // seems like theres a new pointer per connected client; need to manage this better.
        pthread_mutex_lock(&localContext->_lock);
        localContext->hptr.emplace_back(ws);
        localContext->connected = true;
        pthread_mutex_unlock(&localContext->_lock);
    }
    );

    // TOOD: implement this to handle clients correctly...
    localContext->h->onDisconnection([localContext](uServer ws, int code, char *message, size_t length) {
        std::cout << "CLIENT CLOSE: " << code << std::endl;
        std::vector<uServer>::iterator it;
        it = find (localContext->hptr.begin(), localContext->hptr.end(), ws);
        if (it != localContext->hptr.end()){

            pthread_mutex_lock(&localContext->_lock);
            localContext->hptr.erase(it);
            // set connection state based on how many connected clients there are:
            localContext->connected = localContext->hptr.size() > 0;
            pthread_mutex_unlock(&localContext->_lock);
            printf("Client removed!\n");
            if (localContext->hptr.size()==0){
                printf("All Clients disconnected!\n");
            }
        }else{
            printf("client NOT found in array\n");
        }
    });

    localContext->h->onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
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



    if (localContext->h->listen("0.0.0.0",13049)) {
        std::cout << "Listening on port 13049" << std::endl;
        // create op thread...
        pthread_t tid;
        pthread_create(&tid, nullptr, fun, localContext);
        localContext->h->run();
        pthread_join(tid, nullptr);
    }
    pthread_exit(nullptr);
}

void *send_heartbeat(void *ptr){
    auto *context = (ct*) ptr;
    while(1){
        if(context->connected){
            auto t = now();
            char tt[19];
            sprintf(tt, "%ld",t);
            // create JSON string:
            std::string jmsg = "{\"epoch\": \"" + std::string(tt) + "\"}";
            // send to all clients:
            for(auto cptr: context->hptr)
                cptr->send(jmsg.c_str());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }

    pthread_exit(nullptr);

}

void send(std::string msg, void *ptr){
    auto *context = (ct*) ptr;
    if(!context->connected){
        printf("No Clients connected, skipping send!\n");
        return;
    }
    // create JSON string:
    std::string jmsg = "{\"message\": \"" + msg + "\"}";
    // send to all clients:
    for(auto cptr: context->hptr)
        cptr->send(jmsg.c_str());
}

int main() {

    // instantiate object...
    ct localContext;
    // create local uWS hub object:
    uWS::Hub h;
    // attach hub to local context object:
    localContext.h = &h;
    // create websever thread
    pthread_t ws;
    // run webserver thread (handles client connections/disconnections):
    pthread_create(&ws, nullptr, webserver, &localContext);
    // create clock thread (sends clock info to client):
    pthread_t clock;
    // run the clock thread:
    pthread_create(&clock, nullptr, send_heartbeat, &localContext);

    // dummy string for grabbing keyboard input:
    std::string cmd;
    // initialize exit condition:
    bool exit = false;
    // wait for a moment while uwebsockets starts...
    std::this_thread::sleep_for(std::chrono::seconds(2));
    while(!exit){
        printf(">> ");
        std::getline (std::cin,cmd);
        if (cmd == "exit"){
            exit = true;
            pthread_kill(ws, 0);
            pthread_kill(clock,0);
            continue;
        }else{
            // send message to connected client (browser)
            send(cmd, &localContext);
        }
    }
}