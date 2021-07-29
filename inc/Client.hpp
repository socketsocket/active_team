#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <string>
#include <stdio.h>

#include "RawRequestReader.hpp"
#include "Request.hpp"
#include "Response.hpp"

# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <exception>

class Client
{
private:
	Client();

	int					socket_fd;
	unsigned int		port;
	// RawRequestReader	reader;
	Response			response;
	unsigned long		last_time;
	int					status;

public:
	Client(int server_socket_fd, unsigned int port);
	// Client(int socket_fd, unsigned int port);
	~Client();
	Client(const Client& c);

	Client& operator= (const Client& c);

	class	clientException : public std::exception
	{
	public:
		clientException(std::string error_message) : error_message(error_message) {}
		virtual ~clientException() throw()	{ return ; }

		virtual const char* what() const throw() 
		{
			return error_message.c_str();
		}
	private:
		std::string	error_message;
	};
	
	// getter
	Response			getResponse()	{ return (response);	}
	unsigned long		getLastTime()	{ return (last_time);	}
	int					getStatus()		{ return (status);		}
	int					getSocketFD()	{ return (socket_fd);	}
};

#endif
