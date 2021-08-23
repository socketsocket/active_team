#ifndef PORTMANAGER_HPP
# define PORTMANAGER_HPP

# include <map>
# include <string>

class	Server;

# include "FDManager.hpp"
# include "Server.hpp"

# define PORT_BACKLOG	200

class	PortManager : public FDManager
{
public:
	PortManager(int port);
	virtual	~PortManager();

	void	addServer(std::string server_name, Server *server);

	virtual void	readEvent(long read_size);
	virtual void	writeEvent(long write_size);
	virtual void	timerEvent();

	Server*	getServer(std::string hostname);
	int		getPort();

private:
	PortManager();
	PortManager(const PortManager &other);

	PortManager&	operator=(const PortManager &other);

	int	port;

	std::vector<std::pair<std::string, Server *> >	servers; //pair(hostname, server) 로 저장하고 find_if로 찾기 - default
};

#endif
