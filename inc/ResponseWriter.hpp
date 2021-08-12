#ifndef RESPONSEWRITER_HPP
# define RESPONSEWRITER_HPP

# include "Response.hpp"
# include "Dialogue.hpp"

class	ResponseWriter
{
public:
	ResponseWriter(Dialogue *dialogue_to_write);
	~ResponseWriter();

	void	pushResponse(Response &res);
	bool	emptyBuffer();
	void	writeResponse();

private:
	ResponseWriter();
	ResponseWriter(const ResponseWriter &other);

	ResponseWriter&	operator=(const ResponseWriter &other);

	std::deque<std::string>	buffer;
};

#endif
