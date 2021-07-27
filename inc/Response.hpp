#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>

struct	Message { int code; std::string message; };

class	Response
{
public:
	Response(std::string http_version, unsigned int status_code, std::string message);
	Response(const Response &other);
	~Response();

	Response&	operator=(const Response &other);

	int	raw(std::string *raw_string);

private:
    static const Message status_message[];

	Response();

	std::string							http_version;
	unsigned int						status_code;
	std::string							message;
	std::map<std::string, std::string>	header;
	std::string							body;
};

const Message Response::status_message[] = {
	// 100, 200 jiholee
    // 300, 500 hson
    // 400 seyu
	{400, "Bad Request"}, 
	{401, "Unauthorized"},
	{402, "Payment Required"},
	{403, "Forbidden"},

};


// {300, }
// {301, }
// {302, }
// {303, }
// {304, }
// {305, }
// {306, }
// {307, }
// {308, }
// {}



#endif
