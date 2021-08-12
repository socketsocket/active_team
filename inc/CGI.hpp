#ifndef CGI_HPP
# define CGI_HPP

# include <vector>
# include <string>

# include "Dialogue.hpp"

class	CGI : public FDManager
{
public:
	struct Query { std::string name; std::string value; };

	CGI(const std::string &script_path, Dialogue *dialogue, std::string &server_name, int server_port);
	~CGI();

	void	setPrintFD(int fd);
	void	addQuery(std::string query_string);

	virtual void	readEvent();
	virtual void	writeEvent();
	virtual void	timerEvent();

private:
	CGI();
	CGI(const CGI &other);

	CGI&	operator=(const CGI &other);

	std::string							path;
	std::vector<Query>					queries;
	std::map<std::string, std::string>	meta_variables;
	int									script_stdin;
	pid_t								script_pid;
};

#endif
