#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <map>
# include <vector>
# include <string>
# include <iostream>

class Request
{
public:
	enum Method { GET, POST, DELETE };

	Request();
	~Request();

	int				getStatus();
	Method			getMethod();
	std::string 	&getUri();
	std::string 	getHttpVersion();
	std::map<std::string, std::string>	&getHeaders();
	std::string 	&getBody();
	int				getBodyType();

	void			setStatus(int status);
	void			setMethod(Method method);
	void			setUri(std::string uri);
	void			setHttpVersion(std::string version);
	void			setHeaders(std::string key, std::string val);
	void			addBody(std::string body);
	void			setBodyType(int type);

private:
	Request(const Request& req);

	Request& operator= (const Request &req);
	
	int				status;

	Method			method;
	std::string 	uri;
	std::string 	http_version;
	std::map<std::string, std::string> headers;  // 헤더 중복 허용 안함, 중복시 400 Bad Request, Connection: close
	std::string 	body;

	std::string 	raw_req;
	int				body_type;	//처음에 NOBODY 로 초기화
};

#endif
