#ifndef RESOURCE_CPP
# define RESOURCE_CPP

# include "Resource.hpp"

Resource::Resource() : FDHandler()
{
}

Resource::Resource(std::string path, int fd)
	: FDHandler(openFile(path, fd))
{}

Resource::~Resource()
{
}

Resource& Resource::operator=(const Resource &ref)
{
	return (*this);
}

#endif