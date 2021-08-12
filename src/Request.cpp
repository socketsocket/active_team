#include "Request.hpp"

void
	Request::setMethod(Method method)
{
	this->method = method;
}

void
	Request::setURI(std::string uri)
{
	this->uri = uri;
}

void
	Request::setHTTPVersion(std::string version)
{
	this->http_version = version;
}

void
	Request::addHeader(std::string key, std::string value)
{
	if (headers.insert(std::make_pair(key, value)).second == false)
		throw BadRequest();
}

void
	Request::addBody(std::string body)
{
	this->body.push_back(body);
}
