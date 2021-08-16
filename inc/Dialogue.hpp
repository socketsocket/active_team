#ifndef DIALOGUE_HPP
# define DIALOGUE_HPP

class	Client;

# include "Client.hpp"
# include "Request.hpp"
# include "Response.hpp"

struct	Dialogue
{
public:
	enum	Status
	{
		READING_REQUEST,
		PROCESSING,
		WRITEING_RESPONSE,
		READY_TO_RESPONSE,
		READY_TO_WRITE,
	};

	Dialogue(int client_fd) : client_fd(client_fd), status(READING_REQUEST) {}
	~Dialogue() {}

	int			client_fd;
	Request		req;
	Response	res;
	Status		status;
private:
	Dialogue();
	Dialogue(const Dialogue &other);
};

#endif
