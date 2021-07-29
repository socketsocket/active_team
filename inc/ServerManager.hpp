#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

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

# include "Server.hpp"

class ServerManager
{
private:
	std::vector<Server *>	servers;
	std::map<std::pair<std::string, unsigned int>, Server *>
							uri_to_server; //uri = server_name:port
	std::map<int, int>		server_socket_to_port; // serv_socket_fd -> port
	std::map<int, Client *>	client_socket_to_client; // erase 할 때 별도의 delete Client 필수
	int						kq;

	ServerManager() {}

	Server		*parseServer(std::deque<std::string> *config_deque);
	Location	parseLocation(std::deque<std::string> *config_deque, std::string location_path);
	void		deleteClient(int client_socket);

public:
	ServerManager(std::string config_path);
	ServerManager(const ServerManager& other);
	~ServerManager();

	ServerManager &operator=(const ServerManager& other);

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

	int openPort();
	int monitorEvent();

	Response makeResponse(Request req);

	bool parsingServer(std::ifstream config_file);
	bool makeServer(std::multimap<std::string, std::vector<std::string> > server_info);

};

//throw exception 우선은 standard error

#endif
