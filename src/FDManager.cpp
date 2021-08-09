#include <unistd.h>

#include "EventHandlerInstance.hpp"
#include "FDManager.hpp"
#include "Exception.hpp"

FDManager::FDManager(int fd) : fd(fd)
{
	if (fd < 0)
		throw BadFileDescriptor();
	fcntl(fd, F_SETFL, O_NONBLOCK);
	EventHandlerInstance::getInstance().registerFD(this);
}

FDManager::~FDManager()
{
	EventHandlerInstance::getInstance().unregisterFD(this);
	close(fd);
}
