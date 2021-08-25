#include <sys/socket.h>
#ifdef __APPLE__
# include <sys/event.h>
#elif __linux__
# include <kqueue/sys/event.h>
#endif
#include <arpa/inet.h>
#include <unistd.h>
#include <cassert>
#include <cstring>

#include "EventHandlerInstance.hpp"
#include "Dialogue.hpp"
#include "CGI.hpp"

const ssize_t	CGI::CGIReader::MAX_OFFSET	= 8162;

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
		return (uri.substr(query_start + 1));
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
	default:
		throw BadRequest();
	}
}

static std::string
	getPathInfo(std::string &resource_path)
{
	return (resource_path.substr(0, resource_path.find('?')));
}

static char
	*ft_strjoin(const char *str1, const char *str2)
{
	size_t	strlen_str1 = strlen(str1);
	size_t	strlen_str2 = strlen(str2);

	char	*rtn = new char[strlen_str1 + strlen_str2 + 1];
	strcpy(rtn, str1);
	strcpy(rtn + strlen_str1, str2);
	rtn[strlen_str1 + strlen_str2] = '\0';
	return rtn;
}

CGI::CGI(std::string &script_path, std::string &resource_path, Dialogue *dialogue, int server_port)
	: reader(dialogue, this),
	  writer(dialogue),
	  script_path(script_path),
	  resource_path(resource_path),
	  dialogue(dialogue),
	  server_port(server_port)
{
	struct stat	file_status;

	if (stat(script_path.c_str(), &file_status) == -1 || (file_status.st_mode & 0100) == 0)
		throw Conflict();

	int	cgi_stdin = writer.getReadablePipe();
	int	cgi_stdout = reader.getWritablePipe();

	envp = new char *[32];
	envp[0] = ft_strjoin("AUTH_TYPE=", "");
	envp[1] = ft_strjoin("CONTENT_LENGTH=", findValue(dialogue->req.getHeaders(), "content-length").c_str());
	envp[2] = ft_strjoin("AUTH_TYPE=", "");
	envp[3] = ft_strjoin("CONTENT_LENGTH=", findValue(dialogue->req.getHeaders(), "content-length").c_str());
	envp[4] = ft_strjoin("CONTENT_TYPE=", findValue(dialogue->req.getHeaders(), "content-type").c_str());
	envp[5] = ft_strjoin("GATEWAY_INTERFACE=", "CGI/1.1");
	envp[6] = ft_strjoin("HTTP_ACCEPT=", findValue(dialogue->req.getHeaders(), "accept").c_str());
	envp[7] = ft_strjoin("HTTP_ACCEPT_CHARSET=", findValue(dialogue->req.getHeaders(), "accept-charset").c_str());
	envp[8] = ft_strjoin("HTTP_ACCEPT_ENCODING=", findValue(dialogue->req.getHeaders(), "accept-encoding").c_str());
	envp[9] = ft_strjoin("HTTP_ACCEPT_LANGUAGE=", findValue(dialogue->req.getHeaders(), "accept-language").c_str());
	envp[10] = ft_strjoin("HTTP_CONNECTION=", findValue(dialogue->req.getHeaders(), "connection").c_str());
	envp[11] = ft_strjoin("HTTP_COOKIE=", findValue(dialogue->req.getHeaders(), "cookie").c_str());
	envp[12] = ft_strjoin("HTTP_FORWARDED=", findValue(dialogue->req.getHeaders(), "forwarded").c_str());
	envp[13] = ft_strjoin("HTTP_HOST=", findValue(dialogue->req.getHeaders(), "host").c_str());
	envp[14] = ft_strjoin("HTTP_PROXY_AUTHORIZATION=", findValue(dialogue->req.getHeaders(), "proxy-authorization").c_str());
	envp[15] = ft_strjoin("HTTP_REFERER=", findValue(dialogue->req.getHeaders(), "referer").c_str());
	envp[16] = ft_strjoin("HTTP_USER_AGENT=", findValue(dialogue->req.getHeaders(), "user-agent").c_str());
	envp[17] = ft_strjoin("PATH_INFO=", getPathInfo(dialogue->req.getUri()).c_str());
	envp[18] = ft_strjoin("PATH_TRANSLATED=", resource_path.c_str());
	envp[19] = ft_strjoin("QUERY_STRING=", getQueryString(dialogue->req.getUri()).c_str());
	envp[20] = ft_strjoin("REMOTE_ADDR=", getAddr(dialogue->client_fd).c_str());
	envp[21] = ft_strjoin("REMOTE_HOST=", "");
	envp[22] = ft_strjoin("REMOTE_IDENT=", "");
	envp[23] = ft_strjoin("REMOTE_USER=", "");
	envp[24] = ft_strjoin("REQUEST_METHOD=", getMethod(dialogue->req.getMethod()).c_str());
	envp[25] = ft_strjoin("REQUEST_URI=", dialogue->req.getUri().c_str());
	envp[26] = ft_strjoin("SCRIPT_NAME=", dialogue->req.getUri().substr(0, dialogue->req.getUri().find('?')).c_str());
	envp[27] = ft_strjoin("SERVER_NAME=", dialogue->req.getHeaders().find("host")->second.c_str());
	envp[28] = ft_strjoin("SERVER_PORT=", std::to_string(server_port).c_str());
	envp[29] = ft_strjoin("SERVER_PROTOCOL=", "HTTP/1.1");
	envp[30] = ft_strjoin("SERVER_SOFTWARE=", "hsonseyu/1.1");
	envp[31] = NULL;

	if ((script_pid = fork()) == -1)
		throw SystemCallError("fork");
	else if (script_pid == 0)
	{
		close(reader.getFD());
		close(writer.getFD());
		dup2(cgi_stdin, 0);
		close(cgi_stdin);
		dup2(cgi_stdout, 1);
		close(cgi_stdout);

		if (execle(script_path.c_str(), script_path.c_str(), resource_path.c_str(), NULL, envp) == -1)
			throw SystemCallError("execl");
	}
	else
	{
		EventHandlerInstance::getInstance().registerProcess(script_pid, envp);

		close(cgi_stdin);
		close(cgi_stdout);

		EventHandlerInstance::getInstance().enableReadEvent(reader.getFD());
		EventHandlerInstance::getInstance().enableWriteEvent(writer.getFD());
	}
}

CGI::~CGI()
{}

CGI::CGIWriter::CGIWriter(Dialogue *dialogue)
	: FDManager(openWritablePipe(readable_pipe)),
	  dialogue(dialogue)
{}

CGI::CGIWriter::~CGIWriter()
{}

void
	CGI::CGIWriter::readEvent(long read_size, short flags)
{
	(void)flags;
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
	else if ((size_t)write_bytes == target.length())
	{
		target.clear();
		close(this->getFD());
	}
	else
	{
		target.erase(0, write_bytes);
		EventHandlerInstance::getInstance().enableWriteEvent(this->getFD());
	}
}

void
	CGI::CGIWriter::timerEvent()
{
	throw UnexceptedEventOccured("CGIWriter timer event occured");
}

int	CGI::CGIWriter::getReadablePipe()
{
	return (readable_pipe);
}

CGI::CGIReader::CGIReader(Dialogue *dialogue, CGI *cgi)
	: FDManager(openReadablePipe(writable_pipe)),
	  dialogue(dialogue),
	  cgi(cgi),
	  status(HEADER)
{}

CGI::CGIReader::~CGIReader()
{}

static void
	string_tolower(std::string &str)
{
	for (std::string::iterator itr = str.begin(); itr != str.end(); ++itr)
		*itr = std::tolower(*itr);
}

void
	CGI::CGIReader::readEvent(long read_size, short flags)
{
	read_size = std::min(MAX_OFFSET, read_size);

	std::string	buf;
	ssize_t		read_bytes;

	buf.resize(read_size);
	read_bytes = read(getFD(), &buf[0], read_size);
	this->buffer.append(buf);

	if (read_bytes == -1)
		throw SystemCallError("read");

	std::string::size_type	newline_idx;

	if (status == HEADER)
	{
		while ((newline_idx = buffer.find("\r\n")) != std::string::npos)
		{
			if (newline_idx == 0)
			{
				buffer.erase(0, 2);
				dialogue->res.makeStartLine("HTTP/1.1", dialogue->res.getStatusCode(), Server::statusMessage(dialogue->res.getStatusCode()));
				dialogue->res.addHeader("Transfer-Encoding", "chunked");

				if (dialogue->res.getHeaders().find("Content-Type") != dialogue->res.getHeaders().end())
					status = BODY;
				else if (dialogue->res.getHeaders().find("Location")->first.find("/") == 0)
				{
					int	resource_fd = open(dialogue->res.getHeaders().find("Location")->first.c_str(), O_RDONLY);

					if (resource_fd == -1)
						throw NotFound();
					Resource	*resource = new Resource(resource_fd, dialogue);
					EventHandlerInstance::getInstance().enableReadEvent(resource_fd);
					dialogue->res.setResource(resource);
					status = DONE;
				}
				else
				{
					EventHandlerInstance::getInstance().enableWriteEvent(dialogue->client_fd);
					status = DONE;
				}
				break ;
			}
			std::string	line = buffer.substr(0, newline_idx);
			buffer.erase(0, newline_idx + 2);

			std::string	key;
			std::string	value;
			std::string::size_type	colon_idx;

			Response::header_type	&headers = dialogue->res.getHeaders();

			colon_idx = line.find(':');
			key = line.substr(0, colon_idx);
			value = line.substr(colon_idx + 2);
			string_tolower(key);
			if (key == "content-type")
				headers.insert(std::make_pair("Content-Type", value));
			else if (key == "location")
				headers.insert(std::make_pair("Location", value));
			else if (key == "status")
				dialogue->res.setStatusCode(std::strtol(value.c_str(), NULL, 10));
		}
	}
	if (status == BODY)
	{
		if (buffer.length() > 0)
		{
			std::stringstream	ss;
			std::string			length_hex;

			ss << std::hex << buffer.length();
			ss >> length_hex;

			std::string	&body = dialogue->res.getBody();

			body += length_hex;
			body += "\r\n";
			body += buffer;
			body += "\r\n";
			buffer.clear();
		}
		if (flags & EV_EOF)
		{
			dialogue->res.getBody() += "0\r\n\r\n";
			EventHandlerInstance::getInstance().enableWriteEvent(dialogue->client_fd);
			status = DONE;
		}
	}
}

void
	CGI::CGIReader::writeEvent(long write_size)
{
	(void)write_size;
	throw UnexceptedEventOccured("CGIReader write event occured");
}

void
	CGI::CGIReader::timerEvent()
{
	throw UnexceptedEventOccured("CGIReader timer event occured");
}

int	CGI::CGIReader::getWritablePipe()
{
	return (writable_pipe);
}
