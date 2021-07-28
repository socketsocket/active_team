#include <string>	// std::string
#include <fstream>	// std::ifstream
#include <map>		// std::map, std::multimap
#include <cctype>	// std::tolower
#include <algorithm>// std::transform
#include <queue>

#include "webserv.hpp"

using namespace std;

int	main(int argc, char *argv[])
{
	std::string	config_path;

	if (argc == 1)
		config_path = "./config/default.conf";
	else if (argc == 2)
		config_path = argv[1];
	else
	{
		std::cerr << "Too many arguments" << std::endl;
		return (1);
	}

	ServerManager *manager;
	try
	{
		manager = new ServerManager(config_path);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		delete manager;
		return (1);
	}

	if (manager->start() == 1)
	{
		delete manager;
		return (perror("Failed to start"), 1);
	}
	return (0);
}

static std::string
	pop_front(std::deque<std::string> *config_deque)
{
	if (config_deque->empty() == true)
		throw ServerManager::parseException("Error occured: unexcepted EOF");
	
	std::string	return_value = config_deque->front();
	config_deque->pop_front();
	return (return_value);
}

ServerManager::ServerManager(std::string config_path)
{
	std::ifstream	config_file(config_path);
	std::deque<std::string>	config_deque;
	std::string	word;

	while (config_file >> word)
	{
		if (word[0] == '#')
			std::getline(config_file, word);
		else
			config_deque.push_back(std::transform(word.cbegin(), word.cend(), word.begin(), std::tolower));
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

Server	*ServerManager::parseServer(std::deque<std::string> *config_deque)
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

	for (; port.empty() == false; port.pop())
		this->port_map.insert(make_pair(make_pair(server_name, port.front()), serv));
	return (serv);
}

Location	ServerManager::parseLocation(std::deque<std::string> *config_deque, std::string location_path)
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
					loc.addMethod(Request::Method::GET);
				else if (method == "POST")
					loc.addMethod(Request::Method::POST);
				else if (method == "DELETE")
					loc.addMethod(Request::Method::DELETE);
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