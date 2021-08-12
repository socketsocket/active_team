#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include <exception>
# include <string>
# include <cerrno>

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

class	BadDirectiveValue : ParsingError
{
public:
	BadDirectiveValue(std::string directive) : ParsingError(directive + ": unknown value") {}
};

class	DoubleDirective : ParsingError
{
public:
	DoubleDirective(std::string directive) : ParsingError(directive + ": double declaration") {}
};

class	NoExpectedDirective : ParsingError
{
public:
	NoExpectedDirective(std::string directive) : ParsingError("Expected `" + directive + "`") {}
};

class	UnexceptedEOF : ParsingError
{
public:
	UnexceptedEOF() : ParsingError("Unexcepted EOF") {}
};

class	UnexceptedEventOccured : std::exception
{
public:
	UnexceptedEventOccured(std::string event_object)	{ error_message = "Unexcepted event occured: " + event_object; }

	const char	*what() const throw() override
	{
		return (error_message.c_str());
	}

private:
	std::string	error_message;
};

class	UnexceptedFlowOccured : std::exception
{
public:
	UnexceptedFlowOccured(std::string flow_name)	{ error_message = "Unexcepted flow detected: " + flow_name; }

	const char* what() const throw() override
	{
		return (error_message.c_str());
	}
private:
	std::string	error_message;
};

class	BadRequest : std::exception
{
public:
	const char* what() const throw() override
	{
		return ("400 Bad Request");
	}
};

#endif
