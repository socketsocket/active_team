#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <string>
#include "RawRequestReader.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Client
{
private:
	Client();

	int					socket_fd;
	RawRequestReader	reader;
	Response			response;
	unsigned long		last_time;
	int					status;

public:
	Client(int socket_fd);
	~Client();
	Client(const Client& c);

	Client& operator= (const Client& c);

	// getter
	Response			getResponse();
	unsigned long		getLastTime();
	int					getStatus();
};

#endif
