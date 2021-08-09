#ifndef FDHANDLER_HPP
# define FDHANDLER_HPP

class	FDHandler
{
public:
	FDHandler();
	FDHandler(int fd);
	virtual ~FDHandler();

	int	getFD() { return (fd); }

private:
	FDHandler(const FDHandler &other);

	FDHandler&	operator=(const FDHandler &other);

	int	fd;
};

#endif
