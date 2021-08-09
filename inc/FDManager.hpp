#ifndef FDManager_HPP
# define FDManager_HPP

# include <fcntl.h>

class	FDManager
{
public:
	FDManager(int fd);
	virtual ~FDManager();

	int				getFD()	{ return (fd); }

	virtual void	readEvent() = 0;
	virtual void	writeEvent() = 0;

private:
	FDManager();
	FDManager(const FDManager &other);

	FDManager&	operator=(const FDManager &other);

	int	fd;
};

#endif
