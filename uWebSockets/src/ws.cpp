#include <uWS/uWS.h>
#include <iostream>
#include <thread>

using namespace uWS;


struct ct{
    uWS::WebSocket<uWS::SERVER> *hptr = nullptr;
    bool connected = false;
};

void * fun(void *ptr){
    auto *context = (ct*) ptr;
    printf("I've made it into my thread!\n");
    while(!context->connected){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    context->hptr->send("why hello there\n");
    pthread_exit(nullptr);
}

int main() {
    uWS::Hub h;
    ct localContext;

    h.onConnection([&localContext](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
                         std::cout << "A client connected" << std::endl;
                         localContext.hptr = ws;
                         localContext.connected = true;
                     }
    );
    h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
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



    if (h.listen("0.0.0.0",13049)) {
        std::cout << "Listening on port 13049" << std::endl;
        // create op thread...
        pthread_t tid;
        pthread_create(&tid, nullptr, fun, &localContext);
        h.run();
        pthread_join(tid, nullptr);
    }
}