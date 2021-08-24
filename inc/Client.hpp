#ifndef CLIENT_HPP
# define CLIENT_HPP

# define BUFFER_SIZE 65536

# include <queue>
# include <sstream>
# include <string>
# include <iostream>
# include <fcntl.h>

struct	Dialogue;
class	Location;

# include "FDManager.hpp"
# include "Dialogue.hpp"
# include "Location.hpp"
# include "PortManager.hpp"
# include "RequestReader.hpp"
# include "ResponseWriter.hpp"

class	Client : public FDManager
{
public:
	Client(PortManager *pm);
	virtual ~Client();

	virtual void	readEvent(long read_size, short flags);
	virtual void	writeEvent(long write_size);
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

	void 					prepareResponse(Dialogue *dial);
	std::string*			isCGIRequest(Request &req, Location &location);

	PortManager				*pm;
	RequestReader			reader;
	ResponseWriter			writer;

	std::queue<Dialogue *>	dialogues;
};

#endif
