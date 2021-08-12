#include "Response.hpp"

Response::Response()
{
}

Response::~Response()
{
}

void
	Response::makeStartLine(std::string http_version, unsigned int status_code, std::string msg)
{
	this->http_version = http_version;
	this->status_code = status_code;
	this->message = message;

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