#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include <exception>
# include <string>

class	SystemCallError : std::exception
{
public:
	SystemCallError(std::string function_name)	{ error_message = function_name + ": " + strerror(errno); }

	const char	*what() const throw() override
	{
		return (error_message.c_str());
	}

private:
	std::string	error_message;
};

class	UnknownEventIdentifier : std::exception
{
public:
	const char	*what() const throw() override
	{
		return ("Unknown identifier is returned from kqueue");
	}
};

class	BadFileDescriptor : std::exception
{
public:
	const char	*what() const throw() override
	{
		return ("Bad file descriptor");
	}
};

class	ParsingError : std::exception
{
public:
	ParsingError(std::string reason)	{ error_message = "Parsing error: " + reason; }

	const char	*what() const throw() override
	{
		return (error_message.c_str());
	}

private:
	std::string	error_message;
};

class	BadDirective : ParsingError
{
public:
	BadDirective(std::string directive)	: ParsingError(directive + ": unknown directive") {}
};

class	NoExpectedDirective: ParsingError
{
public:
	NoExpectedDirective(std::string directive) : ParsingError("Expected `" + directive + "`") {}
};

class	BadRequest : ParsingError
{
public:
	BadRequest(int error_code)	: ParsingError(error_code + ": bad request") {}
};

class	UnexceptedEOF : ParsingError
{
public:
	UnexceptedEOF() : ParsingError("Unexcepted EOF") {}
};

#endif