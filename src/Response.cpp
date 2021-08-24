#include "Response.hpp"

Response::Response()
	: status_code(200),
	  cgi(NULL),
	  resource(NULL)
{}

Response::~Response()
{
	delete cgi;
	// DO NOT DELETE RESOURCE; IT IS UP TO RESPONSE_WRITER
	//delete resource;
}

void
	Response::makeStartLine(std::string http_version, unsigned int status_code, std::string msg)
{
	this->http_version = http_version;
	this->status_code = status_code;
	this->message = msg;
}

void
	Response::addHeader(std::string key, std::string val)
{
	this->header.insert(make_pair(key, val));
}

void
	Response::addBody(std::string body)
{
	this->body += body;
}

std::string
	Response::getStartLine()
{
	std::string	start_line;

	start_line = http_version;
	start_line += " ";
	start_line += std::to_string(status_code);
	start_line += " ";
	start_line += message;
	return (start_line);
}

std::string&
	Response::getBody()
{
	return (body);
}

std::map<std::string, std::string>&
	Response::getHeaders()
{
	return (header);
}

unsigned int
	Response::getStatusCode()
{
	return (status_code);
}

CGI	*Response::getCGI()
{
	return (cgi);
}

Resource
	*Response::getResource()
{
	return (resource);
}

void
	Response::setStatusCode(unsigned int code)
{
	this->status_code = code;
}

void
	Response::setCGI(CGI *cgi)
{
	this->cgi = cgi;
}

void
	Response::setResource(Resource *res)
{
	this->resource = res;
}
