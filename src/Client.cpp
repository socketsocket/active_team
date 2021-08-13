#include <sys/socket.h>

#include "EventHandlerInstance.hpp"
#include "Exception.hpp"
#include "Client.hpp"
#include "CGI.hpp"

static int
	openSocket(int server_socket)
{
	int	client_socket = accept(server_socket, NULL, NULL);

	if (client_socket == -1)
		throw SystemCallError("accept");
	return (client_socket);
}

Client::Client(PortManager *pm)
	: FDManager(openSocket(pm->getFD())),
	  pm(pm),
	  reader(this->getFD()),
	  writer(this->getFD())
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
	Client::readEvent(int read_size)
{
	reader.readRequest(read_size);
	for (Dialogue *pingpong = reader.parseRequest(); pingpong != NULL; pingpong = reader.parseRequest())
	{
		dialogues.push(pingpong);
		this->prepareResponse(pingpong);
	}
}

void
	Client::writeEvent(int write_size)
{
	if (dialogues.empty() == true)
		throw UnexceptedEventOccured("Client write during empty response queue");
	if (writer.emptyBuffer() == true)
	{
		writer.pushResponse(dialogues.front()->res);
		dialogues.pop();
	}
	if (writer.writeResponse(write_size))
		delete this;
	else if (dialogues.empty() == false && dialogues.front()->status == Dialogue::READY_TO_WRITE)
		EventHandlerInstance::getInstance().enableWriteEvent(getFD());
}

void
	Client::timerEvent()
{
	// timeout Response
	// Client Destruct
}

std::string*
	Client::isCGIRequest(Request &request, Location &location)
{
	size_t dot;

	if ((dot = request.getUri().find('.')) == std::string::npos)
		return (0);
	size_t extension = dot;
	while (request.getUri()[extension] != '?' && extension != request.getUri().length())
		extension++;

	std::string cgi_ext = request.getUri().substr(dot, extension - dot);

	std::string* cgi_path = location.getCGIExecPath(cgi_ext);
	if (cgi_path == 0)
		throw BadRequest();

	return (cgi_path);
}

void
	Client::prepareResponse(Dialogue *dial)
{
	std::map<std::string, std::string>::iterator iter = dial->req.getHeaders().find("host");
	Server		*server = pm->getServer(iter->second);
	Location	&location = *(server->getLocation(dial->req.getUri()));


	// Allowed Method (405 error)
	if (location.getMethodAllowed().empty() ||
			std::find(location.getMethodAllowed().begin(), location.getMethodAllowed().end(), dial->req.getMethod()) == location.getMethodAllowed().end())
		server->makeErrorResponse(dial, location, 405);

	// Client Body Limit
	if (dial->req.getBody().length() > server->getBodyLimit())
		server->makeErrorResponse(dial, location, 413);

	// Server Block return
	if (server->getReturnCode() != 0 || location.getReturnCode() != 0)
		server->makeReturnResponse(dial, location, server->getReturnCode());

	// response
	try
	{
		std::string*	cgi_path;
		if ((cgi_path = this->isCGIRequest(dial->req, location)) != 0)
		{
			//add headers
			Response &res = dial->res;
			res.addHeader("Date", server->dateHeader());
			res.addHeader("Server", "hsonseyu Server");

			CGI cgi(*(cgi_path), dial, pm->getPort());
		}
		else
		{
			std::string resource_path = dial->req.getUri();
			resource_path.replace(0, location.getPath().length(), location.getRoot());

			if (dial->req.getMethod() == Request::GET)
				server->makeGETResponse(dial, location, resource_path);
			else if (dial->req.getMethod() == Request::POST)
				server->makePOSTResponse(dial, location, resource_path);
			else if (dial->req.getMethod() == Request::DELETE)
				server->makeDELETEResponse(dial, location, resource_path);
			// dial->req.setStatus(NEED_RESOURCE);

			if (dial->status == Dialogue::READY_TO_RESPONSE)
				EventHandlerInstance::getInstance().enableWriteEvent(this->getFD());

		}
	}
	catch (BadRequest)
	{
		server->makeErrorResponse(dial, location, 400);
	}
}

