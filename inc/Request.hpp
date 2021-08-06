#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <map>

class Request
{
public:
	enum Method { GET, POST, DELETE };

	Request();
	Request(const Request& req);
	~Request();

	Request& operator= (const Request &req);

	//필요
	Method		getMethod();
	std::string 	getUri()	{ return (uri);	};
	std::string 	getHttp_version();
	std::string		getHeaderValue(std::string key);
	std::string 	getBody();

private:
	int		status;
	Method	method;
	std::string uri;
	std::string http_version;
	std::map<std::string, std::string> headers;  // 헤더 중복 허용 안함, 중복시 400 Bad Request, Connection: close
	std::string body;
};

#endif