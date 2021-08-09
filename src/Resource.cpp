#ifndef RESOURCE_CPP
# define RESOURCE_CPP

# include "Resource.hpp"

Resource::Resource(std::string path, int fd)
	: FDHandler(openFile(path, fd))
{}

#endif