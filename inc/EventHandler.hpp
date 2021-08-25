#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include <map>
# include <vector>
# include <string>

class	PortManager;

# include "FDManager.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "Dialogue.hpp"
# include "PortManager.hpp"

# define MAX_EVENT_SIZE		200

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

	void	registerProcess(pid_t pid, char **envp);

	void	enableReadEvent(int fd);
	void	enableWriteEvent(int fd);
	void	disableWriteEvent(int fd);
	void	setTimerEvent(int fd);
	void	unsetTimerEvent(int fd);

private:
	EventHandler();

	std::vector<FDManager *>		fds;
	std::vector<Server *>			servers;
	std::map<int, PortManager *>	port_managers;
	std::vector<struct kevent>		new_events;
	int								kq;

	static const int	socket_timeout_in_second = 60;
	static const int	new_event_reserve_size = 200;
};

#endif
