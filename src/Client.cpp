#include "Client.hpp"

Client::Client()
{
	socket_fd = -1;
	status = 0; // request 받을준비중
	last_time = 0;

}

Client::~Client()
{

}

Client::Client(const Client &c)
{

}

Client& Client::operator= (const Client& c)
{

}