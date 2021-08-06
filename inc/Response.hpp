#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>

struct	Message { int code; std::string message; };

class	Response
{
public:
	Response();
	// Response(std::string http_version, unsigned int status_code, std::string message);
	Response(const Response &other);
	~Response();

	Response&	operator=(const Response &other);

	int	raw(std::string *raw_string);

	void			setStartLine(std::string http_version, unsigned int status_code, std::string msg);
	void			addHeader(std::string, std::string);
	void			addBody(std::string);

	std::string		getBody();
	void			setResoureFD(int fd);

private:
	// static const Message status_message[];


	std::string							http_version;
	unsigned int						status_code;
	std::string							message;
	std::map<std::string, std::string>	header;
	// std::queue<std::string>			body;
	std::string							body;
	//한번에 읽으면 bodysize = 1 이니까 content-length 읽어서 헤더처리,
	//1이 아니면 chunked 로 처리

	int		resourse_fd;
};

#endif
