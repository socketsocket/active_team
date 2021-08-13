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
	RequestReader(int client_socket);
	~RequestReader();

	void		readRequest(int read_size);
	Dialogue*	parseRequest();

	void	makeStartLine();
	void 	makeReqHeader();
	void	checkBody();
	void	makeChunkedBody();
	void	makeLengthBody();

	std::string	getRawRequest();

private:
	RequestReader();
	RequestReader(const RequestReader &other);

	RequestReader&	operator=(const RequestReader &other);

	int			client_fd;
	Dialogue	*dial;
	bool		chunked;
	long		content_length;
	std::string	buffer;
	Status		stat;
};

#endif
