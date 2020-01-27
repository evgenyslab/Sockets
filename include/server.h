#include "common_header.h"

class server{
    SOCKET sock;
    fd_set master;
    int timeout;
    std::vector<int> clients;
    pthread_t thread_listen;
    pthread_mutex_t mutex_client = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_cout = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_writer = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_msg = PTHREAD_MUTEX_INITIALIZER;
    int port;
    char *ID = new char[ID_LENGTH];


    // Generic Write to client
    static int _write(int sock, char *s, int len)
    {
        if (len < 1) { len = strlen(s); }
        {
            try
            {
                int retval = ::send(sock, s, len, 0x4000);
                return retval;
            }
            catch (int e)
            {
                std::cout << "An exception occurred. Exception Nr. " << e << '\n';
            }
        }
        return -1;
    }

    static int _read(int socket, char* buffer)
    {
        int result;
        result = recv(socket, buffer, 4096, 0);
        if (result < 0 )
        {
            std::cout << "An exception occurred. Exception Nr. " << result << '\n';
            return result;
        }
        std::string s = buffer;
        buffer = (char*) s.substr(0, (int) result).c_str();
        return result;
    }

    /* Help create a thread from a class member function through
     * a static class function
     *
     * */
    static void* listen_Helper(void* context)
    {
        ((server *)context)->Listener();
        return nullptr;
    }


public:

    std::vector<char> mymsg;

    explicit server(int port = 0)
            : sock(INVALID_SOCKET_m)
            , timeout(TIMEOUT_M)
            , port(port)
    {
        signal(SIGPIPE, SIG_IGN);
        randomString(ID, ID_LENGTH);
        FD_ZERO(&master);
    }

    ~server()
    {
        release();
    }

    bool release()
    {
        if (sock != INVALID_SOCKET_m)
            shutdown(sock, 2);
        sock = (INVALID_SOCKET_m);
        std::cout << "Server cleaned and closed\n";
        return false;
    }

    bool open()
    {
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        SOCKADDR_IN address{};
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        // EN --- trying to reuse port cleanly
        int optval = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
        //- ^^^^^^^^^^^^^^^^^^^^^^^^
        if (::bind(sock, (SOCKADDR*)&address, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
        {
            std::cerr << "error : couldn't bind sock " << sock << " to port " << port << "!" << std::endl;
            return release();
        }
        if (listen(sock, NUM_CONNECTIONS) == SOCKET_ERROR)
        {
            std::cerr << "error : couldn't listen on sock " << sock << " on port " << port << " !" << std::endl;
            return release();
        }
        FD_SET(sock, &master);
        return true;
    }

    bool isOpened()
    {
        return sock != INVALID_SOCKET_m;
    }

    void start(){
        std::cout << "Starting server at: " << port << "\n";
        pthread_mutex_lock(&mutex_writer);
        // createa a thread that listens for incoming connections:
        pthread_create(&thread_listen, nullptr, this->listen_Helper, this);
    }

    void stop(){
        pthread_join(thread_listen, nullptr);
        this->release();
    }

    void write(const std::string &msg){
        // This needs to write 2 messages:
        // 8 byte message of length == n
        // n byte message
        if (!clients.empty() && this->isOpened()){

            int ret = 0;
            unsigned int s = static_cast<unsigned int> (msg.size()+20) ;
            // write 8 byte message length:
            auto bytes_to_decode = vencode(s);
            std::string m(bytes_to_decode.begin(), bytes_to_decode.end());
            std::vector<int> clients_to_remove;
            // for each client...
            for (auto it =  clients.begin(); it != clients.end(); ++it)
            {
                // write actual message [[4 bytes length][msg]]:
                std::string smsg = m + std::to_string(getNow()) + " " + msg;
                if(_write(*it, const_cast<char *>(smsg.data()), smsg.size()) != smsg.size()){
                    // lost connection, add client to remove list...
                    clients_to_remove.emplace_back(*it);
                    continue;
                }
            }

            for (auto &k : clients_to_remove){
                pthread_mutex_lock(&mutex_client);
                // remove client from list...
                std::vector<int>::iterator it;
                it = find (clients.begin(), clients.end(), k);
                if (it != clients.end())
                {
                    std::cerr << "kill client " << k << std::endl;
                    clients.erase(std::remove(clients.begin(), clients.end(), k));
                    ::shutdown(k, 2);
                }
                pthread_mutex_unlock(&mutex_client);
            }

            pthread_mutex_unlock(&mutex_msg);
        }
    }


private:

    void Listener(){

        char header[128];
        sprintf(header,"Hey there look at me I'm MASTER meeseeks!\nMy ID is: %s\n", this->ID);
        fd_set rread;
        SOCKET maxfd;
        pthread_mutex_unlock(&mutex_writer);
        printf("%lld: Starting Listener\n",getNow());
        while (true)
        {
            rread = master;
            struct timeval to = { 0, timeout };
            maxfd = sock + 1;

            int sel = select(maxfd, &rread, nullptr, nullptr, &to);
            if (sel > 0) {
                for (int s = 0; s < maxfd; s++)
                {
                    if (FD_ISSET(s, &rread) && s == sock)
                    {
                        int         addrlen = sizeof(SOCKADDR);
                        SOCKADDR_IN address = { 0 };
                        SOCKET      client = accept(sock, (SOCKADDR*)&address, (socklen_t*)&addrlen);
                        if (client == SOCKET_ERROR)
                        {
                            std::cerr << "error : couldn't accept connection on sock " << sock << " !" << std::endl;
                            return;
                        }
                        maxfd = (maxfd>client ? maxfd : client);
                        pthread_mutex_lock(&mutex_cout);
                        std::cout << "new client " << client << std::endl;
                        char headers[4096] = "\0";
                        int readBytes = _read(client, headers);
                        // TODO Verify handshake, create pthread for this client...
                        std::cout << headers;
                        pthread_mutex_unlock(&mutex_cout);
                        pthread_mutex_lock(&mutex_client);
                        _write(client, header, sizeof(header));
                        clients.push_back(client);
                        pthread_mutex_unlock(&mutex_client);
                    }
                }
            }
            usleep(10);
        }
    };
};