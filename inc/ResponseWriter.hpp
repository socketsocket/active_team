#ifndef RESPONSEWRITER_HPP
# define RESPONSEWRITER_HPP

# include <deque>
# include <map>
# include <string>

struct	Dialogue;

# include "Response.hpp"
# include "CGI.hpp"

class	ResponseWriter
{
public:
	ResponseWriter(int client_socket);
	~ResponseWriter();

	void	pushResponse(Response &res);
	bool	emptyBuffer();
	bool	writeResponse(long write_size);

private:
	ResponseWriter();
	ResponseWriter(const ResponseWriter &other);

	ResponseWriter&	operator=(const ResponseWriter &other);

	int			client_fd;
	bool		last_communication;
	CGI			*cgi;
	std::string	buffer;
};

#endif
