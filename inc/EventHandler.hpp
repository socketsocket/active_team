#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include <vector>
# include <string>

# include "FDManager.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "Dialogue.hpp"
# include "PortManager.hpp"

# define MAX_LINE_LENGTH	80
# define MAX_EVENT_SIZE		10

class	EventHandler
{
public:
	EventHandler(std::string config_file);
	EventHandler(const EventHandler& other);
	~EventHandler();

	EventHandler&	operator=(const EventHandler& other);

	void	start();

	void	registerFD(FDManager *fdm);
	void	unregisterFD(FDManager *fdm);

	void	enableReadEvent(int fd);
	void	enableWriteEvent(int fd);
	void	setTimerEvent(int fd, int second);

private:
	EventHandler();

	std::vector<FDManager *>		fds;
	std::vector<Server *>			servers;
	std::map<int, PortManager *>	portManagers;
	int								kq;
};

#endif