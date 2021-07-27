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

	std::map<std::pair<std::string, int>, Server> servers;
	std::map<int, Client> clients;
	std::map<int, int> server_sockets; // KEY -> socket, VALUE -> port

	ServerManager();

public:
	ServerManager(std::string config_path);
	ServerManager(const ServerManager& req);
	~ServerManager();

	ServerManager &operator=(const ServerManager &other);

	int start();
	int stop(); //signal

	Response makeResponse(Request req);
};

#endif


