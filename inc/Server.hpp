#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include <string>

# include "Request.hpp"
# include "Resource.hpp"
# include "Response.hpp"
# include "Location.hpp"
# include "Exception.hpp"

# include <sys/stat.h>
# include <dirent.h>
# include <time.h>
# include <fcntl.h>
# include <sys/event.h>
# include <unistd.h>

# define NotFound	0
# define File		1
# define Directory	2

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

	//config parsing
	int			addLocation(std::string path, Location *loc); // Success : 0, Fail : 1
	int			addErrorPage(int error_code, std::string page_path); // Success : 0, Fail : 1

	int			setBodyLimit(unsigned long limit);
	void		setAutoindex(std::string on_off_string);
	int			setReturnInfo(int code, std::string uri);
	
	
	//make Response
	Response	makeResponse(Request req); //안에서 스타트라인,헤더처리

	std::string	makeHTMLPage(std::string body);
	std::string	generateErrorPage(size_t error_code);
	
	Response	makeErrorResponse(Server &server, Location &location, size_t);
	Response	makeReturnResponse(size_t);
	std::string	makeAutoIndexPage(std::string path, std::string uri, Location &location);
	Response	makeGETResponse(Server &, Location &, std::string);
	Response	makePOSTResponse(Server &, Location &, std::string);
	Response	makeDELETEResponse(Server &, Location &, std::string);
	Response	makeCGIResponse();

	std::string					statusMessage(size_t error_code);
	std::string					contentTypeHeader(std::string extension);
	std::string					dateHeader();
	
	bool						isAutoIndex();
	int							checkPath(std::string path);
	int							deleteDirectory(std::string path);
	

	//getter
	Location					getLocation(std::string uri);
	unsigned int				getBodyLimit();
	size_t						getReturnCode();
	std::map<int, std::string>	getErrorPages();
};

#endif