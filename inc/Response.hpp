#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>
# include <queue>
# include <deque>

# include "CGI.hpp"
class	Resource;

class	Response
{
public:
	struct	Message { int code; std::string message; };

	typedef std::map<std::string, std::string>	header_type;

	Response();
	~Response();

	int	raw(std::string *raw_string);

	void			makeStartLine(std::string http_version, unsigned int status_code, std::string msg);
	void			addHeader(std::string, std::string);
	void			addBody(std::string);
	void			setStatusCode(unsigned int code);
	void			setCGI(CGI *cgi);
	void			setResource(Resource *resource);

	std::string		getStartLine();
	std::string		&getBody();
	// void			setResoureFD(int fd);
	std::map<std::string, std::string> &getHeaders();
	unsigned int	getStatusCode();
	CGI*			getCGI();
	Resource*		getResource();


private:
	Response(const Response &other);

	Response&	operator=(const Response &other);

	std::string							http_version;
	unsigned int						status_code;
	std::string							message;

	std::map<std::string, std::string>	header;
	std::string							body;

	CGI			*cgi;
	Resource	*resource;
};

#endif
