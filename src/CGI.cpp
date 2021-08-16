#include <sys/socket.h>
#include <unistd.h>

#include "EventHandlerInstance.hpp"
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

CGI::CGI(const std::string &script_path, Dialogue *dialogue, int server_port)
	: FDManager(openPipe(script_stdin)),
	  dialogue(dialogue)
{
	std::map<std::string, std::string>	&req_header = dialogue->req.getHeaders();
	std::string	&uri = dialogue->req.getUri();

	if ((script_pid = fork()) == -1)
		throw SystemCallError("fork");
	else if (script_pid == 0)
	{
		setenv("AUTH_TYPE", "", 1);
		setenv("CONTENT_LENGTH", findValue(req_header, "content_length").c_str(), 1);
		setenv("CONTENT_TYPE", findValue(req_header, "content_type").c_str(), 1);
		setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
		setenv("HTTP_ACCEPT", findValue(req_header, "accept").c_str(), 1);
		setenv("HTTP_ACCEPT_CHARSET", findValue(req_header, "accept_charset").c_str(), 1);
		setenv("HTTP_ACCEPT_ENCODING", findValue(req_header, "accept_encoding").c_str(), 1);
		setenv("HTTP_ACCEPT_LANGUAGE", findValue(req_header, "accept_language").c_str(), 1);
		setenv("HTTP_CONNECTION", findValue(req_header, "connection").c_str(), 1);
		setenv("HTTP_COOKIE", findValue(req_header, "cookie").c_str(), 1);
		setenv("HTTP_FORWARDED", findValue(req_header, "forwarded").c_str(), 1);
		setenv("HTTP_HOST", findValue(req_header, "host").c_str(), 1);
		setenv("HTTP_PROXY_AUTHORIZATION", findValue(req_header, "proxy_authorization").c_str(), 1);
		setenv("HTTP_REFERER", findValue(req_header, "referer").c_str(), 1);
		setenv("HTTP_USER_AGENT", findValue(req_header, "user_agent").c_str(), 1);
		setenv("PATH_INFO", "", 1);
		setenv("PATH_TRANSLATED", "", 1);
		setenv("QUERY_STRING", uri.substr(uri.find('?')).c_str(), 1);
		setenv("REMOTE_ADDR", getAddr(dialogue->client_fd).c_str(), 1);
		setenv("REMOTE_HOST", "", 1);
		setenv("REMOTE_IDENT", "", 1);
		setenv("REMOTE_USER", "", 1);
		setenv("REQUEST_METHOD", getMethod(dialogue->req.getMethod()).c_str(), 1);
		setenv("SCRIPT_NAME", uri.substr(0, uri.find('?')).c_str(), 1);
		setenv("SERVER_NAME", req_header.find("host")->second.c_str(), 1);
		setenv("SERVER_PORT", std::to_string(server_port).c_str(), 1);
		setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
		setenv("SERVER_SOFTWARE", "webserv/1.0", 1);

		dup2(script_stdin, 0);
		// close(script_stdin);
		dup2(dialogue->client_fd, 1);
		// close(dialogue->client->getFD());

		char* const	argv[3] = { const_cast<char *>(script_path.c_str()), getenv("SCRIPT_NAME"), NULL };

		if (execv(script_path.c_str(), argv) == -1)
			throw SystemCallError("execv");
	}
	else
	{
		close(script_stdin);
		EventHandlerInstance::getInstance().enableWriteEvent(getFD());
	}
}

CGI::~CGI()
{
	close(script_stdin);
}

void
	CGI::readEvent(long read_size)
{
	(void)read_size;
	throw UnexceptedEventOccured("CGI Read event");
}

void
	CGI::writeEvent(long write_size)
{
	std::string	&write_buffer = dialogue->req.getBody();
	ssize_t		write_byte;

	if ((write_byte = write(getFD(), &write_buffer[0], write_size)) == -1)
		throw SystemCallError("write");
	else if ((size_t)write_byte == write_buffer.size())
		dialogue->req.getBody().clear();
	else
		dialogue->req.getBody().erase(0, write_byte);

	if (dialogue->req.getBody().size() > 0)
		EventHandlerInstance::getInstance().enableWriteEvent(getFD());
}

void
	CGI::timerEvent()
{
	throw UnexceptedEventOccured("CGI Timer event");
}
