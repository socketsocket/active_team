#ifndef FDManager_HPP
# define FDManager_HPP

# include <fcntl.h>

# define READ_BUFFER_SIZE	65535
# define WRITE_BUFFER_SIZE	65535

class	FDManager
{
public:
	FDManager(int fd);
	virtual ~FDManager();

	int				getFD()	{ return (fd); }

	virtual void	readEvent(long read_size) = 0;
	virtual void	writeEvent(long write_size) = 0;
	virtual void	timerEvent() = 0;

private:
	FDManager();
	FDManager(const FDManager &other);

	FDManager&	operator=(const FDManager &other);

	int	fd;
};

#endif
