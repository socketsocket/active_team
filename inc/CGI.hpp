#ifndef CGI_HPP
# define CGI_HPP

# include <vector>
# include <string>

# include "Dialogue.hpp"

class	CGI : public FDManager
{
public:
	struct Query { std::string name; std::string value; };

	CGI(const std::string &script_path, Dialogue *dialogue, int server_port);
	~CGI();

	void	setPrintFD(int fd);
	void	addQuery(std::string query_string);

	virtual void	readEvent(int read_size);
	virtual void	writeEvent(int write_size);
	virtual void	timerEvent();

private:
	CGI();
	CGI(const CGI &other);

	CGI&	operator=(const CGI &other);

	int			script_stdin;
	pid_t		script_pid;
	Dialogue	*dialogue;
};

#endif
