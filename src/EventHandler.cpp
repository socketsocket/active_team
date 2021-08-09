#include <sys/event.h>	// kevent

#include <cstdlib>	// atoi

#include <fstream>
#include <queue>

#include "EventHandler.hpp"
#include "Exception.hpp"

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
			{
				server->addLocation(location_path, parseLocation(config_queue));
			}
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

		if (front_pop(config_queue) != ";")
			throw NoExpectedDirective(";");
	}

	if (ports.size() == 0)
		ports.push_back(80);
	return (server);
}

static Location
	*parseLocation(std::queue<std::string> &config_queue)
{
	Location	*location = new Location;

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
		else if (word == "allowed_method")
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
		else
			throw BadDirective(word);

		if (front_pop(config_queue) != ";")
			throw NoExpectedDirective(";");
	}
	return (location);
}

static void
	connectPortWithServer(
		std::map<int, PortManager *> &portManagers,
		std::vector<int> &ports,
		std::vector<std::string> &names,
		Server *server )
{
	for (std::vector<int>::iterator port_itr = ports.begin(); port_itr != ports.end(); ++port_itr)
	{
		std::map<int, PortManager *>::iterator	foundPM;

		foundPM = portManagers.find(*port_itr);
		if (foundPM == portManagers.end())
		{
			foundPM = portManagers.insert(std::make_pair(*port_itr, new PortManager(*port_itr))).first;
		}
		for (	std::vector<std::string>::iterator name_itr = names.begin();
				name_itr != names.end();
				++name_itr)
		{
			foundPM->second->addServer(*name_itr, server);
		}
	}
}

EventHandler::EventHandler(std::string config_file_path)
{
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
				connectPortWithServer(portManagers, ports, names, server);
			}
		}
		else
			throw BadDirective(config_queue.front());
	}
}

void
	EventHandler::start()
{
	if ((kq = kqueue()) == -1)
		throw SystemCallError("kqueue");

	for (	std::map<int, PortManager *>::iterator itr = portManagers.begin();
			itr != portManagers.end();
			++itr)
	{
		registerFD(itr->second);
		addReadEvent(itr->second->getFD());
	}

	struct kevent	event_list[MAX_EVENT_SIZE];
	struct kevent	*curr_event;
	FDHandler		*fd_handler;

	while (true)
	{
		int	num_of_event;

		if ((num_of_event = kevent(kq, NULL, 0, event_list, MAX_EVENT_SIZE, NULL)) == -1)
			throw SystemCallError("kevent");

		for (int i = 0; i < num_of_event; ++i)
		{
			curr_event = event_list + i;
			fd_handler = fds[curr_event->ident];
			if (dynamic_cast<PortManager *>(fd_handler))
			{
				PortManager	*pm = dynamic_cast<PortManager *>(fd_handler);

				if (curr_event->filter & EVFILT_READ)
				{
					int			client_fd = pm->acceptClient();

					registerFD(new Client(client_fd, pm));
					addReadEvent(client_fd);
				}
				else
					throw UnknownEventIdentifier();
			}
			else if (dynamic_cast<Client *>(fd_handler))
			{
				Client	*client = dynamic_cast<Client *>(fd_handler);

				if (curr_event->filter & EVFILT_READ)
				{
					client->readRequest();
				}
				else if (curr_event->filter & EVFILT_WRITE)
				{
					client->writeResponse();
				}
				else if (curr_event->filter & EVFILT_TIMER)
				{
					// client->write408();
					unregisterFD(client->getFD());
					delete client;
				}
				else
					throw UnknownEventIdentifier();
			}
			else if (dynamic_cast<Resource *>(fd_handler))
			{
				Resource	*resource = dynamic_cast<Resource *>(fd_handler);

				if (curr_event->filter & EVFILT_READ)
				{
					resource->readResource();
				}
				else if (curr_event->filter & EVFILT_WRITE)
				{
					resource->writeResource();
				}
				else
					throw UnknownEventIdentifier();
			}
			else
				throw UnknownEventIdentifier();
		}
	}
}