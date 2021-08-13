#include <cstdlib>

#include "RequestReader.hpp"
#include "Dialogue.hpp"
#include "Exception.hpp"

RequestReader::RequestReader(int client_socket)
	: client_fd(client_socket),
	  dial(new Dialogue()),
	  chunked(false),
	  content_length(0),
	  stat(PARSING_START)
{}

RequestReader::~RequestReader()
{
	delete dial;
}

static void
	string_tolower(char *first, char *last)
{
	for (char *itr = first; itr != last; ++itr)
		*itr= std::tolower(*itr);
}

static long
	string_tolong(std::string str)
{
	char	*itr;
	long	temp;

	temp = std::strtol(str.c_str(), &itr, 10);
	if (*itr != '\0')
		throw std::exception();
	return (temp);
}

void	RequestReader::readRequest(int read_size)
{
	std::string::size_type	buffer_size = buffer.size();
	buffer.resize(buffer_size + read_size);

	int	read_bytes = read(this->client_fd, &buffer[buffer_size], read_size);

	if (read_bytes <= 0)
		throw SystemCallError("read");
}

void	RequestReader::makeStartLine()
{
	Request			&req = dial->req;
	size_t			pos;
	std::string		start_line;
	std::string		tmp;

	pos = this->buffer.find("\r\n");
	if (pos == std::string::npos)
		return ;
	start_line = this->buffer.substr(0, pos);

	string_tolower(&start_line[0], &start_line[start_line.size()]);

	pos = start_line.find(' ');
	tmp = start_line.substr(0, pos);
	if (tmp == "get")
		req.setMethod(Request::GET);
	else if (tmp == "post")
		req.setMethod(Request::POST);
	else if (tmp == "delete")
		req.setMethod(Request::DELETE);
	else
		throw BadRequest();

	tmp = start_line.substr(pos + 1, start_line.rfind(' ') - (pos + 1));
	req.setUri(tmp);

	tmp = start_line.substr(start_line.rfind(' ') + 1);
	req.setHttpVersion(tmp);

	if (this->buffer.length() > start_line.size())
		this->buffer = this->buffer.substr(start_line.size() + 2);
	else
		this->buffer.clear();

	stat = PARSING_HEADER;
}

void	RequestReader::makeReqHeader()
{
	Request		&req = dial->req;
	size_t		pos;
	std::string header_line;
	std::string key, value;

	pos = this->buffer.find("\r\n");
	if (pos == std::string::npos)
		return ;
	while (pos != std::string::npos && pos != 0)
	{
		header_line = this->buffer.substr(0, pos);
		pos = this->buffer.find(":");
		key = header_line.substr(0, pos);
		value = header_line.substr(pos + 2);
		string_tolower(&key[0], &key[key.size()]);
		req.setHeaders(key, value);

		this->buffer.erase(header_line.size() + 2);
		pos = this->buffer.find("\r\n");
	}
	if (pos == 0)
		stat = PARSING_BODY;
}

void	RequestReader::checkBody()
{
	typedef std::map<std::string, std::string>::iterator	iterator;

	iterator	iter;
	Request		&req = dial->req;

	iter = req.getHeaders().find("transfer-encoding");
	if (iter != req.getHeaders().end())
	{
		if (iter->second == "chunked")
		{
			this->chunked = true;
			return ;
		}
	}
	iter = req.getHeaders().find("content-length");
	if (iter != req.getHeaders().end())
	{
		try
		{
			content_length = string_tolong(iter->second);
			return ;
		}
		catch (const std::exception& e)
		{
			throw BadRequest();
		}
	}
}

void	RequestReader::makeChunkedBody()
{
	Request	&req = dial->req;
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
		stat = REQUEST_COMPLETE;
}

void	RequestReader::makeLengthBody()
{
	Request	&req = dial->req;
	size_t	len;

	len = atoi(req.getHeaders()["Content-Length"].c_str());
	req.addBody(this->buffer.substr(0, len));
	this->buffer.erase(0, len);
	stat = REQUEST_COMPLETE;
}


Dialogue*	RequestReader::parseRequest(void)
{
	if (stat == PARSING_START)
		this->makeStartLine();
	if (stat == PARSING_HEADER)
	{
		this->makeReqHeader();
		this->checkBody();
	}
	if (stat == PARSING_BODY)
	{
		if (chunked)
			this->makeChunkedBody();
		else if (content_length > 0)
			this->makeLengthBody();
		else
			stat = REQUEST_COMPLETE;
	}
	if (stat == REQUEST_COMPLETE)
	{
		Dialogue	*rtn = dial;

		dial = new Dialogue();
		stat = PARSING_START;
		return (rtn);
	}
	else
		return (NULL);
}
