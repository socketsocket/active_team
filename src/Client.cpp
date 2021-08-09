#include <sys/socket.h>

#include "Client.hpp"

/* static */

static int
	openSocket(int server_socket)
{
	int	client_socket = accept(server_socket, NULL, NULL);

	if (client_socket == -1)
		throw SystemCallError("accept");
	return (client_socket);
}

/* public */

Client::Client(PortManager *pm)
	: FDManager(openSocket(pm->getFD())),
	  pm(pm),
	  reader(this->getFD()),
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

void
	Client::readEvent()
{
	reader.readRequest();
	for (Dialogue *pingpong = reader.parseRequest(); pingpong != NULL; pingpong = reader.parseRequest())
	{
		dialogues.push(pingpong);
		// Make Response
	}
}

void
	Client::writeEvent()
{
	if (dialogues.empty() == true)
		throw UnexceptedEventOccured("Client write during empty response queue");
	if (writer.emptyBuffer() == true)
	{
		writer.setResponse(dialogues.front()->res);
		dialogues.pop();
	}
}

/* private */
