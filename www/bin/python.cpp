#include <unistd.h>

#include <iostream>

int	main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cout << "Status: 500" << std::endl;
		return (1);
	}
	std::cout << "Content-Type: text/html" << std::endl;
	std::cout << std::endl;
	return (execl("/usr/bin/python", "/usr/bin/python", argv[1], NULL));
}
