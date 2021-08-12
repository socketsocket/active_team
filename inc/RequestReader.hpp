#ifndef REQUESTREADER_HPP
# define REQUESTREADER_HPP

# include "Dialogue.hpp"
# include "Client.hpp"
# include "algorithm"

class	RequestReader
{
public:
	enum	Status {
		PARSING_START,
		PARSING_HEADER,
		PARSING_BODY
	};
	enum	Status {
		NOBODY,
		CHUNKED,
		CONTENT_LENGTH
	};
	RequestReader(int client_socket);
	~RequestReader();

	RequestReader&	operator=(const RequestReader &other);

	void		readRequest();
	Dialogue*	parseRequest();

	void	makeStartLine(Request &req);
	void 	makeReqHeader(Request &req);
	void	checkBody(Request &req);
	void	makeChunkedBody(Request &req);
	void	makeLengthBody(Request &req);

	std::string	getRawRequest();

private:
	RequestReader();
	RequestReader(const RequestReader &other);

	int			client_fd;
	std::string	buffer;

};

#endif
