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
		ReadingRequest,
		Processing,
		WritingResponse,
		ReadyToResponse
	};

	Dialogue() : status(ReadingRequest) {}
	~Dialogue() {}

	Client		*client;
	Request		req;
	Response	res;
	Status		status;
};

#endif
