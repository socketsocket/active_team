#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "EventHandlerInstance.hpp"
#include "Dialogue.hpp"
#include "CGI.hpp"

static int
	openReadablePipe(int &writable_pipe)
{
	int	fildes[2];

	if (pipe(fildes) == -1)
		throw SystemCallError("pipe");
	writable_pipe = fildes[1];
	return (fildes[0]);
}

static int
	openWritablePipe(int &readable_pipe)
{
	int	fildes[2];

	if (pipe(fildes) == -1)
		throw SystemCallError("pipe");
	readable_pipe = fildes[0];
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
	getQueryString(std::string &uri)
{
	std::string::size_type	query_start;

	query_start = uri.find('?');
	if (query_start == std::string::npos)
		return ("");
	else
		return (uri.substr(query_start));
}

static std::string
	getAddr(int socket_fd)
{
	struct sockaddr_in	client_addr;
	socklen_t			addr_len = sizeof(struct sockaddr_in);
	char				client_ip_addr[16];

	if (getsockname(socket_fd, (struct sockaddr *)&client_addr, &addr_len) == -1)
		throw SystemCallError("getsockname");
	strncpy(client_ip_addr, inet_ntoa(client_addr.sin_addr), 16);
	return (client_ip_addr);
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
	case Request::NON:
		assert(false);
	}
}

static std::string
	getPathInfo(std::string &script_path)
{
	return (script_path.substr(0, script_path.rfind('/')));
}

static void
	setEnvironmentVariables(
		std::map<std::string, std::string> &req_header,
		std::string &script_path,
		std::string &uri,
		Dialogue *dialogue,
		int server_port)
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
	setenv("PATH_INFO", getPathInfo(script_path).c_str(), 1);
	setenv("PATH_TRANSLATED", "", 1);
	setenv("QUERY_STRING", getQueryString(uri).c_str(), 1);
	setenv("REMOTE_ADDR", getAddr(dialogue->client_fd).c_str(), 1);
	setenv("REMOTE_HOST", "", 1);
	setenv("REMOTE_IDENT", "", 1);
	setenv("REMOTE_USER", "", 1);
	setenv("REQUEST_METHOD", getMethod(dialogue->req.getMethod()).c_str(), 1);
	setenv("SCRIPT_NAME", uri.substr(0, uri.find('?')).c_str(), 1);
	setenv("SERVER_NAME", req_header.find("host")->second.c_str(), 1);
	setenv("SERVER_PORT", std::to_string(server_port).c_str(), 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("SERVER_SOFTWARE", "hsonseyu/1.1", 1);
}

CGI::CGI(std::string &script_path, std::string &resource_path, Dialogue *dialogue, int server_port)
	: reader(dialogue),
	  writer(dialogue),
	  script_path(script_path),
	  resource_path(resource_path),
	  dialogue(dialogue),
	  server_port(server_port)
{
	struct stat	file_status;

	if (stat(script_path.c_str(), &file_status) == -1)
		throw Conflict();
}

CGI::~CGI()
{}

void
	CGI::start()
{
	int	cgi_stdin = writer.getReadablePipe();
	int	cgi_stdout = reader.getWritablePipe();

	if ((script_pid = fork()) == -1)
		throw SystemCallError("fork");
	else if (script_pid == 0)
	{
		setEnvironmentVariables(dialogue->req.getHeaders(), script_path, dialogue->req.getUri(), dialogue, server_port);
		dup2(cgi_stdin, 0);
		close(cgi_stdin);
		dup2(cgi_stdout, 1);
		close(cgi_stdout);

		if (execl(script_path.c_str(), script_path.c_str(), resource_path.c_str(), NULL) == -1)
			throw SystemCallError("execl");
	}
	else
	{
		close(cgi_stdin);
		close(cgi_stdout);
		EventHandlerInstance::getInstance().enableReadEvent(reader.getFD());
		EventHandlerInstance::getInstance().enableWriteEvent(writer.getFD());
	}
}

CGI::CGIWriter::CGIWriter(Dialogue *dialogue)
	: FDManager(openWritablePipe(readable_pipe)),
	  dialogue(dialogue)
{}

CGI::CGIWriter::~CGIWriter()
{}

void
	CGI::CGIWriter::readEvent(long read_size)
{
	(void)read_size;
	throw UnexceptedEventOccured("CGIWriter read event occured");
}

void
	CGI::CGIWriter::writeEvent(long write_size)
{
	std::string	&target = dialogue->req.getBody();
	write_size = (target.length() < (size_t)write_size) ? target.length() : write_size;
	ssize_t		write_bytes = write(getFD(), &target[0], write_size);

	if (write_bytes == -1)
		throw SystemCallError("write");
	else if ((size_t)write_bytes == target.size())
	{
		target.clear();
	}
	else
	{
		target.erase(0, write_bytes);
		EventHandlerInstance::getInstance().enableWriteEvent(this->getFD());
	}
}

CGI::CGIReader::CGIReader(Dialogue *dialogue)
	: FDManager(openReadablePipe(writable_pipe)),
	  dialogue(dialogue)
{}

CGI::CGIReader::~CGIReader()
{}

void
	CGI::CGIReader::readEvent(long read_size)
{
}

void
	CGI::CGIReader::writeEvent(long write_size)
{
	(void)write_size;
	throw UnexceptedEventOccured("CGIReader write event occured");
}
