#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <queue>
# include <sstream>
# include <string>

# include "FDHandler.hpp"
# include "RequestReader.hpp"
# include "RequestProcessor.hpp"
# include "ResponseWriter.hpp"
# include "Dialogue.hpp"

class	Client : public FDHandler
{
public:
	Client(int socket_fd, PortManager *pm);
	~Client();

	Client&	operator=(const Client& other);

	void	readRequest();
	void	writeResponse();

private:
	Client();
	Client(const Client& other);

	RequestReader		reader;
	RequestProcessor	processor;
	ResponseWriter		writer;

	std::queue<Dialogue *>	dialogues;
};

#endif
