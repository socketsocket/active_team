#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include <string>

# include "Request.hpp"
# include "Response.hpp"
# include "Location.hpp"
# include "Exception.hpp"

# include <sys/stat.h>
# include <dirent.h>
# include <time.h>

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

	Response	makeResponse(Request req); //안에서 스타트라인,헤더처리

	Response	makeErrorResponse(Server &server, Location &location, size_t);
	Response	makeReturnResponse(size_t);
	std::string	makeAutoIndexPage(std::string path, std::string uri, Location &location);
	Response	makeCGIResponse();
	Response	makeGETResponse(Location &, std::string);
	Response	makePOSTResponse(Location &, std::string);
	Response	makeDELETEResponse(Location &, std::string);

	void		makeStartLine();
	void		makeHeaders();

	int			addLocation(std::string path, Location *loc); // Success : 0, Fail : 1
	int			addErrorPage(int error_code, std::string page_path); // Success : 0, Fail : 1

	int			setBodyLimit(unsigned long limit);
	void		Server::setAutoindex(std::string on_off_string);
	// int			setAutoindex(std::string auto_index);
	int			setReturnInfo(int code, std::string uri);

	//필요
	std::string		contentTypeHeader(std::string extension);
	std::string		statusMessage(size_t error_code);

	std::string		dateHeader();
	
	Location		getLocation(std::string uri);
	unsigned int	getBodyLimit();
	size_t			getReturnCode();
	// std::string		getErrorPage(int error_code);
	std::map<int, std::string>		getErrorPages();


	std::string 	makeHTMLPage(std::string body);
	
	bool			isAutoIndex();
	int				checkPath(std::string path);
	void			deleteDirectory(std::string name);

	void			generateErrorPage(size_t error_code);
};

#endif