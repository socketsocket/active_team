#include "EventHandlerInstance.hpp"
#include "EventHandler.hpp"

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

	try
	{
		EventHandler	*evh = new EventHandler(config_path);
		EventHandlerInstance::setInstance(evh);
		evh->start();
		delete evh;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}


	return (0);
}
