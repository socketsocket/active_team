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
	dialogue->res.body.push_back(std::string());

	std::string	*buffer = &*(dialogue->res.body.rbegin());
	int			read_size = read(getFD(), buffer, READ_BUFFER_SIZE);

	if (read_size == -1)
		throw SystemCallError("read");
	else if (read_size == 0)
		EventHandlerInstance::getInstance().enableWriteEvent(dialogue->client->getFD());
	buffer->resize(buffer->size());
}

void
	Resource::writeEvent()
{
	std::string	&target = dialogue->req.body.front();
	int			write_size = write(getFD(), &target[0], target.size());

	if (write_size == -1)
		throw SystemCallError("write");
	else if (write_size == target.size())
		dialogue->req.body.pop_front();
	else
	{
		if (dialogue->req.body.front().size() == write_size)
			dialogue->req.body.pop_front();
		else
			dialogue->req.body.front().erase(dialogue->req.body.front().begin(), dialogue->req.body.front().begin() + write_size);
	}
}

void
	Resource::timerEvent()
{
	throw UnexceptedEventOccured("Resource timer event");
}

/* private */
