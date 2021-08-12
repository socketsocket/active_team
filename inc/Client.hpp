#ifndef CLIENT_HPP
# define CLIENT_HPP

# define BUFFER_SIZE 65536

# include <queue>
# include <sstream>
# include <string>
# include <iostream>
# include <fcntl.h>

# include "FDManager.hpp"
# include "RequestReader.hpp"
# include "ResponseWriter.hpp"
# include "Dialogue.hpp"
# include "CGI.hpp"
# include "Location.hpp"
# include "PortManager.hpp"

class	Client : public FDManager
{
public:
	Client(PortManager *pm);
	~Client();

	Client&	operator=(const Client& other);

	virtual void	readEvent();
	virtual void	writeEvent();
	virtual void	timerEvent();

	enum	Status {
		REQUEST_COMPLETE,
		MAKING_RESPONSE,
		NEED_RESOURCE,
		SENDING_BODYR,
		RESPONSE_COMPLETE
	};

private:
	Client();
	Client(const Client& other);
	Client&	operator=(const Client& other);

	void 	prepareResponse(PortManager *pm, Dialogue *dial);
	int		isCGIRequest(Request &req, Location &location);


	int	openSocket(int server_socket);

	PortManager				*pm;
	RequestReader		reader;
	ResponseWriter		writer;

	std::queue<Dialogue *>	dialogues;
};

#endif
