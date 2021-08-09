#ifndef RESOURCE_HPP
# define RESOURCE_HPP

# include <string>

# include "FDManager.hpp"
# include "CGI.hpp"

class	Resource : FDManager
{
public:
	enum Type { READ, OVERWRITE, APPEND };

	Resource(std::string path, Type type);
	Resource(CGI &cgi);
	~Resource();

	Resource&	operator=(const Resource &other);

	virtual void	readEvent();
	virtual void	writeEvent();

private:
	Resource();
	Resource(const Resource &other);
};

#endif
