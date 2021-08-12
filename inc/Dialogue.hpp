#ifndef DIALOGUE_HPP
# define DIALOGUE_HPP

# include "Request.hpp"
# include "Response.hpp"

class	Dialogue
{
public:
	enum	Status
	{
		ReadingRequest,
		Processing,
		ReadyToResponse,
		WritingResponse,
	};

	Dialogue() : status(ReadingRequest) {}
	~Dialogue() {}

	Request		&getReq();
	Response	&getRes();

	void		setStatus(Status status);
	Status		getStatus();

private:
	Dialogue(const Dialogue &other);

	Dialogue&	operator=(const Dialogue &other);

	Request		req;
	Response	res;
	Status		status;
};

#endif