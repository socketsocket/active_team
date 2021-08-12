#include "Location.hpp"
#include "Exception.hpp"

Location::Location()
	: auto_index(false),
	  return_info(std::make_pair(200, ""))
{}

Location::~Location()	{}

void
	Location::setRoot(std::string root_path)
{
	if (root.empty() == false)
		throw DoubleDirective("root");
	root = root_path;
}

void
	Location::setAutoindex(std::string on_off_string)
{
	if (on_off_string == "on")
		auto_index = true;
	else if (on_off_string == "off")
		auto_index = false;
	else
		throw BadDirectiveValue("on_off_string");
}

void
	Location::setReturnInfo(int code, std::string uri)
{
	return_info.first = code;
	return_info.second = uri;
}

void
	Location::setErrorPage(int error_code, std::string page_path)
{
	if (error_pages.insert(std::make_pair(error_code, page_path)).second == false)
		throw BadDirective("error_code");
}

void
	Location::setCGI(std::string extension, std::string exe_path)
{
	if (cgi_info.insert(std::make_pair(extension, exe_path)).second == false)
		throw BadDirective(extension);
}

void
	Location::addIndex(std::string path)
{
	index.push_back(path);
}

void
	Location::addMethod(std::string method)
{
	std::pair<std::set<Request::Method>::iterator, bool>	insert_return;
	if (method == "get")
		insert_return = allowed_methods.insert(Request::GET);
	else if (method == "post")
		insert_return = allowed_methods.insert(Request::POST);
	else if (method == "delete")
		insert_return = allowed_methods.insert(Request::DELETE);
	else
		throw BadDirectiveValue(method);

	if (insert_return.second == false)
		throw BadDirectiveValue(method);
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
