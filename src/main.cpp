#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include "webserv.hpp"

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



ServerManager::ServerManager(std::string config_path)
{
	std::ifstream	config_file(config_path);


	while (parsingServer(config_file) == true);
}

bool parsingServer(std::ifstream config_file)
{
	std::stringstream ss;
	std::string		buf;
	std::string		word;

	while (std::getline(config_file, buf))
	{
		ss.str(buf);

		while (ss >> word)
		{
			if (word[0] == '#')
				break ;
			else if (word == "server_name") {}
			else if (word == "listen") {}
			else if (word == "error_page") {}
			else if (word == "client_body_limit") {}
			else if (word == "auto_index") {}
			else if (word == "methods_allowed") {}
			else if (word == "") {}
			else if (word == "") {}
			else if (word == "") {}
			else if (word == "") {}
			else if (word == "") {}
			
			else if (word == "location")
			{
				std::string	path;
				ss >> path; // -> path

				ss >> word; // -> '{'
				if (word != '{')
					throw ();
				// parsingLocation(config_file, path);
			}
		}
	}
}

while ( (ret = get_next_line(fd, &line)) > 0 )
	{
		split_ret = ft_split(line, " \t");
		idx = -1;
		while (split_ret[++idx] != 0)
		{
			if (!strcmp(*split_ret, "server_name"))
			{
				setServerName(split_ret[++idx]);

				if (!strcmp(split_ret[idx], "localhost"))
					setIP("127.0.0.1");
				else
					setIP("non");
			}
			else if (!strcmp(*split_ret, "listen"))
				setPort(atoi(split_ret[++idx]));
			else if (!strcmp(*split_ret, "location"))
			{

				location loc;

				loc_name = split_ret[++idx];
				locations[loc_name] = loc;
				break;
			}
			else if (!strcmp(*split_ret, "root"))
			{
				locations[loc_name].setRoot(split_ret[++idx]);
			}
			else if (!strcmp(*split_ret, "index"))
			{
				while (split_ret[++idx] != '\0')
					locations[loc_name].getIndex().push_back(split_ret[idx]);
			}
			else if (!strcmp(*split_ret, "methods_allowed"))
			{
				while (split_ret[++idx] != '\0')
					locations[loc_name].getAllowMethods().push_back(split_ret[idx]);
			}
			else if (!strcmp(*split_ret, "client_body_limit"))
			{
				locations[loc_name].setClientBodyBufferSize(atoi(split_ret[++idx]));
			}
			else
				continue;
		}
		idx = -1;
		while (split_ret[++idx] != 0)
			free(split_ret[idx]);
		free(split_ret);
		free(line);
	}
	free(line);