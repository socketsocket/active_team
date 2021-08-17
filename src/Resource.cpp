#include <unistd.h>

#include "Resource.hpp"
#include "Exception.hpp"
#include "EventHandlerInstance.hpp"

/* static */

/* public */
Resource::Resource(int fd, Dialogue *dialogue)
	: FDManager(fd),
	  dialogue(dialogue)
{}

Resource::~Resource()
{}

void
	Resource::readEvent(long read_size)
{
	std::string				&buffer = dialogue->res.getBody();
	std::string::size_type	buffer_size = buffer.size();
	ssize_t					read_bytes;

	buffer.resize(buffer_size + read_size);
	read_bytes= read(getFD(), &buffer[buffer_size], read_size);

	if (read_bytes == -1)
		throw SystemCallError("read");
	else if (read_bytes == read_size)
	{
		EventHandlerInstance::getInstance().enableWriteEvent(dialogue->client_fd);
		delete this;
	}
}

void
	Resource::writeEvent(long write_size)
{
	std::string	&target = dialogue->req.getBody();
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

/* private */
