#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include <vector>
# include <string>

# include "FDHandler.hpp"
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

	//임시로
	void	enableReadEvent(int);
	void	enableWriteEvent(int);
	void	registerFD(int);
	void	unregisterFD(int);
	void	setTimerEvent(int);

	void	start();

private:
	EventHandler();

	void	registFD(FDHandler *handler);

	void	addReadEvent(int fd);
	void	addWriteEvent(int fd);
	void	addTimerEvent(int fd, int time);

	void	deleteReadEvent(int fd);
	void	deleteWriteEvent(int fd);
	void	deleteTimerEvent(int fd);

	std::vector<FDHandler *>		fds;
	std::vector<Server *>			servers;
	std::map<int, PortManager *>	portManagers;
	int								kq;

	void	registerFD(FDHandler *);
	void	unregisterFD(int);

};

#endif
