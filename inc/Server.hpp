#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include <string>

# include <sys/stat.h>
# include <dirent.h>
# include <time.h>
# include <fcntl.h>
# include <sys/event.h>
# include <unistd.h>

class	Request;

# include "Exception.hpp"
# include "Response.hpp"
# include "Location.hpp"
# include "Dialogue.hpp"
# include "Resource.hpp"

class Server
{
public:
	Server();
	~Server();

	//config parsing
	void		addLocation(std::string path, Location* loc); // Success : 0, Fail : 1
	void		addErrorPage(int error_code, std::string page_path); // Success : 0, Fail : 1

	void		setBodyLimit(int limit);
	void		setAutoindex(std::string on_off_string);
	void		setReturnInfo(int code, std::string uri);
	
	
	//make Response
	Response	makeResponse(Request req); //안에서 스타트라인,헤더처리

	std::string	makeHTMLPage(std::string body);
	std::string	generateErrorPage(size_t error_code);
	
	void		makeErrorResponse(Dialogue *, Location &location, size_t);
	void		makeReturnResponse(Dialogue *, Location &location, size_t);
	std::string	makeAutoIndexPage(std::string path, std::string uri, Location &location);
	void		makeGETResponse(Dialogue *, Location &, std::string);
	void		makePOSTResponse(Dialogue *, Location &, std::string);
	void		makeDELETEResponse(Dialogue *, Location &, std::string);
	// Response	makeCGIResponse();

	std::string					statusMessage(size_t code);
	std::string					contentTypeHeader(std::string extension);
	std::string					dateHeader();
	std::string					lastModifiedHeader();
	std::string					connectionHeader(Request &req);

	int							checkPath(std::string path);
	int							deleteDirectory(std::string path);
	// bool						isAutoIndex();
	
	//getter
	Location					*getLocation(std::string uri);
	unsigned int				getBodyLimit();
	size_t						getReturnCode();
	std::map<int, std::string>	getErrorPages();

private:
	enum FileType { NotFound, File, Directory };

	Server(const Server& s);

	Server& operator= (const Server& s);

	std::map<std::string, Location*>	locations;
	std::map<int, std::string>			error_page;

	bool							auto_index;
	int								body_limit;
	std::pair<int, std::string>		return_info;

};

#endif
