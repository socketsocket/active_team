#ifndef REQUESTREADER_HPP
# define REQUESTREADER_HPP

class	RequestReader
{
public:
	RequestReader(int client_socket);
	~RequestReader();

	RequestReader&	operator=(const RequestReader &other);

	void		readRequest();
	Dialogue*	parseRequest();

private:
	RequestReader();
	RequestReader(const RequestReader &other);

	int			client_fd;
	std::string	buffer;
};

#endif
