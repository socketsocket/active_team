#ifndef RESPONSEWRITER_HPP
# define RESPONSEWRITER_HPP

# include <deque>
# include <map>
# include <string>

struct	Dialogue;

# include "Response.hpp"

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
	std::string	buffer;
};

#endif
