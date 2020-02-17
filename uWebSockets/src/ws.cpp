#include <uWS/uWS.h>
#include <iostream>
#include <thread>

using namespace uWS;


struct ct{
    uWS::Hub *h= nullptr;
    std::vector<uWS::WebSocket<uWS::SERVER>*> hptr = {};
    bool connected = false;
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

    localContext->h->onConnection([localContext](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        std::cout << "A client connected" << std::endl;
        // seems like theres a new pointer per connected client; need to manage this better.
        localContext->hptr.emplace_back(ws);
        localContext->connected = true;
    }
    );

    // TOOD: implement this to handle clients correctly...
//    localContext->h->onDisconnection([localContext](uWS::WebSocket<uWS::SERVER> *ws) {
//         std::cout << "A client connected" << std::endl;
//         // seems like theres a new pointer per connected client; need to manage this better.
//         localContext->hptr.emplace_back(ws);
//         localContext->connected = true;
//     }
//     );

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

void send(std::string msg, void *ptr){
    auto *context = (ct*) ptr;
    while(!context->connected){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    // create JSON string:
    std::string jmsg = "{\"message\": \"" + msg + "\"}";
    context->hptr[0]->send(jmsg.c_str());
}

int main() {

    // instantiate object...
    ct localContext;
    uWS::Hub h;
    localContext.h = &h;
    // create websever thread
    pthread_t ws;
    pthread_create(&ws, nullptr, webserver, &localContext);

    std::string cmd;
    bool exit = false;
    // wait for a moment while uwebsockets starts...
    std::this_thread::sleep_for(std::chrono::seconds(2));
    while(!exit){
        printf(">> ");
        std::getline (std::cin,cmd);
        if (cmd == "exit"){
            exit = true;
            continue;
        }else{
            // send message to connected client (browser)
            send(cmd, &localContext);
        }
    }

    pthread_join(ws, nullptr);


}