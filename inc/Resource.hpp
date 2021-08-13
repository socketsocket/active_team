#ifndef RESOURCE_HPP
# define RESOURCE_HPP

# include <vector>
# include <queue>
# include <string>

# include "FDManager.hpp"
# include "Dialogue.hpp"

class	Resource : public FDManager
{
public:
	enum Type { READ, OVERWRITE, APPEND };

	Resource(int fd, Dialogue *dialogue);
	~Resource();

	virtual void	readEvent(int read_size);
	virtual void	writeEvent(int write_size);
	virtual void	timerEvent();

private:
	Resource();
	Resource(const Resource &other);

	Resource&	operator=(const Resource &other);

	Dialogue *dialogue;
};

#endif
