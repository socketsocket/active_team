#ifndef CGI_HPP
# define CGI_HPP

class CGI
{
	private:
		/* data */
	public:
		CGI(/* args*/);
		CGI(const CGI &ref);
		~CGI();
		CGI& operator=(const CGI &ref);
};

#endif