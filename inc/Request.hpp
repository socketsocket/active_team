#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <map>
# include <vector>
# include <string>

# include "Request.hpp"
# include "Response.hpp"

class	Request
{
public:
	enum Method	{ GET, POST, DELETE };

	Request();
	Request(const Request &other);
	~Request();

	Request&	operator=(const Request &other);

private:
	Method								method;
	std::string							uri;
	std::string							http_version;
	std::map<std::string, std::string>	headers;
	std::vector<std::string>			body;
};

#endif
