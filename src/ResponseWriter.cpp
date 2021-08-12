#include "ResponseWriter.hpp"

ResponseWriter::ResponseWriter()
{
}

ResponseWriter::ResponseWriter(const ResponseWriter &ref)
{
	*this = ref;
}

ResponseWriter::~ResponseWriter()
{
}

ResponseWriter& ResponseWriter::operator=(const ResponseWriter &ref)
{
	return (*this);
}

void	ResponseWriter::holdResponse(Response *res)
{
	this->entire_response += res->getStartLine();
	this->entire_response += "\r\n";
	
	for (std::map<std::string, std::string>::iterator iter = res->getHeaders().begin(); iter != res->getHeaders().end(); iter++)
	{
		this->entire_response += iter->first;
		this->entire_response += ": ";
		this->entire_response += iter->second;
		this->entire_response += "\r\n";
	}
	this->entire_response += "\r\n";

	this->entire_response += res->getBody();
	
}
