#include "Location.hpp"

Location::Location()
{
}

Location::Location(const Location &other)
{
	*this = other;
}

Location::~Location()
{
}

Location& Location::operator=(const Location &other)
{
	return (*this);
}

void	Location::setAutoindex(std::string on_off_string)
{
}

void	Location::setReturnInfo(int code, std::string uri)
{
	this->return_info = make_pair(code, uri);
}

void	Location::setErrorPage(int error_code, std::string page_path)
{
	this->error_pages.insert(make_pair(error_code, page_path));
}

void	Location::setCGI(std::string extension, std::string exe_path)
{
	this->cgi_info.insert(make_pair(extension, exe_path));
}

void	Location::addIndex(std::string path)
{
	this->index.push_back(path);
}

std::string					Location::getPath()
{
	return (path);
}

std::string					Location::getRoot()
{
	return (root);
}
std::vector<std::string>	Location::getIndex()
{
	return (index);
}
std::string					Location::getCGIExecPath(std::string extension)
{
	std::map<std::string, std::string>::iterator iter = cgi_info.find(extension);
	return (iter->second);
}

std::map<std::string, std::string>	Location::getCGIInfo()
{
	return (cgi_info);
}

bool						Location::isAutoIndex()
{
	return (auto_index);
}
size_t						Location::getReturnCode()
{
	return (return_info.first);
}
std::map<int, std::string>	Location::getErrorPages()
{
	return (error_pages);
}
std::set<Request::Method>	Location::getMethodAllowed()
{
	return (allowed_methods);
}
