#include <sys/socket.h>
#include <unistd.h>

#include "Dialogue.hpp"
#include "CGI.hpp"

static int
	openPipe(int &script_stdin)
{
	int	fildes[2];

	pipe(fildes);
	script_stdin = fildes[0];
	return (fildes[1]);
}

static std::string
	findValue(
		std::map<std::string, std::string> &req_header,
		std::string key)
{
	std::map<std::string, std::string>::iterator	itr;

	itr = req_header.find(key);
	if (itr == req_header.cend())
		return ("");
	else
		return (itr->second);
}

static std::string
	getAddr(int socket_fd)
{
	struct sockaddr	addr;
	socklen_t		addr_len;
	std::string		addr_string;

	if (getsockname(socket_fd, &addr, &addr_len) == -1)
		throw SystemCallError("getsockname");
	for (int i = 0; i < addr.sa_len; i++)
		addr_string.push_back(addr.sa_data[i]);
	return (addr_string);
}

static std::string
	getMethod(Request::Method method)
{
	switch (method)
	{
	case Request::GET:
		return ("GET");
	case Request::POST:
		return ("POST");
	case Request::DELETE:
		return ("DELETE");
	}
}

static void
	setMetaVariables(
		std::map<std::string, std::string> &meta_variables,
		Dialogue *dialogue,
		std::string &server_name,
		int server_port)
{
	std::map<std::string, std::string>
				&req_header = dialogue->req.headers;
	std::string	uri = dialogue->req.uri;

	meta_variables.insert(std::make_pair("AUTH_TYPE", ""));
	meta_variables.insert(std::make_pair("CONTENT_LENGTH", findValue(req_header, "content_length")));
	meta_variables.insert(std::make_pair("CONTENT_TYPE", findValue(req_header, "content_type")));
	meta_variables.insert(std::make_pair("GATEWAY_INTERFACE", "CGI/1.1"));
	meta_variables.insert(std::make_pair("HTTP_ACCEPT", findValue(req_header, "accept")));
	meta_variables.insert(std::make_pair("HTTP_ACCEPT_CHARSET", findValue(req_header, "accept_charset")));
	meta_variables.insert(std::make_pair("HTTP_ACCEPT_ENCODING", findValue(req_header, "accept_encoding")));
	meta_variables.insert(std::make_pair("HTTP_ACCEPT_LANGUAGE", findValue(req_header, "accept_language")));
	meta_variables.insert(std::make_pair("HTTP_CONNECTION", findValue(req_header, "connection")));
	meta_variables.insert(std::make_pair("HTTP_COOKIE", findValue(req_header, "cookie")));
	meta_variables.insert(std::make_pair("HTTP_FORWARDED", findValue(req_header, "forwarded")));
	meta_variables.insert(std::make_pair("HTTP_HOST", findValue(req_header, "host")));
	meta_variables.insert(std::make_pair("HTTP_PROXY_AUTHORIZATION", findValue(req_header, "proxy_authorization")));
	meta_variables.insert(std::make_pair("HTTP_REFERER", findValue(req_header, "referer")));
	meta_variables.insert(std::make_pair("HTTP_USER_AGENT", findValue(req_header, "user_agent")));
	meta_variables.insert(std::make_pair("PATH_INFO", ""));
	meta_variables.insert(std::make_pair("PATH_TRANSLATED", ""));
	meta_variables.insert(std::make_pair("QUERY_STRING", uri.substr(uri.find('?'))));
	meta_variables.insert(std::make_pair("REMOTE_ADDR", getAddr(dialogue->client->getFD())));
	meta_variables.insert(std::make_pair("REMOTE_HOST", ""));
	meta_variables.insert(std::make_pair("REMOTE_IDENT", ""));
	meta_variables.insert(std::make_pair("REMOTE_USER", ""));
	meta_variables.insert(std::make_pair("REQUEST_METHOD", getMethod(dialogue->req.method)));
	meta_variables.insert(std::make_pair("SCRIPT_NAME", uri.substr(0, uri.find('?'))));
	meta_variables.insert(std::make_pair("SERVER_NAME", server_name));
	meta_variables.insert(std::make_pair("SERVER_PORT", std::to_string(server_port)));
	meta_variables.insert(std::make_pair("SERVER_PROTOCOL", "HTTP/1.1"));
	meta_variables.insert(std::make_pair("SERVER_SOFTWARE", "webserv/1.0"));
}

static int
	execute_cgi(const std::string &script_path, std::map<std::string, std::string> &meta_variables, int script_stdin)
{
	int	pid;
}

CGI::CGI(const std::string &script_path, Dialogue *dialogue, std::string &server_name, int server_port)
	: FDManager(openPipe(script_stdin))
{
	setMetaVariables(meta_variables, dialogue, server_name, server_port);
}
