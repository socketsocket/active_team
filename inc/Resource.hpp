#ifndef RESOURCE_HPP
# define RESOURCE_HPP

# include <string>

# include "FDHandler.hpp"
# include "Dialogue.hpp"

class	Resource : FDHandler
{
public:
	Resource(); //얘가 필요함
	Resource(std::string path, int oflag);
	Resource(int fd, Dialogue *dialogue);
	~Resource();

	Resource&	operator=(const Resource &other);

	void	readResource();
	void	writeResource();

	int		getFD();

private:
	Resource(const Resource &other);

	int	openFile(std::string path, int oflag);
	int	fd;
};

#endif