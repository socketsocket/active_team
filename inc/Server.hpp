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
	std::map<std::string, Location>	locations;
	std::map<int, std::string>		error_page;
	bool							auto_index;
	unsigned long					body_limit; // 0 ~ string::max_size(), default: string::max_size()
	std::pair<int, std::string>		return_info;

public:
	Server();
	Server(const Server& s);
	~Server();
	Server& operator= (const Server& s);

	Response	makeResponse(Request req);

	int			addLocation(std::string path, Location loc); // Success : 0, Fail : 1
	int			addErrorPage(int error_code, std::string page_path); // Success : 0, Fail : 1

	int			setBodyLimit(unsigned long limit);
	int			setAutoindex(std::string auto_index);
	int			setReturnInfo(int code, std::string uri);
};

#endif