#ifndef PORTMANAGER_HPP
# define PORTMANAGER_HPP

# include <map>
# include <string>

# include "FDHandler.hpp"
# include "Server.hpp"

# define PORT_BACKLOG	10

class	PortManager : public FDHandler
{
public:
	/* ---------------------------------------- */
	/*         Constructor / Destructor         */
	/* ---------------------------------------- */

	PortManager(int port) : FDHandler(openSocket(port)) {}
	virtual	~PortManager();

	/* ---------------------------------------- */
	/*               Initializer                */
	/* ---------------------------------------- */

	void	addServer(std::string server_name, Server *server);

	/* ---------------------------------------- */
	/*             TCP Communicator             */
	/* ---------------------------------------- */

	int		acceptClient();	// return client fd

	Server	getServer(std::string);

private:
	/* ---------------------------------------- */
	/*         Constructor / Destructor         */
	/* ---------------------------------------- */

	PortManager();
	PortManager(const PortManager &other);

	/* ---------------------------------------- */
	/*               Initializer                */
	/* ---------------------------------------- */

	int	openSocket(int port);

	/* ---------------------------------------- */
	/*                 Operator                 */
	/* ---------------------------------------- */

	PortManager&	operator=(const PortManager &other);

	/* ---------------------------------------- */
	/*             Member Variables             */
	/* ---------------------------------------- */

	std::map<std::string, Server *>	servers;
};

#endif