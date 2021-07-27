#ifndef CLIENT_HPP
# define CLIENT_HPP
#include "RawRequestReader.hpp"

class Client
{
    private:
        int socket_fd;
        // RawRequestReader reader;
        // Request request;
        // Response response;
        unsigned long last_time;
        int status;
    public:
        Client();
        ~Client();
        Client(const Client& c);

        Client& operator= (const Client& c);
};

#endif