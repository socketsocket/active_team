#include <unistd.h>

#include "ResponseWriter.hpp"
#include "Response.hpp"
#include "Exception.hpp"

ResponseWriter::ResponseWriter(int client_fd)
	: client_fd(client_fd),
	  last_communication(false)
{}

ResponseWriter::~ResponseWriter()
{}

void
	ResponseWriter::pushResponse(Response &res)
{
	if (res.getHeaders().find("Connection")->second == "close")
		last_communication = true;
	buffer += res.getStartLine();
	buffer += "\r\n";

	if (cgi == NULL)
		res.addHeader("Content-Length", std::to_string(res.getBody().length()));
	for (std::map<std::string, std::string>::iterator iter = res.getHeaders().begin(); iter != res.getHeaders().end(); iter++)
	{
		buffer += iter->first;
		buffer += ": ";
		buffer += iter->second;
		buffer += "\r\n";
	}
	if ((cgi = res.getCGI()) == NULL)
	{
		buffer += "\r\n";
		buffer += res.getBody();
	}
}

bool
	ResponseWriter::emptyBuffer()
{
	return (this->buffer.empty());
}

bool
	ResponseWriter::writeResponse(long write_size)
{
	write_size = (write_size < (ssize_t)buffer.size() ? write_size : buffer.size());

	ssize_t	write_bytes = write(client_fd, &buffer[0], write_size);

	if (write_bytes == 0)
		throw SystemCallError("write");
	else if ((size_t)write_bytes == buffer.size())
	{
		buffer.clear();
		if (cgi)
		{
			cgi->start();
			delete cgi;
		}
		return (last_communication);
	}
	else
		buffer.erase(0, write_bytes);
	return (false);
}
