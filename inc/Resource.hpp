#ifndef RESOURCE_HPP
# define RESOURCE_HPP

# include <string>

# include "FDHandler.hpp"

class	Resource : FDHandler
{
public:
	Resource();
	Resource(std::string path, int oflag);
	~Resource();

	Resource&	operator=(const Resource &other);

	void	readResource();
	void	writeResource();

private:
	Resource(const Resource &other);

	int	openFile(std::string path, int oflag);
};

#endif