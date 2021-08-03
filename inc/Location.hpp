#ifndef LOCATION_HPP
#define LOCATION_HPP

# include <string>
# include <vector>
# include <map>
# include <set>

# include "Request.hpp"
# include "Response.hpp"

class Location
{
private:
	Location();

	std::string							path;
	std::string							root;
	std::vector<std::string>			index;
	std::map<std::string, std::string>	cgi_info;
	std::map<int, std::string>			error_page;
	std::set<Request::Method>			method_allowed;
	bool								auto_index;

public:
	Location(std::string path) {};
	Location(const Location& s);
	~Location() {};

	Location& operator= (const Location& s);

	int	setRoot(std::string root);
	int	setAutoindex(std::string auto_index);

	int	addIndex(std::string path); // Success : 0, Fail : 1
	int	addCGI(std::string extension, std::string exe_path); // Success : 0, Fail : 1
	int	addErrorPage(int error_code, std::string page_path); // Success : 0, Fail : 1
	int	addMethod(Request::Method method);

	//getter
	std::string	getPath();
	std::string	getRoot();
	std::vector<const std::string>	getIndex();
	std::string	getCGIExecPath(std::string extension);
	std::string	getErrorPage(int error_code);
};

#endif

