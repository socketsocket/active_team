#include "Request.hpp"
#include "Exception.hpp"

Request::Request() : method(NON), uri("/"), http_version("http/1.1")
{
}

Request::~Request()
{
}

// int		Request::getStatus()
// {
// 	return (status);
// }

Request::Method
	Request::getMethod()
{
	return (method);
}

std::string& Request::getUri()
{
	return (uri);
}

std::string Request::getHttpVersion()
{
	return (http_version);
}

std::map<std::string, std::string>&
	Request::getHeaders()
{
	return (headers);
}

std::string&	Request::getBody()
{
	return (body);
}

// int	Request::getBodyType()
// {
// 	return (body_type);
// }

// void	Request::setStatus(int status)
// {
// 	this->status = status;
// }

void
	Request::setMethod(Method method)
{
	this->method = method;
}

void
	Request::setUri(std::string uri)
{
	this->uri = uri;
}

void Request::setHttpVersion(std::string version)
{
	this->http_version = version;
}

void Request::setHeaders(std::string key, std::string val)
{
	if (headers.insert(std::make_pair(key, val)).second == false)
		throw BadRequest();
}

void Request::addBody(std::string body)
{
	this->body += body;
}

bool Request::keepConnection()
{
	std::map<std::string, std::string>::iterator	found;

	found = headers.find("connection");
	if (found != headers.end() && found->second == "close")
		return (false);
	return (true);
}

// void Request::setBodyType(int type)
// {
// 	this->body_type = type;
// }
