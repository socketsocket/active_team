#ifndef REQUESTREADER_HPP
# define REQUESTREADER_HPP

# include <algorithm>

# include "Request.hpp"
# include "Exception.hpp"

struct	Dialogue;

class	RequestReader
{
public:
	enum	Status {
		PARSING_START,
		PARSING_HEADER,
		PARSING_BODY,
		REQUEST_COMPLETE,
	};
	enum	BodyStatus {
		NOBODY,
		CHUNKED,
		CONTENT_LENGTH,
	};
	RequestReader(int client_socket);
	~RequestReader();

	void		readRequest(int read_size);
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

	RequestReader&	operator=(const RequestReader &other);

	int			client_fd;
	std::string	buffer;

};

#endif
