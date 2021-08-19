#ifndef RESOURCE_HPP
# define RESOURCE_HPP

# include <vector>
# include <queue>
# include <string>

struct	Dialogue;

# include "FDManager.hpp"

class	Resource : public FDManager
{
public:
	enum	Status { WAITING, READING, DONE };

	Resource(int fd, Dialogue *dialogue);
	virtual ~Resource();

	virtual void	readEvent(long read_size);
	virtual void	writeEvent(long write_size);
	virtual void	timerEvent();

	Status	getStatus();

private:
	Resource();
	Resource(const Resource &other);

	Resource&	operator=(const Resource &other);

	Dialogue	*dialogue;
	Status		status;

	static const long	MAX_OFFSET;
};

#endif
