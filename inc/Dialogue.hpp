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
		WritingResponse,
	};

	Dialogue() : req(NULL), res(NULL), status(ReadingRequest) {}
	~Dialogue() {}

	Request		*getReq();

private:
	Dialogue(const Dialogue &other);

	Dialogue&	operator=(const Dialogue &other);

	Request		*req;
	Response	*res;
	Status		status;
};

#endif