#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>
# include <queue>

# include "Resource.hpp"


class	Response
{
public:
	struct	Message { int code; std::string message; };

	Response(const Response &other);
	~Response();

	Response&	operator=(const Response &other);

	int	raw(std::string *raw_string);

	void			setStartLine(std::string http_version, unsigned int status_code, std::string msg);
	void			addHeader(std::string, std::string);
	void			addBody(std::string);

	std::string		getBody();
	void			setResoureFD(int fd);

private:
	Response();
	// static const Message status_message[63];

	std::string							http_version;
	unsigned int						status_code;
	std::string							message;

	std::map<std::string, std::string>	header;
	// std::queue<std::string>			body;
	std::string							body;

	Resource	*resource;
	int		resourse_fd;
};

#endif
