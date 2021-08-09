#ifndef FDHANDLER_HPP
# define FDHANDLER_HPP

class	FDHandler
{
public:
	FDHandler(int fd);
	virtual ~FDHandler();

	int	getFD() { return (fd); }

private:
	FDHandler();
	FDHandler(const FDHandler &other);

	FDHandler&	operator=(const FDHandler &other);

	int	fd;
};

#endif
