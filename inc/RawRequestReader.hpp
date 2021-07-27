#ifndef RAWREQUESTREADER_HPP
# define RAWREQUESTREADER_HPP

#include <string>

class RawRequestReader
{
    friend class Client;
    private:
        std::string raw_request;
        // Request request;
    public:
        void read_startHead();
        void read_body();
};

#endif