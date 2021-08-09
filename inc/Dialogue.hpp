#ifndef DIALOGUE_HPP
# define DIALOGUE_HPP

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
	};

	Dialogue() : status(ReadingRequest) {}
	~Dialogue() {}

	Request		req;
	Response	res;
	Status		status;
};

#endif
