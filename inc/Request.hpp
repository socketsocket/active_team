#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <map>
# include <deque>
# include <string>

# include "Request.hpp"
# include "Response.hpp"

class	Request
{
public:
	enum Method	{ GET, POST, DELETE };

	Request();
	~Request();

	void	setMethod(Method method);
	void	setURI(std::string uri);
	void	setHTTPVersion(std::string version);
	void	addHeader(std::string key, std::string value);
	void	addBody(std::string body);

	Method								method;
	std::string							uri;
	std::string							http_version;
	std::map<std::string, std::string>	headers;
	std::deque<std::string>				body;

private:
	Request(const Request &other);

	Request&	operator=(const Request &other);
};

#endif
