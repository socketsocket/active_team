#ifndef PORTMANAGER_HPP
# define PORTMANAGER_HPP

# include <map>
# include <string>

# include "FDManager.hpp"
# include "Server.hpp"

# define PORT_BACKLOG	10

class	PortManager : public FDManager
{
public:
	PortManager(int port);
	virtual	~PortManager();

	void	addServer(std::string server_name, Server *server);

	virtual void	readEvent();
	virtual void	writeEvent();
	virtual void	timerEvent();

	Server*	getServer(std::string hostname);

private:
	PortManager();
	PortManager(const PortManager &other);

	PortManager&	operator=(const PortManager &other);

	std::vector<std::pair<std::string, Server *> >	servers; //pair(hostname, server) 로 저장하고 find_if로 찾기 - default
};

#endif
