#ifndef CGI_HPP
# define CGI_HPP

# include <vector>
# include <string>

# include "FDManager.hpp"

struct	Dialogue;

class	CGI : public FDManager
{
public:
	struct Query { std::string name; std::string value; };

	CGI(std::string &script_path, std::string &resource_path, Dialogue *dialogue, int server_port);
	virtual ~CGI();

	void	setPrintFD(int fd);
	void	addQuery(std::string query_string);

	virtual void	readEvent(long read_size);
	virtual void	writeEvent(long write_size);
	virtual void	timerEvent();

	void	start();

private:
	CGI();
	CGI(const CGI &other);

	CGI&	operator=(const CGI &other);

	int			script_stdin;
	pid_t		script_pid;
	std::string	&script_path;
	std::string	resource_path;
	Dialogue	*dialogue;
	int			server_port;
};

#endif
