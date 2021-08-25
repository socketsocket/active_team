#include <unistd.h>

#include <iostream>

int	main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cout << "Status: 500" << "\r\n";
		return (1);
	}
	std::cout << "Content-Type: text/html" << "\r\n";
	std::cout << "\r\n";
	fflush(stdout);
	return (execl("/usr/bin/python", "/usr/bin/python", argv[1], NULL));
}
