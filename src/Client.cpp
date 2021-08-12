#include "Client.hpp"

Client::Client(int socket_fd, PortManager *port_manager)
	: FDHandler(socket_fd),
	  reader(socket_fd),
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

Client& Client::operator=(const Client &other)
{
	return (*this);
}

void	Client::readEvent()
{
	//읽어서 RequestReader buffer 에 raw_request 를 담고
	//파싱 함수 실행

	char	buf[BUFFER_SIZE];
	int		len;

	len = read(this->getFD(), buf, BUFFER_SIZE - 1);
	if (len <= 0)
	{
		//error
	}
	buf[len] = 0;
	reader.getRawRequest() += buf;

	if (REQUEST_COMPLETE)
	dialogues.push(reader.parseRequest());
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

// 클라이언트에서 응답을 준비시키고
// 그 안에서 서버가 makeResponse 를 실행시키고
// 그 과정에서 리스폰스 클래스에 있는 addHeader 함수 등을 실행시켜 필요한 정보를 담는다

void 	Client::prepareResponse(PortManager *pm, Dialogue *dial)
{
	std::map<std::string, std::string>::iterator iter = dial->getReq().getHeaders().find("HOST");
	Server		server = pm->getServer(iter->second);
	Location	location = server.getLocation(dial->getReq().getUri());


	// Allowed Method (405 error)
	if (location.getMethodAllowed().empty() ||
			std::find(location.getMethodAllowed().begin(), location.getMethodAllowed().end(), dial->getReq().getMethod()) == location.getMethodAllowed().end())
		server.makeErrorResponse(dial, location, 405);
	
	// Client Body Limit
	if (dial->getReq().getBody().length() > server.getBodyLimit())
		server.makeErrorResponse(dial, location, 413);

	// Server Block return
	if (server.getReturnCode() != 0 || location.getReturnCode() != 0)
		server.makeReturnResponse(dial, location, server.getReturnCode());

	// response
	if (this->isCGIRequest(dial->getReq(), location))
	{
		//add headers
		Response &res = dial->getRes();
		res.addHeader("Date", server.dateHeader());
		res.addHeader("Server", "hsonseyu Server");

		CGI(dial);
	}
	else
	{
		std::string resource_path = dial->getReq().getUri();
		resource_path.replace(0, location.getPath().length(), location.getRoot());
		
		if (dial->getReq().getMethod() == Request::GET)
			server.makeGETResponse(dial, location, resource_path);
		else if (dial->getReq().getMethod() == Request::POST)
			server.makePOSTResponse(dial, location, resource_path);
		else if (dial->getReq().getMethod() == Request::DELETE)
			server.makeDELETEResponse(dial, location, resource_path);
		dial->getReq().setStatus(NEED_RESOURCE);

		if (dial->getStatus() == Dialogue::ReadyToResponse)
			EventHandlerInstance::getInstance().enableWriteEvent(this->getFD());

	}
}
