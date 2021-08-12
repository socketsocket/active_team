#include "RequestReader.hpp"


RequestReader::RequestReader(int client_socket)
{
}

RequestReader::~RequestReader()
{
}

RequestReader& RequestReader::operator=(const RequestReader &other)
{
	return (*this);
}

template <typename InputIt>
static void
	string_tolower(InputIt first, InputIt last)
{
	for (InputIt itr = first; itr != last; ++itr)
		*itr= std::tolower(*itr);
}

void	RequestReader::makeStartLine(Request &req)
{
	size_t			pos;
	std::string		start_line;
	std::string		tmp;

	pos = this->buffer.find("\r\n");
	if (pos == std::string::npos)
		return ;
	start_line = this->buffer.substr(0, pos);
	string_tolower(start_line.begin(), start_line.end());

	pos = start_line.find(' ');
	tmp = start_line.substr(0, pos);
	if (tmp == "GET")
		req.setMethod(Request::GET);
	else if (tmp == "POST")
		req.setMethod(Request::POST);
	else if (tmp == "DELETE")
		req.setMethod(Request::DELETE);
	else
		req.setMethod(Request::OTHER);
	
	tmp = start_line.substr(pos + 1, start_line.rfind(' ') - (pos + 1));
	req.setUri(tmp);
	
	tmp = start_line.substr(start_line.rfind(' ') + 1);
	req.setHttpVersion(tmp);

	if (this->buffer.length() > pos + 2)
		this->buffer = this->buffer.substr(pos + 2);
	else
		this->buffer = "";

}

void	RequestReader::makeReqHeader(Request &req)
{
	size_t		pos;
	std::string header_line;
	std::string key, value;

	pos = this->buffer.find("\r\n");
	if (pos == std::string::npos)
		throw BadRequest(400);
	while (pos != std::string::npos && pos != 0)
	{
		header_line = this->buffer.substr(0, pos);
		pos = this->buffer.find(":");
		key = header_line.substr(0, pos);
		string_tolower(key.begin(), key.end());
		value = header_line.substr(pos + 2);
		req.setHeaders(key, value);

		this->buffer.erase(pos + 2);
		pos = this->buffer.find("\r\n");
	}
	if (pos == 0)
		req.setStatus(PARSING_BODY);

}

void	RequestReader::checkBody(Request &req)
{
	std::map<std::string, std::string>::iterator iter = req.getHeaders().find("transfer-encoding");
	if (iter != req.getHeaders().end() && iter->second == "chunked")
	{
		if ((iter = req.getHeaders().find("content-length")) != req.getHeaders().end())
			throw BadRequest(400);
		req.setBodyType(CHUNKED);
	}

	iter = req.getHeaders().find("content-length");
	if (iter != req.getHeaders().end())
	{
		if ((iter = req.getHeaders().find("transfer-encoding")) != req.getHeaders().end())
			throw BadRequest(400);
		req.setBodyType(CONTENT_LENGTH);
	}

	if (req.getBodyType() == NOBODY)
		req.setStatus(REQUEST_COMPLETE);
}

void	RequestReader::makeChunkedBody(Request &req)
{
	size_t	len_line, content_line;
	size_t	len = 1;
	std::string	tmp;

	while (len)
	{
		len_line = this->buffer.find("\r\n");
		content_line = this->buffer.find("\r\n", len_line + 2);
		if (len_line == std::string::npos || content_line == std::string::npos)
			break;
		tmp = this->buffer.substr(0, len_line);
		len = strtol(tmp.c_str(), NULL, 16);
		if (len == 0)
			break;
		tmp = this->buffer.substr(len_line + 2, len);
		req.addBody(tmp);
		this->buffer.erase(0, content_line + 2);
	}
	if (len == 0)
		req.setStatus(REQUEST_COMPLETE);


}

void	RequestReader::makeLengthBody(Request &req)
{
	size_t	len;

	len = atoi(req.getHeaders()["Content-Length"].c_str());
	req.addBody(this->buffer.substr(0, len));
	this->buffer.erase(0, len);
	req.setStatus(REQUEST_COMPLETE);
	
}


Dialogue*	RequestReader::parseRequest(void)
{
	Dialogue *dial = new Dialogue;

	if (dial->getReq().getStatus() == PARSING_START)
		this->makeStartLine(dial->getReq());
	if (dial->getReq().getStatus() == PARSING_HEADER)
	{
		this->makeReqHeader(dial->getReq());
		this->checkBody(dial->getReq());
	}
	if (dial->getReq().getStatus() == PARSING_BODY)
	{
		if (dial->getReq().getBodyType() == CHUNKED)
			this->makeChunkedBody(dial->getReq());
		else if (dial->getReq().getBodyType() == CONTENT_LENGTH)
			this->makeLengthBody(dial->getReq());
	}

	if (dial->getReq().getStatus() == REQUEST_COMPLETE)
		return (dial);
	else
		return (0);
}

std::string
	RequestReader::getRawRequest()
{
	return (buffer);
}
