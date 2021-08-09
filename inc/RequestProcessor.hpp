#ifndef REQUESTPROCESSOR_HPP
# define REQUESTPROCESSOR_HPP

# include "Client.hpp" //status 때문에 일단
# include "Response.hpp"
# include "PortManager.hpp"

class	RequestProcessor
{
public:
	RequestProcessor(PortManager *port_manager);
	~RequestProcessor();

	RequestProcessor&	operator=(const RequestProcessor &other);

	void		prepareResponse();

	Response	*getResponse();

private:
	RequestProcessor();
	RequestProcessor(const RequestProcessor &other);

	PortManager	*pm;
	Request		*req;
	Response	*res;

};

#endif