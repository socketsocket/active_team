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

void
	Resource::readEvent()
{
	int	read_size = 1; // read event size parameter

	std::string	&buffer = dialogue->res.getBody();

	buffer.resize(buffer.size() + read_size);
	read_size = read(getFD(), &buffer[buffer.size()], read_size);

	if (read_size == -1)
		throw SystemCallError("read");
	else if (read_size == 0)
		EventHandlerInstance::getInstance().enableWriteEvent(dialogue->client->getFD());
}

void
	Resource::writeEvent()
{
	std::string	&target = dialogue->req.getBody();
	ssize_t		write_size = write(getFD(), &target[0], target.size());

	if (write_size == -1)
		throw SystemCallError("write");
	else if ((size_t)write_size == target.size())
		target.clear();
	else
		target.erase(0, write_size);
}

void
	Resource::timerEvent()
{
	throw UnexceptedEventOccured("Resource timer event");
}

/* private */
