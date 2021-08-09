#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <map>
# include <set>
# include <vector>
# include <string>

# include "Request.hpp"

class	Location
{
public:
	Location();
	Location(const Location &other);
	~Location();

	Location&	operator=(const Location &other);

	void	setRoot(std::string root_path);
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

#endif
