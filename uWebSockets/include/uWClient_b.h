/*
 *
 * server
 *
 * thread -> set up + poll in cycle // runs indefinately until quit message
 * thread -> read from queue, write to queue
 *
 * */

#include <uWS.h>
#include <thread>
#include <deque>
#include <iostream>

#define MAX_MESSAGE_QUEUE 100

using namespace uWS;

class uWClient_b{
private:
    // run thread
    pthread_t _tid;
    // received queue
    std::deque<std::string> rxqueue;
    pthread_mutex_t _rxmutex = PTHREAD_MUTEX_INITIALIZER;
    // send queue
    std::deque<std::string> txqueue;
    pthread_mutex_t _txmutex = PTHREAD_MUTEX_INITIALIZER;
    // port
    int port = 0;
    //
    bool connected = false;

    // functions:
    // helper function
    static void *__run__(void *context) {
        ((uWClient_b *) context)->_run();
        return nullptr;
    }
    // run function:
    void _run(){
        // create hub object local to thread:
        uWS::Hub h;

        h.onConnection([this](uWS::WebSocket<uWS::CLIENT>* ws, uWS::HttpRequest req) {
           std::cout << "Client connected to Server on port: " << this->port << std::endl;
           // seems like theres a new pointer per connected client; need to manage this better.
            printf("%s\n",req.headers->value);
           this->connected = true;
           }
        );

        h.onDisconnection([this](uWS::WebSocket<uWS::CLIENT>* ws, int code, char *message, size_t length) {
            std::cout << "CLIENT Disconnected from Server with code: " << code << std::endl;
            this->connected = false;
        });


        h.onMessage([this](uWS::WebSocket<uWS::CLIENT>* ws, char *message, size_t length, uWS::OpCode opCode){
            // could push message into local context work queue.
            std::string rmsg(message, length);
            // lock queue
            pthread_mutex_lock(&this->_rxmutex);
            // place message:
            this->rxqueue.emplace_back(rmsg);
            // unlock queue
            pthread_mutex_unlock(&this->_rxmutex);
        });

        // TODO: loop this so reconnection attempts if not connected to server.
        h.connect("ws://127.0.0.1:" + std::to_string(this->port), (void *) 1);
        // not connected until run or poll is called...
        printf("Starting Client\nConnection status: %d\n", this->connected);
        h.run();
        bool exit = false;
        while(!exit){
            // need to poll and check for connect & try to reconnect?

        }
        h.run(); // <- blocking call

    }

public:

    uWClient_b(int port):port(port){};
    ~uWClient_b() = default;

    void config(){

    };
    // creates and runs a thread with a hub based on config
    void run(){
        pthread_create(&this->_tid, nullptr, this->__run__, this);
    };
    void stop(){
        pthread_kill(this->_tid, 0);
    };
    // send message
    void send(const std::string rmsg){
        // lock queue
        pthread_mutex_lock(&this->_txmutex);
        // check size of queue
        if(this->txqueue.size()==MAX_MESSAGE_QUEUE)
            this->txqueue.pop_front(); // remove first msg
        this->txqueue.emplace_back(rmsg);
        pthread_mutex_unlock(&this->_txmutex);
    };
    // reads message from queue
    void read(std::string &ret){
        // lock queue
        pthread_mutex_lock(&this->_rxmutex);
        if(!this->rxqueue.empty()){
            ret = this->rxqueue.front();
            this->rxqueue.pop_front();
        }
        pthread_mutex_unlock(&this->_rxmutex);
    };
    // reads all message from queue
    void read(std::vector<std::string> &ret){
        // lock queue
        pthread_mutex_lock(&this->_rxmutex);
        while(!this->rxqueue.empty()){
            std::string r;
            this->read(r);
            ret.emplace_back(r);
            this->rxqueue.pop_front();
        }
        pthread_mutex_unlock(&this->_rxmutex);
    };

};