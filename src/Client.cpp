#include "Client.hpp"

Client::Client(int server_socket, unsigned int port)
{
	this->socket_fd = accept(server_socket, NULL, NULL); // client address가 필요 없어서 NULL!
	if (this->socket_fd == -1)
		throw clientException("accept error: " + string(stderror());

	fcntl(this->socket_fd, F_SETFL, O_NONBLOCK);
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