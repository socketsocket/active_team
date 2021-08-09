#ifndef CGI_HPP
# define CGI_HPP

# include <vector>
# include <string>

class	CGI
{
public:
	struct Query { std::string name; std::string value; };

	CGI(std::string path);
	~CGI();

	void	setPrintFD(int fd);
	void	addQuery(std::string query_string);

private:
	CGI();
	CGI(const CGI &other);

	CGI&	operator=(const CGI &other);

	std::string			path;
	int					print_fd;
	std::vector<Query>	queries;
};

#endif
