#include <sys/event.h>	// kevent

#include <cstdlib>	// atoi

#include <fstream>
#include <queue>

#include "EventHandlerInstance.hpp"
#include "EventHandler.hpp"
#include "Exception.hpp"

/* static */

static inline void
	tolower(std::string &s)
{
	for (std::string::iterator itr = s.begin(); itr != s.end(); ++itr)
		*itr = std::tolower(*itr);
}

template <typename T>
static inline T
	front_pop(std::queue<T> &queue)
{
	T	front;

	if (queue.size() == 0)
		throw (UnexceptedEOF());
	front = queue.front();
	queue.pop();
	return (front);
}

static Location
	*parseLocation(std::queue<std::string> &config_queue)
{
	Location	*location = new Location;

	if (front_pop(config_queue) != "{")
		throw NoExpectedDirective("{");
	for (std::string word; (word = front_pop(config_queue)) != "}"; )
	{
		if (word == "root")
		{
			location->setRoot(front_pop(config_queue));
		}
		else if (word == "index")
		{
			location->addIndex(front_pop(config_queue));
		}
		else if (word == "auto_index")
		{
			location->setAutoindex(front_pop(config_queue));
		}
		else if (word == "error_page")
		{
			location->setErrorPage(std::atoi(front_pop(config_queue).c_str()), front_pop(config_queue));
		}
		else if (word == "methods_allowed")
		{
			while (config_queue.front() != ";")
			{
				location->addMethod(front_pop(config_queue));
			}
		}
		else if (word == "cgi_info")
		{
			location->setCGI(front_pop(config_queue), front_pop(config_queue));
		}
		else if (word == "return")
		{
			int	code = std::atoi(front_pop(config_queue).c_str());

			if (config_queue.front() == ";")
				location->setReturnInfo(code, "");
			else
				location->setReturnInfo(code, front_pop(config_queue));
		}
		else if (word == "client_body_limit")
			location->setBodyLimit(std::atoi(front_pop(config_queue).c_str()));
		else
			throw BadDirective(word);

		if (front_pop(config_queue) != ";")
			throw NoExpectedDirective(";");
	}
	return (location);
}

static Server
	*parseServer(
		std::queue<std::string> &config_queue,
		std::vector<int> &ports,
		std::vector<std::string> &names )
{
	ports.clear();
	names.clear();

	Server	*server = new Server;

	for (std::string word; (word = front_pop(config_queue)) != "}"; )
	{
		if (word == "listen")
		{
			ports.push_back(std::atoi(front_pop(config_queue).c_str()));
		}
		else if (word == "server_name")
		{
			names.push_back(front_pop(config_queue));
		}
		else if (word == "client_body_limit")
		{
			server->setBodyLimit(std::atoi(front_pop(config_queue).c_str()));
		}
		else if (word == "auto_index")
		{
			server->setAutoindex(front_pop(config_queue));
		}
		else if (word == "error_page")
		{
			server->addErrorPage(std::atoi(front_pop(config_queue).c_str()), front_pop(config_queue));
		}
		else if (word == "location")
		{
			std::string location_path = front_pop(config_queue);

			if (config_queue.front() != "{")
				throw NoExpectedDirective("{");
			else
				server->addLocation(location_path, parseLocation(config_queue));
		}
		else if (word == "return")
		{
			int	code = std::atoi(front_pop(config_queue).c_str());

			if (config_queue.front() == ";")
				server->setReturnInfo(code, "");
			else
				server->setReturnInfo(code, front_pop(config_queue));
		}
		else
			throw BadDirective(word);

		if (word != "location" && front_pop(config_queue) != ";")
			throw NoExpectedDirective(";");
	}

	if (ports.size() == 0)
		ports.push_back(80);
	return (server);
}

static void
	connectPortWithServer(
		std::map<int, PortManager *> &port_managers,
		std::vector<int> &ports,
		std::vector<std::string> &names,
		Server *server )
{
	for (std::vector<int>::iterator port_itr = ports.begin(); port_itr != ports.end(); ++port_itr)
	{
		std::map<int, PortManager *>::iterator	foundPM;

		foundPM = port_managers.find(*port_itr);
		if (foundPM == port_managers.end())
		{
			PortManager	*pm = new PortManager(*port_itr);
			foundPM = port_managers.insert(std::make_pair(*port_itr, pm)).first;
		}
		for (	std::vector<std::string>::iterator name_itr = names.begin();
				name_itr != names.end();
				++name_itr)
		{
			foundPM->second->addServer(*name_itr, server);
		}
	}
}

/* public */

EventHandler::EventHandler(std::string config_file_path) : socket_timeout_in_second(60)
{
	EventHandlerInstance::setInstance(this);

	std::ifstream			config_file(config_file_path);
	std::queue<std::string>	config_queue;

	std::vector<int>			ports;
	std::vector<std::string>	names;

	for (std::string word; config_file >> word; )
	{
		tolower(word);

		if (word[0] == '#')
			std::getline(config_file, word);
		else
			config_queue.push(word);
	}
	while (config_queue.empty() == false)
	{
		if (config_queue.front() == "server")
		{
			config_queue.pop();
			if (config_queue.front() != "{")
				throw NoExpectedDirective("{");
			else
			{
				config_queue.pop();

				Server	*server = parseServer(config_queue, ports, names);

				servers.push_back(server);
				connectPortWithServer(port_managers, ports, names, server);
			}
		}
		else
			throw BadDirective(config_queue.front());
	}
}

EventHandler::~EventHandler()
{
	for (	std::vector<FDManager *>::iterator itr = fds.begin();
			itr != fds.end();
			++itr)
		delete *itr;
	fds.clear();
	for (	std::vector<Server *>::iterator itr = servers.begin();
			itr != servers.end();
			++itr)
		delete *itr;
	servers.clear();
	for (	std::map<int, PortManager *>::iterator itr = port_managers.begin();
			itr != port_managers.end();
			++itr)
		delete itr->second;
	port_managers.clear();
	close(kq);
}

void
	EventHandler::start()
{
	if ((kq = kqueue()) == -1)
		throw SystemCallError("kqueue");

	for (	std::map<int, PortManager *>::iterator itr = port_managers.begin();
			itr != port_managers.end();
			++itr)
	{
		enableReadEvent(itr->second->getFD());
	}

	struct kevent	event_list[MAX_EVENT_SIZE];

	while (true)
	{
		int	num_of_event;

		if ((num_of_event = kevent(kq, NULL, 0, event_list, MAX_EVENT_SIZE, NULL)) == -1)
			throw SystemCallError("kevent");

		for (struct kevent *curr_event = event_list; curr_event < (event_list + num_of_event); ++curr_event)
		{
			if ((curr_event->flags & EV_ERROR)
				|| (curr_event->flags & EV_EOF)
				|| (fds[curr_event->ident] == NULL))
			{
				delete fds[curr_event->ident];
				continue ;
			}

			if (curr_event->filter == EVFILT_READ)
				fds[curr_event->ident]->readEvent(curr_event->data);
			else if (curr_event->filter == EVFILT_WRITE)
				fds[curr_event->ident]->writeEvent(curr_event->data);
			else if (curr_event->filter == EVFILT_TIMER)
				fds[curr_event->ident]->timerEvent();
		}
	}
}

void
	EventHandler::registerFD(FDManager *fdm)
{
	if (fdm->getFD() < 0)
		throw BadFileDescriptor();
	if (fds.size() <= (size_t)fdm->getFD())
		fds.resize(fdm->getFD() + 1, NULL);
	fds[fdm->getFD()] = fdm;
}

void
	EventHandler::unregisterFD(FDManager *fdm)
{
	fds.at(fdm->getFD()) = NULL;
}

void
	EventHandler::enableReadEvent(int fd)
{
	struct kevent	temp;

	EV_SET(&temp, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if (kevent(kq, &temp, 1, NULL, 0, NULL) == -1)
		throw SystemCallError("kevent");
}

void
	EventHandler::enableWriteEvent(int fd)
{
	struct kevent	temp;

	EV_SET(&temp, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT | EV_ENABLE, 0, 0, NULL);
	if (kevent(kq, &temp, 1, NULL, 0, NULL) == -1)
		throw SystemCallError("kevent");
}

void
	EventHandler::setTimerEvent(int fd)
{
	struct kevent	temp;

	EV_SET(&temp, fd, EVFILT_TIMER, EV_ADD | EV_ENABLE, NOTE_SECONDS, socket_timeout_in_second, NULL);
	if (kevent(kq, &temp, 1, NULL, 0, NULL) == -1)
		throw SystemCallError("kevent");
}

/* private */
