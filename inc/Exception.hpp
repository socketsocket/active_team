#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include <exception>
# include <string>
# include <cerrno>

class	SystemCallError : public std::exception
{
public:
	SystemCallError(std::string function_name)	{ error_message = function_name + ": " + strerror(errno); }
	virtual ~SystemCallError() throw()	{}

	virtual const char	*what() const throw()
	{
		return (error_message.c_str());
	}

private:
	std::string	error_message;
};

class	UnknownEventIdentifier : public std::exception
{
public:
	virtual const char	*what() const throw()
	{
		return ("Unknown identifier is returned from kqueue");
	}
};

class	BadFileDescriptor : public std::exception
{
public:
	virtual const char	*what() const throw()
	{
		return ("Bad file descriptor");
	}
};

class	ParsingError : public std::exception
{
public:
	ParsingError(std::string reason)	{ error_message = "Parsing error: " + reason; }
	virtual ~ParsingError()	throw()		{}

	virtual const char	*what() const throw()
	{
		return (error_message.c_str());
	}

private:
	std::string	error_message;
};

class	BadDirective : public ParsingError
{
public:
	BadDirective(std::string directive)	: ParsingError(directive + ": unknown directive") {}
};

class	NoExpectedDirective: public ParsingError
{
public:
	NoExpectedDirective(std::string directive) : ParsingError("Expected `" + directive + "`") {}
};

// class	BadRequest : public ParsingError
// {
// public:
// 	BadRequest(int error_code)	: ParsingError(std::to_string(error_code) + ": bad request") {}
// };

class	BadDirectiveValue : public ParsingError
{
public:
	BadDirectiveValue(std::string directive) : ParsingError(directive + ": unknown value") {}
};

class	DoubleDirective : public ParsingError
{
public:
	DoubleDirective(std::string directive) : ParsingError(directive + ": double declaration") {}
};

class	UnexceptedEOF : public ParsingError
{
public:
	UnexceptedEOF() : ParsingError("Unexcepted EOF") {}
};

class	UnexceptedEventOccured : public std::exception
{
public:
	UnexceptedEventOccured(std::string event_object)	{ error_message = "Unexcepted event occured: " + event_object; }
	virtual ~UnexceptedEventOccured() throw()	{}

	virtual const char	*what() const throw()
	{
		return (error_message.c_str());
	}

private:
	std::string	error_message;
};

class	UnexceptedFlowOccured : public std::exception
{
public:
	UnexceptedFlowOccured(std::string flow_name)	{ error_message = "Unexcepted flow detected: " + flow_name; }
	virtual ~UnexceptedFlowOccured() throw()		{}

	const char* what() const throw()
	{
		return (error_message.c_str());
	}
private:
	std::string	error_message;
};

class	BadRequest : public std::exception
{
public:
	const char* what() const throw()
	{
		return ("exception: 400 Bad Request");
	}
};

class	Forbidden : public std::exception
{
public:
	const char* what() const throw()
	{
		return ("exception: 403 Forbidden");
	}
};

class	NotFound : public std::exception
{
public:
	const char* what() const throw()
	{
		return ("exception: 404 Not Found");
	}
};

class	MethodNotAllowed : public std::exception
{
public:
	const char* what() const throw()
	{
		return ("405 Method Not Allowed");
	}
};

class	Conflict : public std::exception
{
public:
	const char* what() const throw()
	{
		return ("exception: 409 Conflict");
	}
};

#endif
