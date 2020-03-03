//
//

#ifndef WEBSOCKETS_TESTING_UWSERVER_H
#define WEBSOCKETS_TESTING_UWSERVER_H

#include <uWS/uWS.h>
#include <iostream>
#include <algorithm>
#include <thread>
#include <fstream>
#include <nlohmann/json.hpp>

typedef uWS::WebSocket<uWS::SERVER>* uServer;

using namespace uWS;

namespace uWServer{

    class uWServer {
        int port = 0;
        std::vector<uServer> connections = {};
        bool connected = false;
        pthread_mutex_t _lock = PTHREAD_MUTEX_INITIALIZER;
        pthread_t _t;
        uWS::Hub *h = nullptr;


        // helper function
        static void *runThread(void *context) {
            ((uWServer *) context)->_run();
            return nullptr;
        }


    public:

        uWServer(int _port = 11111);

        ~uWServer(){
            this->stop();
        };

        void run();

        void stop();

        void send(const std::string msg);


    private:

        // run thread definition, class-based:
        void _run();

    };

};


#endif //WEBSOCKETS_TESTING_UWSERVER_H
