#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <map>

# include "Request.hpp"
# include "Response.hpp"
# include "Location.hpp"

class Server
{
private:
	Server();

	std::string						ip;
	unsigned int					port;
	std::string						server_name;
	std::map<std::string, Location>	locations;
	std::map<int, std::string>		error_page;
	unsigned long					body_limit; // 0 ~ string::max_size(), default: string::max_size()

public:
	Server(unsigned int port, std::string server_name);
	Server(const Server& s);
	~Server();
	Server& operator= (const Server& s);

	Response	makeResponse(Request req);
	int			newLocation(std::string path, Location loc); // Success : 0, Fail : 1
};

#endif