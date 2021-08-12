#include <sys/socket.h>
#include "Exception.hpp"

#include "Client.hpp"

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
		writer.pushResponse(dialogues.front()->res);
		dialogues.pop();
	}
}

void
	Client::timerEvent()
{
	// timeout Response
	// Client Destruct
}

int		Client::isCGIRequest(Request &request, Location &location)
{
	size_t dot;

	if ((dot = request.getUri().find('.')) == std::string::npos)
		return (0);
	size_t extension = dot;
	while (request.getUri()[extension] != '?' && extension != request.getUri().length())
		extension++;
	
	std::string cgi_ext = request.getUri().substr(dot, extension - dot);
	
	std::map<std::string, std::string>::iterator cgi_iter = location.getCGIInfo().find(cgi_ext);
	if (cgi_iter == location.getCGIInfo().end())
		return (0);
	
	return (1);
}
void 	Client::prepareResponse(PortManager *pm, Dialogue *dial)
{
	std::map<std::string, std::string>::iterator iter = dial->req.getHeaders().find("HOST");
	Server		*server = pm->getServer(iter->second);
	Location	&location = server->getLocation(dial->req.getUri());


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
	if (this->isCGIRequest(dial->req, location))
	{
		//add headers
		Response &res = dial->res;
		res.addHeader("Date", server->dateHeader());
		res.addHeader("Server", "hsonseyu Server");

		//CGI(dial); 다시!
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
		dial->req.setStatus(NEED_RESOURCE);

		if (dial->status == Dialogue::ReadyToResponse)
			EventHandlerInstance::getInstance().enableWriteEvent(this->getFD());

	}
}

