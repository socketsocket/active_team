

#include "webserv.hpp"

static std::string
	pop_front(std::deque<std::string> *config_deque)
{
	if (config_deque->empty() == true)
		throw ServerManager::parseException("Error occured: unexcepted EOF");

	std::string	return_value = config_deque->front();
	config_deque->pop_front();
	return (return_value);
}

template <typename InputIt>
static void
	string_tolower(InputIt first, InputIt last)
{
	for (InputIt itr = first; itr != last; itr++)
		*first = std::tolower(*first);
}

ServerManager::ServerManager(std::string config_path)
{
	std::ifstream	config_file(config_path);
	std::deque<std::string>	config_deque;
	std::string	word;

	while (config_file >> word)
	{
		string_tolower(word.begin(), word.end());
		if (word[0] == '#')
			std::getline(config_file, word);
		else
			config_deque.push_back(word);
	}
	while (config_deque.empty() == false)
	{
		if (config_deque.front() == "server")
		{
			config_deque.pop_front();
			if (config_deque.front() != "{")
				throw (parseException("Server Parsing Error : no exist '{'"));
			else
			{
				config_deque.pop_front();
				servers.push_back(parseServer(&config_deque));
			}
		}
		else
			throw (parseException("No Server Block"));
	}
}

Server
	*ServerManager::parseServer(std::deque<std::string> *config_deque)
{
	Server	*serv = new Server();

	std::string					server_name = "";
	std::queue<unsigned int>	ports;
	std::string					word;

	bool	exist_body_limit = false;
	bool	exist_auto_index = false;

	while ((word = pop_front(config_deque)) != "}")
	{
		if (word == "listen")
			ports.push(std::stoul(pop_front(config_deque)));
		else if (word == "server_name")
			server_name = pop_front(config_deque);
		else if (word == "client_body_limit")
		{
			if (exist_body_limit == true)
				throw ServerManager::parseException("Server Parsing Error: double client_body_limit input");
			exist_body_limit = true;
			serv->setBodyLimit(std::stoul(pop_front(config_deque)));
		}
		else if (word == "auto_index")
		{
			if (exist_auto_index == true)
				throw ServerManager::parseException("Server Parsing Error: double auto_index input");
			exist_auto_index = true;
			serv->setAutoindex(pop_front(config_deque));
		}
		else if (word == "error_page")
		{
			int			error_code = std::stoi(pop_front(config_deque));
			std::string	error_page = pop_front(config_deque);

			serv->addErrorPage(error_code, error_page);
		}
		else if (word == "location")
		{
			std::string	location_path = pop_front(config_deque);

			if (config_deque->front() != "{")
				throw ServerManager::parseException("Location Parsing Error : no exist '{'");
			else
				serv->addLocation(location_path, parseLocation(config_deque, location_path));
		}
		else
			throw ServerManager::parseException(word + ": unexcepted direction");

		if (word != "location" && pop_front(config_deque) != ";")
			throw ServerManager::parseException("Excepted ';'");
	}

	if (word != "}")
		throw ServerManager::parseException("Server Parsing Error : no exist '}");

	for (; ports.empty() == false; ports.pop())
		this->uri_to_server.insert(make_pair(make_pair(server_name, ports.front()), serv));
	return (serv);
}

Location
	ServerManager::parseLocation(std::deque<std::string> *config_deque, std::string location_path)
{
	Location	loc(location_path);
	std::string	word;
	bool		exist_auto_index = false;
	
	while ((word = pop_front(config_deque)) != "}")
	{
		if (word == "root")
		{
			if (loc.setRoot(pop_front(config_deque)) == FAILED)
				throw ServerManager::parseException("Location Parse Error: double root setting");
		}
		else if (word == "index")
		{
			loc.addIndex(pop_front(config_deque));
		}
		else if (word == "auto_index")
		{
			if (exist_auto_index == true)
				throw ServerManager::parseException("Location Parse Error: double autoindex setting");
			exist_auto_index = true;
			loc.setAutoindex(pop_front(config_deque));
		}
		else if (word == "error_page")
		{
			if (loc.addErrorPage(stoi(pop_front(config_deque)), pop_front(config_deque)) == FAILED)
				throw ServerManager::parseException("Location Parse Error: invalid error page");
		}
		else if (word == "methods_allowed")
		{
			std::string	method;
			while ((method = pop_front(config_deque) ) != ";")
			{
				if (method == "GET")
					loc.addMethod(Request::GET);
				else if (method == "POST")
					loc.addMethod(Request::POST);
				else if (method == "DELETE")
					loc.addMethod(Request::DELETE);
			}
		}
		else if (word == "cgi_info")
			loc.addCGI(pop_front(config_deque), pop_front(config_deque));
		else
			throw ServerManager::parseException(word + ": unexcepted direction");

		if (word != "error_page" && pop_front(config_deque) != ";")
			throw ServerManager::parseException("Excepted ';'");
	}

	if (word != "}")
		throw ServerManager::parseException("Server Parsing Error : no exist '}");
	return (loc);
}

void exit_with_perror(const std::string message)
{
	perror(message.c_str());
	exit(1);
}

static void
	print_open_status(std::string server_name, unsigned int server_port)
{
	std::cout << server_name << ":" << server_port << " started" << std::endl;
	return ;
}

int	ServerManager::openPort()
{
	typedef	std::map<std::pair<std::string, unsigned int>, Server *>::iterator	portmap_iterator;

	std::set<unsigned int>		open_port;

	if ((this->kq = kqueue()) == -1)
		return (perror("kqueue error"), 1);

	for (portmap_iterator itr = uri_to_server.begin(); itr != uri_to_server.end(); itr++)
	{
		const unsigned int	port = itr->first.second;

		if (open_port.find(port) != open_port.end())
		{
			print_open_status(itr->first.first, port);
			continue ;
		}

		int					server_socket;
		struct sockaddr_in	server_addr;

		server_socket = socket(PF_INET, SOCK_STREAM, 0);
		if (server_socket == -1)
			return (perror("socket error"), 1);

		memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		server_addr.sin_port = htons(port);

		if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
			return (perror("socket error"), 1);

		if (listen(server_socket, 5) == -1)
			return (perror("socket error"), 1);

		fcntl(server_socket, F_SETFL, O_NONBLOCK);

		struct kevent	event;
		EV_SET(&event, server_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
		kevent(kq, &event, 1, NULL, 0, NULL);

		open_port.insert(port);
		server_socket_to_port.insert(std::make_pair(server_socket, port));

		print_open_status(itr->first.first, port);
	}
}

void
	ServerManager::deleteClient(int client_socket)
{
	delete client_socket_to_client.find(client_socket)->second;
	client_socket_to_client.erase(client_socket);
}


int	ServerManager::monitorEvent()
{
	int							num_events;
	struct kevent				*curr_event;
	std::vector<struct kevent>	change_list;
	struct kevent				event_list[EVENTSIZE];
	struct kevent				temp_event;           // client read/write 등록용

	while (true)
	{
		// if (change_list.size() == 0)
		// 	num_events = kevent(kq, NULL, 0, event_list, EVENTSIZE, NULL);
		// else
		num_events = kevent(kq, &change_list[0], change_list.size(), event_list, EVENTSIZE, NULL);
		if (num_events == -1)
			return (perror("kevent error"), 1);

		change_list.clear();

		// 6
		// 6q 6q 6q 6q 6q
		//    6q 6q 6q 6q
		//    6q

		// 6q -> client ->req complete->serverManager->server->location -> ".html" "read"


		for (int i = 0; i < num_events; ++i)
		{
			curr_event = &event_list[i];
			if (curr_event->flags & EV_ERROR)
			{
				if (server_socket_to_port.find(curr_event->ident) != server_socket_to_port.end())
					return (perror("server socket error"), 1);
				else
				{
					deleteClient(curr_event->ident);
					return (perror("client socket error"), 1);
				}
			}
			else if (curr_event->filter == EVFILT_READ)
			{
				if (server_socket_to_port.find(curr_event->ident) != server_socket_to_port.end())
				{
					//이거 포인터임 주의
					Client *client = new Client(curr_event->ident, server_socket_to_port.find(curr_event->ident)->second);

					client_socket_to_client.insert(std::make_pair(client->getSocketFD(), client));

					EV_SET(&temp_event, client->getSocketFD(), EVFILT_READ, EV_ADD, 0, 0, NULL);
					change_list.push_back(temp_event);
					EV_SET(&temp_event, client->getSocketFD(), EVFILT_WRITE, EV_ADD, 0, 0, NULL);
					// EVFILT_WRITE: Takes a file descriptor as the identifier, and returns whenever it is possible to write to the descriptor.
					change_list.push_back(temp_event);
				}
				else if (client_data.find(curr_event->ident) != client_data.end())
				{
					// 클라이언트가 요청을 처리
					//  RawRequestReader를 통해
					//클라이언트에게 알려줘서 RawRequestReader 를 불러서 처리!
					char	buffer[1024];
					int		n;

					n = read(curr_event->ident, buffer, sizeof(buffer) - 1);
					if (n == -1)
					{
						std::cerr << "read error" << std::endl;
						continue;
					}
					else if (n == 0)
					{
						std::cout << "client disconnected" << std::endl;
						client_data.erase(curr_event->ident);
						close(curr_event->ident);
					}
					else
					{
						buffer[n] = '\0';
						client_data[curr_event->ident] += buffer;
					}
					raw_request 호출
				}
			}
			else if (curr_event->filter == EVFILT_WRITE)
			{
				if (//client.getStatus == "RESPONSE_COMPLETE")
				{
					int	n;
					if((n = write(curr_event->ident, client_data[curr_event->ident].c_str(), client_data[curr_event->ident].length())) == -1)
						std::cerr << "write error" << '\n';
					client_data[curr_event->ident].clear();
				}
			}
		}
	}
	return (0);
}