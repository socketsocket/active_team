#ifndef PORTMANAGER_HPP
# define PORTMANAGER_HPP

# include <iostream>
# include <string>

# include <map>
# include <vector>
# include <deque>
# include <queue>

# include <string>	// std::string
# include <fstream>	// std::ifstream
# include <cctype>	// std::tolower
# include <algorithm>// std::transform , for_each

# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <exception>

# include "Client.hpp"
# include "Server.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "ServerManager.hpp"

# define SUCCESS 1
# define FAILED  0

# define EVENTSIZE 10

class PortManager
{
private:
	

public:
	PortManager(std::string config_path);
	PortManager(const PortManager& other);
	~PortManager() {};

	PortManager &operator=(const PortManager& other);

	class	parseException : public std::exception
	{
	public:
		parseException(std::string error_message) : error_message(error_message) {}
		virtual ~parseException() throw()	{ return ; }

		virtual const char* what() const throw() 
		{
			return error_message.c_str();
		}
	private:
		std::string	error_message;
	};

	//필요
	Server		getServer(std::string hostname);
};

//throw exception 우선은 standard error

#endif
