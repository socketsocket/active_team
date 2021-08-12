#ifndef RESPONSEWRITER_HPP
# define RESPONSEWRITER_HPP

# include "Response.hpp"
# include "Dialogue.hpp"

class	ResponseWriter
{
public:
	ResponseWriter(Dialogue *dialogue_to_write);
	~ResponseWriter();

	void	holdResponse(Response *res);
	bool	writeResponse();

private:
	ResponseWriter();
	ResponseWriter(const ResponseWriter &other);

	ResponseWriter&	operator=(const ResponseWriter &other);

	std::vector<std::string>	buffer;

	std::string	entire_response;
};

#endif
