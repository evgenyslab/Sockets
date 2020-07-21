#include "common_header.h"

#define MAX_QUEUE_SIZE 10

/*
 * socket Interface Namespace
 * */
namespace SocketInterface{

    struct __message{
        long timeReceived = -1;
        std::string originID;
        std::string data;
    };

    /*
     *
     * */
    class Socket{

    private:
        void* _receive(){

        };

    public:
        ~Socket()=default;
        virtual void start()=0;
        virtual void send(char* , int)=0;
        virtual void send(const std::string&);
        virtual std::string readBlocking()=0;
        virtual std::string readNonBlocking()=0;
        virtual void stop()=0;

        //TODO: start()
        //TODO: void* send(char* messageByteArray, int numBytes)
        //TODO: recv()
        //TODO: stop()

    };

};