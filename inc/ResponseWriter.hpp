#ifndef RESPONSEWRITER_HPP
# define RESPONSEWRITER_HPP

class	ResponseWriter
{
public:
	ResponseWriter(Dialogue *dialogue_to_write);
	~ResponseWriter();

	bool	emptyBuffer();
	void	setResponse(Response &res);
	void	writeResponse();

private:
	ResponseWriter();
	ResponseWriter(const ResponseWriter &other);

	ResponseWriter&	operator=(const ResponseWriter &other);

	std::vector<std::string>	buffer;
};

#endif
