#include "Request.hpp"

Request::Request()
{
}

Request::Request(const Request &req)
{
	*this = req;
}

Request::~Request()
{
}

Request& Request::operator=(const Request &req)
{
	return (*this);
}

int		Request::getStatus()
{
	return (status);
}

// Method	Request::getMethod()
// {
// 	return (method);
// }

std::string Request::getUri()
{
	return (uri);
}

std::string Request::getHttpVersion()
{
	return (http_version);
}

std::map<std::string, std::string>
	Request::getHeaders()
{
	return (headers);
}

std::string	Request::getBody()
{
	return (body);
}

int	Request::getBodyType()
{
	return (body_type);
}

void	Request::setStatus(int status)
{
	this->status = status;
}

// void	Request::setMethod()
// {
// 	return (method);
// }

void Request::setUri(std::string uri)
{
	this->uri = uri;
}

void Request::setHttpVersion(std::string version)
{
	this->http_version = version;
}

void Request::setHeaders(std::string key, std::string val)
{
	this->headers.insert(make_pair(key,val));
}

void Request::addBody(std::string body)
{
	this->body += body;
}

void Request::setBodyType(int type)
{
	this->body_type = type;
}