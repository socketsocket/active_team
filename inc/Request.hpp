#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <map>
# include <vector>
# include <string>
# include <iostream>

class Request
{
public:
	enum Method { GET, POST, DELETE, OTHER };

	Request();
	Request(const Request& req);
	~Request();

	Request& operator= (const Request &req);

	//필요
	int				getStatus();
	Method			getMethod();
	std::string 	getUri()	{ return (uri);	};
	std::string 	getHttp_version();
	std::map<std::string, std::string>	getHeaders();
	// std::string		getHeaderValue(std::string key);
	std::string 	getBody();
	void	setBodyType(int);

	void	setStatus(int);
	void	setMethod(int);
	void	setUri(std::string);
	void	setHttpVersion(std::string);
	void	setHeaders(std::string, std::string);
	void	addBody(std::string);
	int		getBodyType();

private:
	int		status;

	Method	method;
	std::string uri;
	std::string http_version;
	std::map<std::string, std::string> headers;  // 헤더 중복 허용 안함, 중복시 400 Bad Request, Connection: close
	std::string body;

	std::string raw_req;
	int		body_type;
};

#endif