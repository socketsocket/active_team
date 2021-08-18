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
	Client::readEvent(long read_size)
{
	EventHandlerInstance::getInstance().setTimerEvent(this->getFD());
	reader.readRequest(read_size);
	for (Dialogue *pingpong = reader.parseRequest(); pingpong != NULL; pingpong = reader.parseRequest())
	{
		dialogues.push(pingpong);
		this->prepareResponse(pingpong);
	}
}

void
	Client::writeEvent(long write_size)
{
	if (dialogues.empty() == true)
		throw UnexceptedEventOccured("Client write during empty response queue");
	if (writer.emptyBuffer() == true)
	{
		writer.pushResponse(dialogues.front()->res);
		if (dialogues.front()->res.getCGI() == NULL)
			delete dialogues.front();
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
	while (dialogues.empty() == false)
	{
		delete dialogues.front();
		dialogues.pop();
	}

	Dialogue *pingpong = new Dialogue(this->getFD());

	pingpong->req.setMethod(Request::GET);
	pingpong->req.setUri("/");
	pingpong->req.setHttpVersion("http/1.1");
	pingpong->req.setHeaders("host", "");
	pingpong->res.setStatusCode(408);
	dialogues.push(pingpong);
	this->prepareResponse(pingpong);
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
		return 0;

	return (cgi_path);
}

void
	Client::prepareResponse(Dialogue *dial)
{
	std::map<std::string, std::string>::iterator iter = dial->req.getHeaders().find("host");
	Server		*server = pm->getServer(iter->second);
	//요청 uri 가 없을 때 어떻게 ?
	Location	*location = server->getLocation(dial->req.getUri());

	//maybe 400
	if (dial->res.getStatusCode() != 0)
		server->makeErrorResponse(dial, location, dial->res.getStatusCode());
	
	else if (location == NULL)
		server->makeErrorResponse(dial, location, 404);

	// Allowed Method (405 error)
	else if (std::find(location->getMethodAllowed().begin(), location->getMethodAllowed().end(), dial->req.getMethod()) == location->getMethodAllowed().end())
		server->makeErrorResponse(dial, location, 405);

	// Client Body Limit
	else if (dial->req.getBody().length() > server->getBodyLimit()
			|| dial->req.getBody().length() > location->getBodyLimit())
		server->makeErrorResponse(dial, location, 413);

	// Server Block return
	else if (server->getReturnCode() != 0 || location->getReturnCode() != 0)
		server->makeReturnResponse(dial, location, server->getReturnCode());

	if (dial->status == Dialogue::READY_TO_RESPONSE)
		return EventHandlerInstance::getInstance().enableWriteEvent(this->getFD());

	if (dial->res.getResource() != NULL)
		return ;

	std::string resource_path = dial->req.getUri();
	resource_path.replace(0, location->getPath().length(), location->getRoot());
	try
	{
		std::string*	cgi_path;
		if ((cgi_path = this->isCGIRequest(dial->req, *location)) != 0)
		{
			if (dial->req.getMethod() != Request::POST)
			{
				struct stat buf;

				if (stat(resource_path.c_str(), &buf) == -1)
					return server->makeErrorResponse(dial, location, 404);
			}
			//add headers
			Response &res = dial->res;
			res.addHeader("Date", server->dateHeader());
			res.addHeader("Server", "hsonseyu Server");
			res.addHeader("Connection", "keep-alive");
			try
			{
				res.setCGI(new CGI(*(cgi_path), dial, pm->getPort()));
				res.makeStartLine("HTTP/1.1", 200, server->statusMessage(200));
			}
			catch (BadRequest &e)
			{
				res.makeStartLine("HTTP/1.1", 400, server->statusMessage(400));
			}
			catch (Forbidden &e)
			{
				res.makeStartLine("HTTP/1.1", 403, server->statusMessage(403));
			}
			catch (NotFound &e)
			{
				res.makeStartLine("HTTP/1.1", 404, server->statusMessage(404));
			}
			EventHandlerInstance::getInstance().enableWriteEvent(this->getFD());
		}
		else
		{
			// root directory check in parsing
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
	catch (BadRequest &e)
	{
		server->makeErrorResponse(dial, location, 400);
	}
}

