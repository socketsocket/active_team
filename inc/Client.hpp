#ifndef CLIENT_HPP
# define CLIENT_HPP

// #include <iostream>
// #include <string>
// #include <stdio.h>

// #include "RawRequestReader.hpp"
// #include "Request.hpp"
// #include "Response.hpp"
// #include "PortManager.hpp"

// # include <sys/types.h>
// # include <sys/event.h>
// # include <sys/time.h>
// # include <sys/socket.h>
// # include <arpa/inet.h>
// # include <unistd.h>
// # include <fcntl.h>
// # include <exception>

# define REQUEST_COMPLETE 3
# define MAKING_RESPONSE 4
# define NEED_RESOURCE 6
# define SENDING_BODYR 5 //chunked 바디 보내는중
# define RESPONSE_COMPLETE 7

# include <queue>
# include <sstream>
# include <string>

# include "FDHandler.hpp"
# include "RequestReader.hpp"
# include "RequestProcessor.hpp"
# include "ResponseWriter.hpp"
# include "Dialogue.hpp"

class	Client : public FDHandler
{
public:
	Client(int socket_fd, PortManager *pm);
	~Client();

	Client&	operator=(const Client& other);

	void	readRequest();
	void	writeResponse();

private:
	Client();
	Client(const Client& other);

	RequestReader		reader;
	RequestProcessor	processor;
	ResponseWriter		writer;

	std::queue<Dialogue *>	dialogues;
};

// class Client
// {
// private:
// 	Client();

// 	int					socket_fd;
// 	unsigned int		port;
// 	// RawRequestReader	reader;
// 	Request				request;	//필요
// 	Response			response;
// 	// int					response_status;
// 	unsigned long		last_time;
// 	int					status;
// 	PortManager			&portmanager;	//필요
// 	// map<std::string,Server>	*portmanager;
// 	//map의 포인터가 되어서 서버매니저가 관리
// 	// vector<PortManager>		[];

// public:
// 	Client(int server_socket_fd, unsigned int port);
// 	// Client(int socket_fd, unsigned int port);
// 	~Client();
// 	Client(const Client& c);

// 	Client& operator= (const Client& c);

// 	class	clientException : public std::exception
// 	{
// 	public:
// 		clientException(std::string error_message) : error_message(error_message) {}
// 		virtual ~clientException() throw()	{ return ; }

// 		virtual const char* what() const throw() 
// 		{
// 			return error_message.c_str();
// 		}
// 	private:
// 		std::string	error_message;
// 	};

// 	void			prepareResponse();
	
// 	// getter
// 	Response			&getResponse()	{ return (response);	}
// 	unsigned long		getLastTime()	{ return (last_time);	}
// 	int					getStatus()		{ return (status);		}
// 	int					getSocketFD()	{ return (socket_fd);	}
// 	//필요
// 	unsigned int		getPort()		{ return (port);		}

// 	PortManager		&getPortManager()	{ return (portmanager);	}
// 	Request			&getRequest()		{ return (request);		}
// };



#endif
