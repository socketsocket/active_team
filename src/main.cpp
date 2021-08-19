#include "EventHandlerInstance.hpp"
#include "EventHandler.hpp"

EventHandler	*EventHandlerInstance::instance = NULL;

int	main(int argc, char *argv[])
{
	std::string	config_path;

	if (argc == 1)
		config_path = "./config/our.conf";
	else if (argc == 2)
		config_path = argv[1];
	else
	{
		std::cerr << "Too many arguments" << std::endl;
		return (1);
	}

	EventHandler	*evh = new EventHandler(config_path);
	evh->start();
	delete evh;

	return (0);
}
