#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include <iostream>
# include <string>

# include <map>
# include <vector>

# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>

# include "Server.hpp"

class ServerManager
{
private:
	// int kq;
	// struct kevent return_events[1024];

	std::vector<Server *>	servers;
	std::map<std::pair<std::string, unsigned int>, Server *>
							port_map; // Host == server_name, port
	std::map<int, int>		server_sockets; // KEY -> socket, VALUE -> port
	std::map<int, Client>	clients;

	ServerManager();

	Server		*parseServer(std::deque<std::string> *config_deque);
	Location	parseLocation(std::deque<std::string> *config_deque);

public:
	ServerManager(std::string config_path);
	ServerManager(const ServerManager& other);
	~ServerManager();

	ServerManager &operator=(const ServerManager& other);

	class	parseException : public std::exception {
	public:
		parseException(std::string error_message) : error_message(error_message) {}

		virtual const char* what() const throw() { return error_message.c_str(); }
	private:
		std::string	error_message;
	};

	int start();
	int stop(); //signal

	Response makeResponse(Request req);

	bool parsingServer(std::ifstream config_file);
	bool makeServer(std::multimap<std::string, std::vector<std::string> > server_info);

};

//throw exception 우선은 standard error

#endif
