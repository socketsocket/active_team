#include <unistd.h>

#include "Resource.hpp"
#include "Exception.hpp"

/* static */
static int
	openCGI(CGI &cgi)
{
	int	fildes[2];
	int	pid;

	if (pipe(fildes) == -1)
		throw SystemCallError("pipe");
	if ((pid = fork()) == -1)
		throw SystemCallError("fork");
	else if (pid == 0)
	{}
	else
	{}
	return (fildes[0]);
}

static int
	openFile(std::string path, Resource::Type type)
{
	int	fd;

	if (type == Resource::READ)
		fd = open(path.c_str(), O_RDONLY);
	else if (type == Resource::OVERWRITE)
		fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == Resource::APPEND)
		fd = open(path.c_str(), O_WRONLY | O_APPEND);

	if (fd == -1)
		throw SystemCallError(path);
	return (fd);
}

/* public */
Resource::Resource(std::string path, Type type) : FDManager(openFile(path, type))
{}

Resource::Resource(CGI &cgi) : FDManager(openCGI(cgi))
{}

/* private */
