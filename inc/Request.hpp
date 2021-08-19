#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <map>
# include <vector>
# include <string>
# include <iostream>

class Request
{
public:
	enum Method { NON, GET, POST, DELETE };

	Request();
	~Request();

	Method			getMethod();
	std::string 	&getUri();
	std::string 	getHttpVersion();
	std::map<std::string, std::string>	&getHeaders();
	std::string 	&getBody();

	void			setMethod(Method method);
	void			setUri(std::string uri);
	void			setHttpVersion(std::string version);
	void			setHeaders(std::string key, std::string val);
	void			addBody(std::string body);
	// void			setBodyType(int type);

	bool			keepConnection();

private:
	Request(const Request& req);

	Request& operator= (const Request &req);

	Method			method;
	std::string 	uri;
	std::string 	http_version;
	std::map<std::string, std::string>
					headers;
	std::string 	body;
};

#endif
