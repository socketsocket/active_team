#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include <string>

# include "Request.hpp"
# include "Response.hpp"
# include "Location.hpp"
# include "Exception.hpp"

class	Server
{
public:
	Server();
	Server(const Server &other);
	~Server();

	Server&	operator=(const Server &other);

	Response	makeResponse(Request req);

	int		addLocation(std::string path, Location *loc);
	int		addErrorPage(int error_code, std::string page_path);

	int		setBodyLimit(int limit) { body_limit = limit; }
	void	setAutoindex(std::string on_off_string);
	int		setReturnInfo(int code, std::string uri);

private:
	std::map<std::string, Location *>	locations;
	std::map<int, std::string>			error_pages;

	bool	auto_index;
	int		body_limit;

};

#endif
