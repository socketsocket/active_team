#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <queue>
# include <string>

# include "FDManager.hpp"
# include "PortManager.hpp"
# include "RequestReader.hpp"
# include "ResponseWriter.hpp"
# include "Dialogue.hpp"

class	Client : public FDManager
{
public:
	Client(PortManager *pm);
	~Client();

	virtual void	readEvent();
	virtual void	writeEvent();

private:
	Client();
	Client(const Client& other);

	Client&	operator=(const Client& other);

	int	openSocket(int server_socket);

	PortManager				*pm;
	RequestReader			reader;
	ResponseWriter			writer;
	std::queue<Dialogue *>	dialogues;
};

#endif
