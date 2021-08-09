#ifndef LOCATION_HPP
#define LOCATION_HPP

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
	Location(const Location &other);
	~Location();

	Location&	operator=(const Location &other);

	void	setRoot(std::string root_path) { root = root_path; }
	void	setAutoindex(std::string on_off_string);
	void	setReturnInfo(int code, std::string uri);
	void	setErrorPage(int error_code, std::string page_path);
	void	setCGI(std::string extension, std::string exe_path);

	void	addIndex(std::string path);
	void	addMethod(std::string method);

private:
	std::string							path;
	std::string							root;
	std::vector<std::string>			index;
	std::map<std::string, std::string>	cgi_info;
	std::map<int, std::string>			error_pages;
	std::set<Request::Method>			allowed_methods;
	std::pair<int, std::string>			return_info;
	bool								auto_index;
};


// class Location
// {
// private:
// 	Location();

// 	std::string							path;
// 	std::string							root;
// 	std::vector<std::string>			index;
// 	std::map<std::string, std::string>	cgi_info;
// 	std::map<int, std::string>			error_page;
// 	std::set<Request::Method>			method_allowed;
// 	std::pair<int, std::string>			return_info;
// 	bool								auto_index;

// public:
// 	Location(std::string path) {};
// 	Location(const Location& s);
// 	~Location() {};

// 	Location& operator= (const Location& s);

// 	int	setRoot(std::string root);
// 	int	setAutoindex(std::string auto_index);

// 	int	addIndex(std::string path); // Success : 0, Fail : 1
// 	int	addCGI(std::string extension, std::string exe_path); // Success : 0, Fail : 1
// 	int	addErrorPage(int error_code, std::string page_path); // Success : 0, Fail : 1
// 	int	addMethod(Request::Method method);

// 	int setReturnInfo(int code, std::string uri);

// 	//getter
// 	std::string	getPath();
// 	std::string	getRoot();
// 	std::vector<std::string>	getIndex();
// 	std::string	getCGIExecPath(std::string extension);
// 	// std::string	getErrorPage(int error_code);
// 	std::set<Request::Method>			getMethodAllowed();
// 	bool		isAutoIndex();
// 	size_t		getReturnCode();
// 	std::map<int, std::string>		getErrorPages();

// };

#endif

