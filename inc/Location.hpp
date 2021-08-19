#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <map>
# include <set>
# include <vector>
# include <string>

# include "Request.hpp"
# include "Response.hpp"

class	Location
{
public:
	Location();
	~Location();

	//setter
	void	setPath(std::string path);
	void	setRoot(std::string root_path);
	void	setAutoindex(std::string on_off_string);
	void	setReturnInfo(int code, std::string uri);
	void	setErrorPage(int error_code, std::string page_path);
	void	setCGI(std::string extension, std::string exe_path);
	void	setBodyLimit(int limit);

	void	addIndex(std::string path);
	void	addMethod(std::string method);

	//getter
	std::string							getPath();
	std::string							getRoot();
	std::vector<std::string>			&getIndex();
	std::string							*getCGIExecPath(std::string extension);
	std::map<std::string, std::string>	&getCGIInfo();
	bool								isAutoIndex();
	size_t								getReturnCode();
	std::pair<int, std::string>			&getReturnInfo();
	std::map<int, std::string>			&getErrorPages();
	std::set<Request::Method>			&getMethodAllowed();
	unsigned int						getBodyLimit();


private:
	Location(const Location &other);

	Location&	operator=(const Location &other);

	std::string							path;
	std::string							root;
	std::vector<std::string>			index;
	std::map<std::string, std::string>	cgi_info;
	std::map<int, std::string>			error_pages;
	std::set<Request::Method>			allowed_methods;
	std::pair<int, std::string>			return_info;
	bool								auto_index;
	int									body_limit;

};

#endif
