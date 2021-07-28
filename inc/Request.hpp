#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <map>

class Request
{
private:
	int status;
	std::string method;
	std::string uri;
	std::string http_version;
	std::map<std::string, std::string> headers;  // 헤더 중복 허용 안함, 중복시 400 Bad Request, Connection: close
	std::string body;

public:
	Request();
	Request(const Request& req);
	~Request();
	Request& operator= (const Request &req);
};

#endif