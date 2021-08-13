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

	Dialogue() : status(READING_REQUEST) {}
	~Dialogue() {}

	Client		*client;
	Request		req;
	Response	res;
	Status		status;
};

#endif
