#ifndef CGI_HPP
# define CGI_HPP

# include <vector>
# include <string>

# include "FDManager.hpp"

struct	Dialogue;

class	CGI
{
public:
	class	CGIWriter : public FDManager
	{
	public:
		CGIWriter(Dialogue *dialogue);
		virtual ~CGIWriter();

		virtual void	readEvent(long read_size, short flags);
		virtual void	writeEvent(long write_size);
		virtual void	timerEvent();

		int	getReadablePipe();

	private:
		CGIWriter(const CGIWriter &other);

		CGIWriter&	operator=(const CGIWriter &other);

		Dialogue	*dialogue;
		int			readable_pipe;
	};

	class	CGIReader : public FDManager
	{
	public:
		enum Status { HEADER, BODY, DONE };

		CGIReader(Dialogue *dialogue, CGI *cgi);
		virtual ~CGIReader();

		virtual void	readEvent(long read_size, short flags);
		virtual void	writeEvent(long write_size);
		virtual void	timerEvent();

		int	getWritablePipe();

	private:
		CGIReader(const CGIReader &other);

		CGIReader&	operator=(const CGIReader &other);

		Dialogue	*dialogue;
		int			writable_pipe;
		std::string	buffer;
		CGI			*cgi;
		Status		status;

		static const ssize_t	MAX_OFFSET;
	};

	struct Query { std::string name; std::string value; };

	CGI(std::string &script_path, std::string &resource_path, Dialogue *dialogue, int server_port);
	~CGI();

	void	start();

private:
	CGI();
	CGI(const CGI &other);

	CGI&	operator=(const CGI &other);

	CGIReader	reader;
	CGIWriter	writer;

	pid_t		script_pid;
	std::string	&script_path;
	std::string	resource_path;
	Dialogue	*dialogue;
	int			server_port;
	char		**envp;
};

#endif
