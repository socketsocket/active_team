#include "RequestProcessor.hpp"

// 클라이언트에서 내 프로세스처리클래스에서 응답을 준비시키고
// 그 안에서 서버가 makeResponse 를 실행시키고
// 그 과정에서 리스폰스 클래스에 있는 addHeader 함수 등을 실행시켜 필요한 정보를 담는다


void 	RequestProcessor::prepareResponse()
{
	Server		server = pm->getServer(req->getHeaderValue("HOST"));
	Location	location = server.getLocation(req->getUri());


	// Allowed Method (405 error)
	if (location.getMethodAllowed().empty() ||
			std::find(location.getMethodAllowed().begin(), location.getMethodAllowed().end(), req->getMethod()) == location.getMethodAllowed().end())
		*(this->getResponse()) = server.makeErrorResponse(server, location, 405);
	
	// Client Body Limit
	if (req->getBody().length() > server.getBodyLimit())
		*(this->getResponse()) = server.makeErrorResponse(server, location, 413);

	// Server Block return
	if (server.getReturnCode() != 0)
		*(this->getResponse()) = server.makeReturnResponse(server.getReturnCode());

	// response
	if (1)	//isCgiRequest())
	{}
	else
	{
		std::string resource_path = req->getUri();
		resource_path.replace(0, location.getPath().length(), location.getRoot());
		
		if (req->getMethod() == Request::GET)
			*(this->getResponse()) = server.makeGETResponse(server, location, resource_path);
		else if (req->getMethod() == Request::POST)
			*(this->getResponse()) = server.makePOSTResponse(server, location, resource_path);
		else if (req->getMethod() == Request::DELETE)
			*(this->getResponse()) = server.makeDELETEResponse(server, location, resource_path);
		req->setStatus(NEED_RESOURCE);
	}
}

std::string Server::makeHTMLPage(std::string content)
{
	std::string html;

	html += "<html>\r\n";

	html += "<head>\r\n";
	html += "</head>\r\n";

	html += "<body>\r\n";
	html += "<hr>\r\n";
	html += "<pre>\r\n";
	html += content;
	html += "</pre>\r\n";
	html += "<hr>\r\n";
	html += "</body>\r\n";

	html += "</html>";
	
	return (html);
}
