#ifndef REQUESTPROCESSOR_HPP
# define REQUESTPROCESSOR_HPP

# include "PortManager.hpp"

class	RequestProcessor
{
public:
	RequestProcessor(PortManager *port_manager);
	~RequestProcessor();

	RequestProcessor&	operator=(const RequestProcessor &other);

private:
	RequestProcessor();
	RequestProcessor(const RequestProcessor &other);

	PortManager	*pm;
	Request		*req;
	Response	*res;

};

#endif
