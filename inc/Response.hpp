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
    {100, "Continue"},
    {101, "Switching Protocols"},
    {102, "Processing"},
    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {203, "Non-Authoritative Information"},
    {204, "No Content"},
    {205, "Reset Content"},
    {206, "Partial Content"},
    {207, "Multi-Status"},
    {208, "Already Reported"},
    {226, "IM Used"},
    {300, "Multiple Choices"},
    {301, "Moved Permanently"},
    {302, "Found"},
    {303, "See Other"},
    {304, "Not Modified"},
    {305, "Use Proxy"},
    {307, "Temporary Redirect"},
    {308, "Permanent Redirect"},

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
