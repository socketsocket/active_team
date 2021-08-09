#ifndef REQUESTREADER_HPP
# define REQUESTREADER_HPP

# include "Dialogue.hpp"
# include "Client.hpp"

#define PARSING_HEADER 0
#define PARSING_BODY 1

#define	CHUNKED 2
#define CONTENT_LENGTH 3

class	RequestReader
{
public:
	RequestReader(int client_socket);
	~RequestReader();

	RequestReader&	operator=(const RequestReader &other);

	// void		readRequest();
	Dialogue*	parseRequest();

	Request*	makeStartLine(Request *req);
	Request* 	makeReqHeader(Request *req);
	void		checkBody(Request *req);
	Request*	makeChunkedBody(Request *req);
	Request*	makeLengthBody(Request *req);

	std::string	getRawRequest();

private:
	RequestReader();
	RequestReader(const RequestReader &other);

	int			client_fd;
	std::string	buffer; //raw_request ?

};

#endif