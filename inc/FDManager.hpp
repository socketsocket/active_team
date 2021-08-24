#ifndef FDManager_HPP
# define FDManager_HPP

# define READ_BUFFER_SIZE	65535
# define WRITE_BUFFER_SIZE	65535

class	FDManager
{
public:
	FDManager(int fd);
	virtual ~FDManager();

	int				getFD()	{ return (fd); }

	virtual void	readEvent(long read_size, short flags) = 0;
	virtual void	writeEvent(long write_size) = 0;
	virtual void	timerEvent() = 0;

private:
	FDManager();
	FDManager(const FDManager &other);

	FDManager&	operator=(const FDManager &other);

	int	fd;
};

#endif
