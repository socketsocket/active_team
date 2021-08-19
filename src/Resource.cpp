#include <unistd.h>

#include "Resource.hpp"
#include "Exception.hpp"
#include "EventHandlerInstance.hpp"

/* static */
const long	Resource::MAX_OFFSET = 100000;

/* public */
Resource::Resource(int fd, Dialogue *dialogue)
	: FDManager(fd),
	  dialogue(dialogue),
	  status(WAITING)
{}

Resource::~Resource()
{}

void
	Resource::readEvent(long read_size)
{
	read_size = std::min(MAX_OFFSET, read_size);

	std::string	&body = dialogue->res.getBody();
	std::string	buffer;
	ssize_t		read_bytes;

	buffer.resize(read_size);
	read_bytes = read(getFD(), &buffer[0], read_size);

	if (read_bytes == -1)
		throw SystemCallError("read");
	if (status == WAITING)
	{
		if (read_bytes == read_size)
		{
			body = buffer;
			status = DONE;
		}

	}
	else if (status == READING)
	{
		std::stringstream	ss;
		std::string			length_hex;

		ss << std::hex << buffer.length();
		ss >> length_hex;
		body += length_hex;
		body += "\r\n";
		body += buffer;
		body += "\r\n";
		if (read_bytes == read_size)
			status = DONE;
	}
	EventHandlerInstance::getInstance().enableWriteEvent(dialogue->client_fd);
}

void
	Resource::writeEvent(long write_size)
{
	std::string	&target = dialogue->req.getBody();
	write_size = (target.length() < (size_t)write_size) ? target.length() : write_size;
	ssize_t		write_bytes = write(getFD(), &target[0], write_size);

	if (write_bytes == -1)
		throw SystemCallError("write");
	else if ((size_t)write_bytes == target.size())
	{
		target.clear();
		EventHandlerInstance::getInstance().enableWriteEvent(dialogue->client_fd);
	}
	else
	{
		target.erase(0, write_bytes);
		EventHandlerInstance::getInstance().enableWriteEvent(this->getFD());
	}
}

void
	Resource::timerEvent()
{
	throw UnexceptedEventOccured("Resource timer event");
}

Resource::Status
	Resource::getStatus()
{
	return (status);
}

/* private */
