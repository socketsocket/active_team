#include "Client.hpp"

Client::Client(int socket_fd, PortManager *port_manager)
	: FDHandler(socket_fd),
	  reader(socket_fd),
	  processor(port_manager),
	  writer(NULL)
{}

Client::~Client()
{
	while (dialogues.empty() == false)
	{
		delete dialogues.front();
		dialogues.pop();
	}
}
