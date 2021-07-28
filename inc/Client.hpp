#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <string>
#include "RawRequestReader.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Client
{
    private:
        int socket_fd;
        // RawRequestReader reader;
        Request request;
        Response response;
        unsigned long last_time;
        int status;
    public:
        Client();
        ~Client();
        Client(const Client& c);

        Client& operator= (const Client& c);

        void readRequest();
};

#endif