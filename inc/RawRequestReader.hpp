#ifndef RAWREQUESTREADER_HPP
# define RAWREQUESTREADER_HPP

# include <string>

# include "Request.hpp"

class RawRequestReader
{
private:
	std::string raw_request;
	Request 	request;
	int			status;

	friend class Client;
public:
	RawRequestReader();
	void	readStartHead();
	void	readBody();
	int		readRequest();
    
    //getter
    std::string     getRawRequest();
};

#endif
