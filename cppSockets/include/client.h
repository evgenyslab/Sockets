#include "common_header.h"

#define MAX_QUEUE_SIZE 10

template<class T>
struct recvConfig{
    int sock;
    T *q;
    recvConfig():sock(-1),q(nullptr){};
    recvConfig(int _sock):sock(_sock), q(nullptr){};
    recvConfig(int _sock, T * _q):sock(_sock),q(_q){};
};

class Client{
private:

    bool connected = false;
    bool autoReConnect = false;
    int port;
    int sock;
    char ip[INET_ADDRSTRLEN];
    char *ID = new char[ID_LENGTH];
    std::deque<message> mqueue = {};
    recvConfig<std::deque<message>> _receiver = {0,&mqueue};
    pthread_mutex_t mutexQueue = PTHREAD_MUTEX_INITIALIZER;

    pthread_t thread_operator;
    struct sockaddr_in server_sock;

    // Generic Write to Client
    int _write( char *s, int len)
    {
        if (len < 1) { len = strlen(s); }
        {
            try
            {
                int retval = write(sock, s, len);
                return retval;
            }
            catch (int e)
            {
                std::cout << "An exception occurred. Exception Nr. " << e << '\n';
            }
        }
        return -1;
    }


    void *_recv(void *lsock){

        int their_sock = *((int *)lsock);
        char *msg = new char[BYTES_TO_READ];
        int len;
        unsigned int mlen;

        long bytes_to_read = 0;

        std::vector<char> vmsg;

        while((len = recv(their_sock,msg, BYTES_TO_READ,0)) > 0) {
            // push msg into char vector
            vmsg.insert(vmsg.end(), msg, msg+len);
            // if bytes_to_read == 0 && vmsg.size() > 3, pop out first 4 bytes, and decode message length
            if (bytes_to_read==0 & vmsg.size()>3){
                char * buf = new char[4];
                std::copy(vmsg.begin(), vmsg.begin()+4, buf);
                decode(reinterpret_cast<unsigned char *>(buf), mlen);
                bytes_to_read = static_cast<long>(mlen);
                vmsg.erase(vmsg.begin(), vmsg.begin()+4);
            }
            // expecting message, theres enough in buffer
            if (bytes_to_read>0 & vmsg.size() >= bytes_to_read){
                // put message into workable queue:
                message m = {getNow(), std::string(vmsg.begin(),vmsg.begin()+bytes_to_read)};
                pthread_mutex_lock(&mutexQueue);
                // cleanup message queue:
                if (mqueue.size()>= MAX_QUEUE_SIZE)
                    mqueue.pop_front();
                mqueue.emplace_back(m);
                pthread_mutex_unlock(&mutexQueue);
                // erase message bytes from char array:
                vmsg.erase(vmsg.begin(), vmsg.begin()+bytes_to_read);
                // reset:
                bytes_to_read = 0;
            }
        }
        printf("Server Disconnected\n");
        return nullptr;
    }


    void _connect(){
        printf("Client Attempting to connect on port: %d\n", port);
        while(!connected){
            // this should be in a loop until connection is established...
            if(connect(sock,(struct sockaddr *)&server_sock,sizeof(server_sock)) < 0) {
                close(sock);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                _init();
            }else{
                // able to connect...
                char header[128];
                sprintf(header,"Hey there look at me I'm mr meeseeks!\nMy ID is: %s\n", this->ID);
                _write(header, sizeof(header));
                char msg[4096];// this shoudl be receive with timeout...
                // try this a couple of times, if it doent work... try to reconnect...
                int k = 0;
                int len = -1;
                for (k=0; k<10;k++) {
                    len = recv(sock, msg, sizeof(msg), MSG_DONTWAIT);
                    if (len>0){
                        printf(msg);
                        // TODO Verify handshake
                        connected = true;
                        break;
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds (10));
                }
                if (k==10){
                    close(sock);
                    std::this_thread::sleep_for(std::chrono::milliseconds (10));
                    _init();
                }
            }
        }

        // create connection to server
        inet_ntop(AF_INET, (struct sockaddr *)&server_sock, ip, INET_ADDRSTRLEN);
        printf("connected to %s, start chatting\n",ip);
    };

    void Operator(){
        // operation of thread, either allow reconnection, or dont
        bool run = true;
        while(run){
            _connect();
            _recv(&sock);
            this->connected = false;
            // if I'm here, means I disconnected...
            if (!this->autoReConnect){
                printf("Existing\n");
                run = false;
            }
        }
    }

    // helper method to create thread from instance...
    static void * _op (void * context){
        ((Client *)context)->Operator();
        return nullptr;
    }

public:

    Client(int lport=7272, bool reconnect=false): port(lport), autoReConnect(reconnect){
        randomString(ID, ID_LENGTH);
        _init();
    };

    ~Client(){
        // todo: disconnect any open socket...
        stop();
        std::cout << "Client cleaned and closed\n";
    };

    void _init(){
        // set up Client's socket:
        sock = socket(AF_INET,SOCK_STREAM,0);
        // set up connection to server's socket:
        memset(server_sock.sin_zero,'\0',sizeof(server_sock.sin_zero));
        server_sock.sin_family = AF_INET;
        server_sock.sin_port = htons(port);
        server_sock.sin_addr.s_addr = inet_addr("127.0.0.1");
    }

    void send(const std::string &msg){
        // send a message by packing the message length into 4bytes then msg:
        unsigned int s = static_cast<unsigned int> (msg.size()) ;
        // write 8 byte message length:
        auto bytes_to_decode = vencode(s);
        std::string m(bytes_to_decode.begin(), bytes_to_decode.end());
            // write actual message [[4 bytes length][msg]]:
            std::string smsg = m + msg;
            if(_write(const_cast<char *>(smsg.data()), smsg.size()) != smsg.size()){
                // something went wrong!
                // server maybe disconnected? - this would be captured by the listen...
            }
    }

    void start(){
        // create op thread...
        pthread_create(&thread_operator,NULL,_op,this);
    }

    void stop(){
        pthread_kill(thread_operator, 0);
        close(sock);
    }


};