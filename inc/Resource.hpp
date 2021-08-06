#ifndef RESOURCE_HPP
# define RESOURCE_HPP

# include <string>

# include "FDHandler.hpp"

class	Resource : FDHandler
{
public:
	Resource(std::string path, int oflag);
	~Resource();

	Resource&	operator=(const Resource &other);

private:
	Resource();
	Resource(const Resource &other);

	int	openFile(std::string path, int oflag);
};

#endif
