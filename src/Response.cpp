#include "Response.hpp"

Response::Response()
	: status_code(0),
	  cgi(NULL)
{}

Response::~Response()
{
	// DO NOT DELETE CGI
	// DELETE CGI AFTER USED BY RESPONSE_WRITER
	//delete cgi;
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
