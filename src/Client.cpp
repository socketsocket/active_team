#include "Client.hpp"

Client::Client(int server_socket, unsigned int port)
{
	// accept(listen_fd,(struct sockaddr *)&client_addr, &addrlen);
	this->socket_fd = accept(server_socket, NULL, NULL); // client address가 필요 없어서 NULL!
	if (this->socket_fd == -1)
		throw ;
	std::cout << "accept new client: " << client_socket << std::endl;
	fcntl(client_socket, F_SETFL, O_NONBLOCK);

	EV_SET(&temp_event, client_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
	change_list.push_back(temp_event);
	EV_SET(&temp_event, client_socket, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
	change_list.push_back(temp_event);

	client_data[client_socket] = "";
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