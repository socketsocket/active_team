#include "Response.hpp"

Response::Response()
	: status_code(0)
{}

Response::~Response()
{}

void
	Response::makeStartLine(std::string http_version, unsigned int status_code, std::string msg)
{
	this->http_version = http_version;
	this->status_code = status_code;
	this->message = msg;

	this->start_line += http_version;
	this->start_line += " ";
	this->start_line += std::to_string(status_code);
	this->start_line += " ";
	this->start_line += message;
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

void
	Response::setStatusCode(unsigned int code)
{
	this->status_code = code;
}
