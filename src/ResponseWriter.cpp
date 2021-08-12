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

void	ResponseWriter::pushResponse(Response &res)
{
	std::string	entire_response;

	entire_response += res.getStartLine();
	entire_response += "\r\n";
	
	for (std::map<std::string, std::string>::iterator iter = res.getHeaders().begin(); iter != res.getHeaders().end(); iter++)
	{
		entire_response += iter->first;
		entire_response += ": ";
		entire_response += iter->second;
		entire_response += "\r\n";
	}
	entire_response += "\r\n";

	entire_response += res.getBody();

	this->buffer.push_back(entire_response);
	
}
