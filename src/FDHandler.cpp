#include <unistd.h>
#include <fcntl.h>

#include "FDHandler.hpp"
#include "Exception.hpp"

/* public */

FDHandler::FDHandler(int fd) : fd(fd)
{
	if (fd < 0)
		throw BadFileDescriptor();
	fcntl(fd, F_SETFL, O_NONBLOCK);
}

FDHandler::~FDHandler()
{
	close(fd);
}

/* private */