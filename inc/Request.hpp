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
	Method			getMethod(); // 이거 어떻게 쓰는거
	std::string 	getUri()	{ return (uri);	}
	std::string 	getHttpVersion();
	std::map<std::string, std::string>	getHeaders();
	std::string 	getBody();
	int				getBodyType();

	void			setStatus(int status);
	void			setMethod(int method);
	void			setUri(std::string uri);
	void			setHttpVersion(std::string version);
	void			setHeaders(std::string key, std::string val);
	void			addBody(std::string body);
	void			setBodyType(int type);

private:
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