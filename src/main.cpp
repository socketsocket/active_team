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

	// if (manager->openPort() == FAILED) || manager->monitorEvent() == 1)
	// {
	// 	delete manager;
	// 	return (perror("Failed to start"), 1);
	// }
	return (0);
}
